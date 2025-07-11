/*******************************************************************************
  Timer/Counter(TC0) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tc0.c

  Summary
    TC0 PLIB Implementation File.

  Description
    This file defines the interface to the TC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "interrupts.h"
#include "plib_tc0.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: TC0 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize the TC module in Timer mode */
void TC0_TimerInitialize( void )
{
    /* Reset TC */
    TC0_REGS->COUNT32.TC_CTRLA = TC_CTRLA_SWRST_Msk;

    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk) == TC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TC0_REGS->COUNT32.TC_CTRLA = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCSYNC_PRESC ;

    /* Configure in Match Frequency Mode */
    TC0_REGS->COUNT32.TC_WAVE = (uint8_t)TC_WAVE_WAVEGEN_MPWM;

    /* Configure timer period */
    TC0_REGS->COUNT32.TC_CC[0U] = 4294967295U;

    /* Clear all interrupt flags */
    TC0_REGS->COUNT32.TC_INTFLAG = (uint8_t)TC_INTFLAG_Msk;



    while((TC0_REGS->COUNT32.TC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the TC counter */
void TC0_TimerStart( void )
{
    TC0_REGS->COUNT32.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk) == TC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the TC counter */
void TC0_TimerStop( void )
{
    TC0_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk) == TC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TC0_TimerFrequencyGet( void )
{
    return (uint32_t)(58593U);
}

void TC0_TimerCommandSet(TC_COMMAND command)
{
    TC0_REGS->COUNT32.TC_CTRLBSET = (uint8_t)((uint32_t)command << TC_CTRLBSET_CMD_Pos);
    while((TC0_REGS->COUNT32.TC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Get the current timer counter value */
uint32_t TC0_Timer32bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TC0_REGS->COUNT32.TC_CTRLBSET |= (uint8_t)TC_CTRLBSET_CMD_READSYNC;

    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_CTRLB_Msk) == TC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TC0_REGS->COUNT32.TC_CTRLBSET & TC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }

    /* Read current count value */
    return TC0_REGS->COUNT32.TC_COUNT;

}

/* Configure timer counter value */
void TC0_Timer32bitCounterSet( uint32_t count )
{
    TC0_REGS->COUNT32.TC_COUNT = count;

    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_COUNT_Msk) == TC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure timer period */
void TC0_Timer32bitPeriodSet( uint32_t period )
{
    TC0_REGS->COUNT32.TC_CC[0] = period;
    while((TC0_REGS->COUNT32.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk) == TC_SYNCBUSY_CC0_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Read the timer period value */
uint32_t TC0_Timer32bitPeriodGet( void )
{
    return TC0_REGS->COUNT32.TC_CC[0];
}



/* Polling method to check if timer period interrupt flag is set */
bool TC0_TimerPeriodHasExpired( void )
{
    uint8_t timer_status = 0U;
    timer_status = (uint8_t)((TC0_REGS->COUNT32.TC_INTFLAG) & TC_INTFLAG_OVF_Msk);
    TC0_REGS->COUNT32.TC_INTFLAG = timer_status;
    return (timer_status != 0U);
}
