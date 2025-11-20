/*******************************************************************************
  SERIAL COMMUNICATION SERIAL PERIPHERAL INTERFACE(SERCOM3_SPI) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom3_spi_slave.c

  Summary
    SERCOM3_SPI PLIB Slave Implementation File.

  Description
    This file defines the interface to the SERCOM SPI Slave peripheral library.
    This library provides access to and control of the associated
    peripheral instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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


#include "interrupts.h"
#include "plib_sercom3_spi_slave.h"
#include "peripheral/port/plib_port.h"
#include <string.h>

// *****************************************************************************
// *****************************************************************************
// Section: MACROS Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: SERCOM3_SPI Slave Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************



void SERCOM3_Initialize(void)
{
    // CHSIZE - 8_BIT, PLOADEN - 1, RXEN - 1
    SERCOM3_REGS->SPIS.SERCOM_CTRLB = 
        SERCOM_SPIS_CTRLB_CHSIZE_8_BIT
        | SERCOM_SPIS_CTRLB_PLOADEN_Msk
        | SERCOM_SPIS_CTRLB_RXEN_Msk;

    // Wait for synchronization
    while (SERCOM3_REGS->SPIS.SERCOM_SYNCBUSY != 0U)
    {
        // Do nothing
    }
    // SPI Slave Mode, DOPO - 0x0, DIPO - PAD3, CPHA - LEADING_EDGE, CPOL - IDLE_LOW, DORD - MSB, ENABLE - 1
    SERCOM3_REGS->SPIS.SERCOM_CTRLA =
        SERCOM_SPIS_CTRLA_MODE_SPI_SLAVE
        | SERCOM_SPIS_CTRLA_DOPO_0x0
        | SERCOM_SPIS_CTRLA_DIPO_PAD3
        | SERCOM_SPIS_CTRLA_CPOL_IDLE_LOW
        | SERCOM_SPIS_CTRLA_CPHA_LEADING_EDGE
        | SERCOM_SPIS_CTRLA_DORD_MSB;
        //| SERCOM_SPIS_CTRLA_ENABLE_Msk;
    
    // Wait for synchronization
    while (SERCOM3_REGS->SPIS.SERCOM_SYNCBUSY != 0U)
    {
        // Do nothing
    }
}

bool SERCOM3_Open(void)
{
    bool enabled = false;
    SERCOM3_REGS->SPIS.SERCOM_CTRLA |= SERCOM_SPIS_CTRLA_ENABLE_Msk;
     // Wait for synchronization
    while (SERCOM3_REGS->SPIS.SERCOM_SYNCBUSY != 0U)
    {
        // Do nothing
    }
    enabled = true;
    
    return enabled;
}

// Read a byte from SPI (blocking until RX is ready)
uint8_t SERCOM3_ByteRead(void)
{
    while ((SERCOM3_REGS->SPIS.SERCOM_INTFLAG & SERCOM_SPIS_INTFLAG_RXC_Msk) == 0U)
    {
        // Wait for RX ready
    }
    return SERCOM3_REGS->SPIS.SERCOM_DATA;
}

// Write a byte to SPI (blocking until TX data register is empty)
void SERCOM3_ByteWrite(uint8_t data)
{
    while ((SERCOM3_REGS->SPIS.SERCOM_INTFLAG & SERCOM_SPIS_INTFLAG_DRE_Msk) == 0U)
    {
        // Wait for TX register empty
    }
    SERCOM3_REGS->SPIS.SERCOM_DATA = data;
}

// Returns 1 if TX is busy (data register not empty), 0 if ready
uint8_t SERCOM3_IsTxReady(void)
{
    return ((SERCOM3_REGS->SPIS.SERCOM_INTFLAG & SERCOM_SPIS_INTFLAG_DRE_Msk) == 0U) ? 1 : 0;
}

// Returns 1 if RX has data available, 0 if not
uint8_t SERCOM3_IsRxReady(void)
{
    return ((SERCOM3_REGS->SPIS.SERCOM_INTFLAG & SERCOM_SPIS_INTFLAG_RXC_Msk) != 0U) ? 1 : 0;
}



