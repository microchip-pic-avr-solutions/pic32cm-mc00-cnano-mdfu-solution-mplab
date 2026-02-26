/**
 * © 2026 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms
 * applicable to your use of third party software (including open
 * source software) that may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
 * PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE
 * OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED,
 * EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
 * DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
 * THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU
 * HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * @file        bl_app_verify.c
 * @ingroup     mdfu_client_32bit
 *
 * @brief       Contains APIs to support verification of the
 *              application image space.
 */

#include <stdint.h>
#include <stdbool.h>
#include "bl_app_verify.h"
#include "bl_config.h"
#include "bl_image_manager.h"
#include "../../../peripheral/nvmctrl/plib_nvmctrl.h"
#include "../../../peripheral/dsu/plib_dsu.h"
#include "../../../peripheral/pac/plib_pac.h"

/**
 * @ingroup mdfu_client_32bit
 * @brief Calculates the CRC32 checksum for a specified memory region.
 *
 * This function computes the CRC32 checksum for given range of memory starting
 * at the given address and spanning the specified length. The result is
 * stored in the provided CRC seed pointer. This function utilized the DSU peripheral.
 *
 * @param [in] startAddress - The starting address of the memory block to calculate the CRC for
 * @param [in] length - The length of the memory block in bytes
 * @param [in,out] crc - Pointer to a variable where the calculated CRC32 checksum will be stored. This variable must
 * be passed to the function with the CRC seed value set at the pointer.
 * @return None
 */
static void CRC32_Calculate(uint32_t startAddress, uint32_t length, uint32_t *crc);
/**
 * @ingroup mdfu_client_32bit
 * @brief Validates the CRC32 checksum for a specified memory region.
 *
 * This function checks the CRC32 checksum of a memory block against a
 * stored CRC value to verify data integrity and then returns a value indicating
 * whether the validation was successful or not.
 *
 * @param [in] startAddress - The starting address of the memory block to validate
 * @param [in] length - The length of the memory block in bytes
 * @param [in] crcAddress - The address where the expected CRC32 checksum is stored
 * @return @ref BL_PASS - Bootloader verified the application image with no errors \n
 * @return @ref BL_FAIL - Bootloader encountered an error and failed unexpectedly \n
 * @return @ref BL_ERROR_VERIFICATION_FAIL - Bootloader image verification failed \n
 */
static bl_result_t CRC32_Validate(uint32_t startAddress, uint32_t length, uint32_t crcAddress);


static void CRC32_Calculate(uint32_t startAddress, uint32_t length, uint32_t *crc)
{
    // Set the CRC seed from the given pointer
    uint32_t workCrc = *crc;

    PAC_PeripheralProtectSetup(PAC_PERIPHERAL_DSU, PAC_PROTECTION_CLEAR);

    (void) DSU_CRCCalculate(startAddress, length, workCrc, &workCrc);

    PAC_PeripheralProtectSetup(PAC_PERIPHERAL_DSU, PAC_PROTECTION_SET);

    // Set the CRC value in the given output pointer
    *crc = workCrc;
}

static bl_result_t CRC32_Validate(uint32_t startAddress, uint32_t length, uint32_t refAddress)
{
    bl_result_t result = BL_FAIL;
    uint32_t crc = 0xFFFFFFFF;
    uint32_t refCRC = 0x00000000U;

    CRC32_Calculate(startAddress, length, &crc);
    (void)NVMCTRL_Read(&refCRC, 4U, refAddress);

    if ((refCRC == 0U) || (crc == 0U) || (refCRC == 0xFFFFFFFFU) || (crc == 0xFFFFFFFFU)) 
    {
        result = BL_ERROR_INVALID_ARGUMENTS;
    }
    else if (refCRC != crc)
    {
        result = BL_ERROR_VERIFICATION_FAIL;
    }
    else 
    {
        result = BL_PASS;
    }

    return result;
}

bl_result_t BL_ImageVerify(void)
{
    // The staging area must always be validated when the FTP is connected to the core.
    // Verify the staging area
    bl_result_t verificationStatus = BL_ImageVerifyById(BL_STAGING_IMAGE_ID);

    // The protocol calls for having Anti-Rollback notify the host of the failure in versions at the time of the update
#if (BL_ANTI_ROLLBACK_ENABLED == 1)
    if (BL_PASS == verificationStatus)
    {
        // Perform rollback check on the data held at the staging area
        if (true == BL_ApplicationRollbackCheck((uint8_t) BL_STAGING_IMAGE_ID))
        {
            verificationStatus = BL_PASS;
        }
        else
        {
            verificationStatus = BL_ERROR_ROLLBACK_FAILURE;
        }
    }
#endif
    return verificationStatus;
}

bl_result_t BL_ImageVerifyById(uint8_t installLocationId)
{
    bl_result_t result = BL_ERROR_VERIFICATION_FAIL;
    bl_footer_data_t footerData = {
        .applicationId = 0U,
        .applicationVersion = 0U,
        .verificationEndAddress = 0U,
        .verificationStartAddress = 0U,
#if BL_HASH_DATA_SIZE != 0U
        .verificationData = 0U
#endif
    };
    
    (void) BL_ApplicationFooterRead(installLocationId, &footerData);

    uint32_t footerStartAddress = BL_ApplicationFooterStartAddressGet(installLocationId);

    uint32_t hashLength = ((footerData.verificationEndAddress + 1U) - footerData.verificationStartAddress);

    if ((0U == footerData.verificationStartAddress) ||
        (0U == hashLength) ||
        (installLocationId > (BL_APPLICATION_IMAGE_COUNT - 1U))
    )
    {
        result = BL_ERROR_INVALID_ARGUMENTS;
    }
    else
    {
        if (installLocationId != (uint8_t)IMAGE_0)
        {
            // Recalculating start address to verify the staging area 
            uint32_t offset = (uint32_t)((uint32_t)installLocationId & 0x00FFU) * (uint32_t) BL_IMAGE_PARTITION_SIZE;
            // This mathematical relation will be consistent as long as the execution image starts at BL_APPLICATION_START_ADDRESS and the sizes of the image areas are the same
            footerData.verificationStartAddress += offset;
        }
        result = CRC32_Validate((uint32_t) footerData.verificationStartAddress, hashLength, footerStartAddress + (uint32_t)HASH_DATA_OFFSET);
    }
    return result;
}