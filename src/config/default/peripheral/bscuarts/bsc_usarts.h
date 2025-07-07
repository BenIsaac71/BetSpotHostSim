/*******************************************************************************
  SERCOM Universal Synchronous/Asynchrnous Receiver/Transmitter PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_sercom1_usart.h

  Summary
    USART peripheral library interface.

  Description
    This file defines the interface to the USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef BSC_USART_H // Guards against multiple inclusion
#define BSC_USART_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "bsc_usarts_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END



// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
BSC_USART_OBJECT *BSC_USART_Initialize( BSC_USART_SERCOM_ID sercom_id, uint8_t address );

void BSC_USART_SetBank(BSC_USART_BANK bank);

bool BSC_USART_SerialSetup( BSC_USART_OBJECT *bsc_usart_obj, USART_SERIAL_SETUP *serialSetup, uint32_t clkFrequency );

void BSC_USART_Enable( BSC_USART_OBJECT *bsc_usart_obj );

void BSC_USART_Disable( BSC_USART_OBJECT *bsc_usart_obj );

void BSC_USART_TransmitterEnable( BSC_USART_OBJECT *bsc_usart_obj );

void BSC_USART_TransmitterDisable( BSC_USART_OBJECT *bsc_usart_obj );

bool BSC_USART_Write( BSC_USART_OBJECT *bsc_usart_obj, void *buffer, const size_t size );

bool BSC_USART_TransmitComplete( BSC_USART_OBJECT *bsc_usart_obj );


bool BSC_USART_WriteIsBusy( BSC_USART_OBJECT *bsc_usart_obj );

size_t BSC_USART_WriteCountGet( BSC_USART_OBJECT *bsc_usart_obj );

void BSC_USART_WriteCallbackRegister( BSC_USART_OBJECT *bsc_usart_obj, SERCOM_USART_CALLBACK callback, uintptr_t context );


void BSC_USART_ReceiverEnable( BSC_USART_OBJECT *bsc_usart_obj );

void BSC_USART_ReceiverDisable( BSC_USART_OBJECT *bsc_usart_obj );

bool BSC_USART_Read(BSC_USART_OBJECT *bsc_usart_obj, void *buffer, const size_t size );

bool BSC_USART_ReadIsBusy( BSC_USART_OBJECT *bsc_usart_obj );

size_t BSC_USART_ReadCountGet( BSC_USART_OBJECT *bsc_usart_obj );

bool BSC_USART_ReadAbort(BSC_USART_OBJECT *bsc_usart_obj);

void BSC_USART_ReadCallbackRegister( BSC_USART_OBJECT *bsc_usart_obj, SERCOM_USART_CALLBACK callback, uintptr_t context );

USART_ERROR BSC_USART_ErrorGet( BSC_USART_OBJECT *bsc_usart_obj );

uint32_t BSC_USART_FrequencyGet( BSC_USART_OBJECT *bsc_usart_obj);

void BSC_USART_SetAddress(BSC_USART_OBJECT *bsc_usart_obj, uint8_t address);

void BSC_USART_SetMode(BSC_USART_OBJECT *bsc_usart_obj, uint8_t address);

//dummy for initialization for harmony framework
#define SERCOM0_USART_Initialize()
#define SERCOM1_USART_Initialize()
#define SERCOM2_USART_Initialize()
#define SERCOM3_USART_Initialize()
#define SERCOM4_USART_Initialize()
#define SERCOM5_USART_Initialize()


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif
// DOM-IGNORE-END

#endif //BSC_USART_H
