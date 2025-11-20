/*******************************************************************************
  SERIAL COMMUNICATION SERIAL PERIPHERAL INTERFACE(SERCOM3_SPI) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom3_spi_slave.h

  Summary
    SERCOM3_SPI PLIB Slave Header File.

  Description
    This file provides the API declarations for controlling the SERCOM3 SPI peripheral in slave mode.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef PLIB_SERCOM3_SPI_SLAVE_H 
#define PLIB_SERCOM3_SPI_SLAVE_H


// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

extern "C" {

#endif
    
#include <stdbool.h>

// DOM-IGNORE-END

 /**
  * @brief This function configures the SERCOM3 SPI module for slave operation.
  *
  * @return None.
 */
void SERCOM3_Initialize(void);

/**
 * @brief Opens the SPI slave peripheral for subsequent operations.
 *
 * This function should be called before starting SPI transactions to ensure
 * the peripheral is ready.
 *
 * @return true if the peripheral was successfully opened, false otherwise.
 */
bool SERCOM3_Open(void);

/**
 * @brief Reads a byte from SPI slave (blocking).
 *
 * This function polls until data is received, then reads a byte from the SPI slave.
 *
 * @return Byte received from master.
 */
uint8_t SERCOM3_ByteRead(void);

/**
 * @brief Writes a byte to SPI slave (blocking).
 *
 * This function polls until the data register is ready, then writes a byte for transmission.
 *
 * @param data Byte to be transmitted to the SPI master.
 * @return None.
 */
void SERCOM3_ByteWrite(uint8_t data);

/**
 * @brief Checks if SPI slave transmit register is ready for new data.
 *
 * @return 1 if transmit register is not ready (busy), 0 if ready.
 */
uint8_t SERCOM3_IsTxReady(void);

/**
 * @brief Checks if SPI slave has received data (RX ready).
 *
 * @return 1 if data received and ready to be read, 0 otherwise.
 */
uint8_t SERCOM3_IsRxReady(void);

#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif /* PLIB_SERCOM3_SPI_SLAVE_H */