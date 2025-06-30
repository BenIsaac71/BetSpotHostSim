/*******************************************************************************
  SERCOM Universal Synchronous/Asynchrnous Receiver/Transmitter PLIB

  Company
    Light & Wonder

  File Name
    bsc_usart.c

  Summary
    BSC SERCOM peripheral library interface.

  Description
    This file defines the interface to the BSC to USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "interrupts.h"
#include "bsc_usarts.h"
#include "definitions.h"
// *****************************************************************************
// *****************************************************************************
// // Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* SERCOM1 USART baud value for 1000000 Hz baud rate */
#define BSC_USART_INT_BAUD_VALUE            (48059UL)

static BSC_USART_OBJECT bsc_usart_objs[BSC_USART_SERCOM_MAX] =
{
    [BSC_USART_SERCOM0] = {
        .bsc_usart_id = BSC_USART_SERCOM0,
        .sercom_regs = SERCOM0_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    },
    [BSC_USART_SERCOM1] =
    {
        .bsc_usart_id = BSC_USART_SERCOM1,
        .sercom_regs = SERCOM1_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    },
    [BSC_USART_SERCOM2] =
    {
        .bsc_usart_id = BSC_USART_SERCOM2,
        .sercom_regs = SERCOM2_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    },
    [BSC_USART_SERCOM3] =
    {
        .bsc_usart_id = BSC_USART_SERCOM3,
        .sercom_regs = SERCOM3_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    },
    [BSC_USART_SERCOM4] =
    {
        .bsc_usart_id = BSC_USART_SERCOM4,
        .sercom_regs = SERCOM4_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    },
    [BSC_USART_SERCOM5] =
    {
        .bsc_usart_id = BSC_USART_SERCOM5,
        .sercom_regs = SERCOM5_REGS,
        .peripheral_clk_freq = 60000000UL, // Assuming 60MHz clock
    }
};

// *****************************************************************************
// *****************************************************************************
// Section: BSC_USART USART Interface Routines
// *****************************************************************************
// *****************************************************************************

void static BSC_USART_ErrorClear(BSC_USART_OBJECT *bsc_usart_obj)
{
    uint8_t  u8dummyData = 0U;
    USART_ERROR errorStatus = (USART_ERROR)(bsc_usart_obj->sercom_regs->USART_INT.SERCOM_STATUS & (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk));

    if (errorStatus != USART_ERROR_NONE)
    {
        /* Clear error flag */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG = (uint8_t)SERCOM_USART_INT_INTFLAG_ERROR_Msk;
        /* Clear all errors */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_STATUS = (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk);

        /* Flush existing error bytes from the RX FIFO */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & (uint8_t)SERCOM_USART_INT_INTFLAG_RXC_Msk) == (uint8_t)SERCOM_USART_INT_INTFLAG_RXC_Msk)
        {
            u8dummyData = (uint8_t)bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA;
        }
    }

    /* Ignore the warning */
    (void)u8dummyData;
}

BSC_USART_OBJECT *BSC_USART_Initialize(int usart_number)
{
    /* Check if the usart index is valid */
    if (usart_number < 0 || usart_number >= 6)
    {
        return NULL; // Invalid index, do nothing
    }

    BSC_USART_OBJECT *bsc_usart_obj = &bsc_usart_objs[usart_number];

    /*
     * Configures USART Clock Mode
     * Configures TXPO and RXPO
     * Configures Data Order
     * Configures Standby Mode
     * Configures Sampling rate
     * Configures IBON
     */

    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | SERCOM_USART_INT_CTRLA_RXPO(0x3UL) | SERCOM_USART_INT_CTRLA_TXPO(0x0UL) | SERCOM_USART_INT_CTRLA_DORD_Msk | SERCOM_USART_INT_CTRLA_IBON_Msk | SERCOM_USART_INT_CTRLA_FORM(0x0UL) | SERCOM_USART_INT_CTRLA_SAMPR(0UL) ;

    /* Configure Baud Rate */
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(BSC_USART_INT_BAUD_VALUE);

    /*
     * Configures RXEN
     * Configures TXEN
     * Configures CHSIZE
     * Configures Parity
     * Configures Stop bits
     */
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT | SERCOM_USART_INT_CTRLB_SBMODE_1_BIT | SERCOM_USART_INT_CTRLB_RXEN_Msk | SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }


    /* Enable the UART after the configurations */
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Initialize instance object */
    bsc_usart_obj->rxBuffer = NULL;
    bsc_usart_obj->rxSize = 0;
    bsc_usart_obj->rxProcessedSize = 0;
    bsc_usart_obj->rxBusyStatus = false;
    bsc_usart_obj->rxCallback = NULL;
    bsc_usart_obj->txBuffer = NULL;
    bsc_usart_obj->txSize = 0;
    bsc_usart_obj->txProcessedSize = 0;
    bsc_usart_obj->txBusyStatus = false;
    bsc_usart_obj->txCallback = NULL;
    bsc_usart_obj->errorStatus = USART_ERROR_NONE;

    return bsc_usart_obj;
}





uint32_t BSC_USART_FrequencyGet(BSC_USART_OBJECT *bsc_usart_obj)
{
    return bsc_usart_obj->peripheral_clk_freq;
}

bool BSC_USART_SerialSetup(BSC_USART_OBJECT *bsc_usart_obj, USART_SERIAL_SETUP *serialSetup, uint32_t clkFrequency)
{
    bool setupStatus       = false;
    uint32_t baudValue     = 0U;
    uint32_t sampleRate    = 0U;
    uint32_t sampleCount   = 0U;

    bool transferProgress = bsc_usart_obj->txBusyStatus;
    transferProgress = bsc_usart_obj->rxBusyStatus || transferProgress;
    if (transferProgress)
    {
        /* Transaction is in progress, so return without updating settings */
        return setupStatus;
    }

    if ((serialSetup != NULL) && (serialSetup->baudRate != 0U))
    {
        if (clkFrequency == 0U)
        {
            clkFrequency = BSC_USART_FrequencyGet(bsc_usart_obj);
        }

        if (clkFrequency >= (16U * serialSetup->baudRate))
        {
            sampleRate = 0U;
            sampleCount = 16U;
        }
        else if (clkFrequency >= (8U * serialSetup->baudRate))
        {
            sampleRate = 2U;
            sampleCount = 8U;
        }
        else if (clkFrequency >= (3U * serialSetup->baudRate))
        {
            sampleRate = 4U;
            sampleCount = 3U;
        }
        else
        {
            /* Do nothing */
        }
        baudValue = 65536U - (uint32_t)(((uint64_t)65536U * sampleCount * serialSetup->baudRate) / clkFrequency);
        /* Disable the USART before configurations */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;

        /* Wait for sync */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
            /* Do nothing */
        }

        /* Configure Baud Rate */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_BAUD = (uint16_t)SERCOM_USART_INT_BAUD_BAUD(baudValue);

        /* Configure Parity Options */
        if (serialSetup->parity == USART_PARITY_NONE)
        {
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA = (bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA & ~(SERCOM_USART_INT_CTRLA_SAMPR_Msk | SERCOM_USART_INT_CTRLA_FORM_Msk)) | SERCOM_USART_INT_CTRLA_FORM(0x0UL) | SERCOM_USART_INT_CTRLA_SAMPR((uint32_t)sampleRate);
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB = (bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk)) | ((uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits);
        }
        else
        {
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA = (bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA & ~(SERCOM_USART_INT_CTRLA_SAMPR_Msk | SERCOM_USART_INT_CTRLA_FORM_Msk)) | SERCOM_USART_INT_CTRLA_FORM(0x1UL) | SERCOM_USART_INT_CTRLA_SAMPR((uint32_t)sampleRate);
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB = (bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB & ~(SERCOM_USART_INT_CTRLB_CHSIZE_Msk | SERCOM_USART_INT_CTRLB_SBMODE_Msk | SERCOM_USART_INT_CTRLB_PMODE_Msk)) | (uint32_t) serialSetup->dataWidth | (uint32_t) serialSetup->stopBits | (uint32_t) serialSetup->parity ;
        }

        /* Wait for sync */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
            /* Do nothing */
        }

        /* Enable the USART after the configurations */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

        /* Wait for sync */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
            /* Do nothing */
        }

        setupStatus = true;
    }

    return setupStatus;
}

USART_ERROR BSC_USART_ErrorGet(BSC_USART_OBJECT *bsc_usart_obj)
{
    USART_ERROR errorStatus = bsc_usart_obj->errorStatus;

    bsc_usart_obj->errorStatus = USART_ERROR_NONE;

    return errorStatus;
}

void BSC_USART_Enable(BSC_USART_OBJECT *bsc_usart_obj)
{
    if ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA & SERCOM_USART_INT_CTRLA_ENABLE_Msk) == 0U)
    {
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

        /* Wait for sync */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
            /* Do nothing */
        }
    }
}

void BSC_USART_Disable(BSC_USART_OBJECT *bsc_usart_obj)
{
    if ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA & SERCOM_USART_INT_CTRLA_ENABLE_Msk) != 0U)
    {
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLA &= ~SERCOM_USART_INT_CTRLA_ENABLE_Msk;

        /* Wait for sync */
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
        {
            /* Do nothing */
        }
    }
}


void BSC_USART_TransmitterEnable(BSC_USART_OBJECT *bsc_usart_obj)
{
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

void BSC_USART_TransmitterDisable(BSC_USART_OBJECT *bsc_usart_obj)
{
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_TXEN_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

bool BSC_USART_Write(BSC_USART_OBJECT *bsc_usart_obj, void *buffer, const size_t size)
{
    bool writeStatus      = false;
    uint32_t processedSize = 0U;

    if ((buffer != NULL) && (size > 0U) && (bsc_usart_obj->txBusyStatus == false))
    {
        bsc_usart_obj->txBuffer = buffer;
        bsc_usart_obj->txSize = size;
        bsc_usart_obj->txBusyStatus = true;

        while (((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk) == false);
        /* 9th bit set  */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA = ((uint8_t *)(buffer))[processedSize] | 0x100U;

        processedSize += 1U;
        bsc_usart_obj->txProcessedSize = processedSize;
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET = (uint8_t)SERCOM_USART_INT_INTFLAG_DRE_Msk;

        writeStatus = true;
    }

    return writeStatus;
}


bool BSC_USART_WriteIsBusy(BSC_USART_OBJECT *bsc_usart_obj)
{
    if (bsc_usart_obj->txBusyStatus == false)
    {
        while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0);
        return false;
    }
    return true;

}

size_t BSC_USART_WriteCountGet(BSC_USART_OBJECT *bsc_usart_obj)
{
    return bsc_usart_obj->txProcessedSize;
}

void BSC_USART_WriteCallbackRegister(BSC_USART_OBJECT *bsc_usart_obj, SERCOM_USART_CALLBACK callback, uintptr_t context)
{
    bsc_usart_obj->txCallback = callback;

    bsc_usart_obj->txContext = context;
}


bool BSC_USART_TransmitComplete(BSC_USART_OBJECT *bsc_usart_obj)
{
    bool transmitComplete = false;

    if ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == SERCOM_USART_INT_INTFLAG_TXC_Msk)
    {
        transmitComplete = true;
    }

    return transmitComplete;
}

void BSC_USART_ReceiverEnable(BSC_USART_OBJECT *bsc_usart_obj)
{
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB |= SERCOM_USART_INT_CTRLB_RXEN_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

void BSC_USART_ReceiverDisable(BSC_USART_OBJECT *bsc_usart_obj)
{
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_CTRLB &= ~SERCOM_USART_INT_CTRLB_RXEN_Msk;

    /* Wait for sync */
    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

bool BSC_USART_Read(BSC_USART_OBJECT *bsc_usart_obj, void *buffer, const size_t size)
{
    bool readStatus         = false;

    if (buffer != NULL)
    {
        if (bsc_usart_obj->rxBusyStatus == false)
        {
            /* Clear error flags and flush out error data that may have been received when no active request was pending */
            BSC_USART_ErrorClear(bsc_usart_obj);

            bsc_usart_obj->rxBuffer = buffer;
            bsc_usart_obj->rxSize = size;
            bsc_usart_obj->rxProcessedSize = 0U;
            bsc_usart_obj->rxBusyStatus = true;
            bsc_usart_obj->errorStatus = USART_ERROR_NONE;

            readStatus = true;

            /* Enable receive and error interrupt */
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET = (uint8_t)(SERCOM_USART_INT_INTENSET_ERROR_Msk | SERCOM_USART_INT_INTENSET_RXC_Msk);
        }
    }

    return readStatus;
}

bool BSC_USART_ReadIsBusy(BSC_USART_OBJECT *bsc_usart_obj)
{
    return bsc_usart_obj->rxBusyStatus;
}

size_t BSC_USART_ReadCountGet(BSC_USART_OBJECT *bsc_usart_obj)
{
    return bsc_usart_obj->rxProcessedSize;
}

bool BSC_USART_ReadAbort(BSC_USART_OBJECT *bsc_usart_obj)
{
    if (bsc_usart_obj->rxBusyStatus == true)
    {
        /* Disable receive and error interrupt */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR = (uint8_t)(SERCOM_USART_INT_INTENCLR_ERROR_Msk | SERCOM_USART_INT_INTENCLR_RXC_Msk);

        bsc_usart_obj->rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        bsc_usart_obj->rxSize = 0U;
        bsc_usart_obj->rxProcessedSize = 0U;
    }

    return true;
}

void BSC_USART_ReadCallbackRegister(BSC_USART_OBJECT *bsc_usart_obj, SERCOM_USART_CALLBACK callback, uintptr_t context)
{
    bsc_usart_obj->rxCallback = callback;

    bsc_usart_obj->rxContext = context;
}


void static __attribute__((used)) BSC_USART_ISR_ERR_Handler(BSC_USART_OBJECT *bsc_usart_obj)
{
    USART_ERROR errorStatus;

    errorStatus = (USART_ERROR)(bsc_usart_obj->sercom_regs->USART_INT.SERCOM_STATUS & (uint16_t)(SERCOM_USART_INT_STATUS_PERR_Msk | SERCOM_USART_INT_STATUS_FERR_Msk | SERCOM_USART_INT_STATUS_BUFOVF_Msk));

    if (errorStatus != USART_ERROR_NONE)
    {
        /* Save the error to be reported later */
        bsc_usart_obj->errorStatus = errorStatus;

        /* Clear the error flags and flush out the error bytes */
        BSC_USART_ErrorClear(bsc_usart_obj);

        /* Disable error and receive interrupt to abort on-going transfer */
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR = (uint8_t)(SERCOM_USART_INT_INTENCLR_ERROR_Msk | SERCOM_USART_INT_INTENCLR_RXC_Msk);

        /* Clear the RX busy flag */
        bsc_usart_obj->rxBusyStatus = false;

        if (bsc_usart_obj->rxCallback != NULL)
        {
            uintptr_t rxContext = bsc_usart_obj->rxContext;

            bsc_usart_obj->rxCallback(rxContext);
        }
    }
}

void static __attribute__((used)) BSC_USART_ISR_RX_Handler(BSC_USART_OBJECT *bsc_usart_obj)
{
    uint16_t temp;


    if (bsc_usart_obj->rxBusyStatus == true)
    {
        if (bsc_usart_obj->rxProcessedSize < bsc_usart_obj->rxSize)
        {

            temp = (uint16_t)bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA;

            size_t rxProcessedSize = bsc_usart_obj->rxProcessedSize;
            if(temp & 0x0100U)
            {
                if(rxProcessedSize > 0)
                {
                    printf("a new packet has started, but the previous packet is not yet processed the rxsize bytes.\n");
                }

                /* 9th bit is set, so it is a start of packet */
                rxProcessedSize = 0;
                bsc_usart_obj->rxProcessedSize = 0;
               
            }
            temp &= 0xFF;   

            //address byte
            if(rxProcessedSize == 0) 
            {
                if((temp != bsc_usart_obj->bsc_address) || (temp != GLOBAL_ADDRESS))
                {
                    return;
                }
            }
            else 
            {
                // length byte
                if(rxProcessedSize == 1)
                {
                    bsc_usart_obj->rxSize = temp + BS_MESSAGE_ADDITIONAL_SIZE; 
                }
            }
            ((uint8_t *)bsc_usart_obj->rxBuffer)[rxProcessedSize] = (uint8_t)(temp);

            /* Increment processed size */
            rxProcessedSize++;
            bsc_usart_obj->rxProcessedSize = rxProcessedSize;

            if (rxProcessedSize == bsc_usart_obj->rxSize)
            {
                bsc_usart_obj->rxBusyStatus = false;
                bsc_usart_obj->rxSize = 0U;
                bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR = (uint8_t)(SERCOM_USART_INT_INTENCLR_RXC_Msk | SERCOM_USART_INT_INTENCLR_ERROR_Msk);

                if (bsc_usart_obj->rxCallback != NULL)
                {
                    bsc_usart_obj->rxCallback(bsc_usart_obj->rxContext);
                }
            }

        }
    }
}

void static __attribute__((used)) BSC_USART_ISR_TX_Handler(BSC_USART_OBJECT *bsc_usart_obj)
{
    bool  dataRegisterEmpty;
    bool  dataAvailable;
    if (bsc_usart_obj->txBusyStatus == true)
    {
        size_t txProcessedSize = bsc_usart_obj->txProcessedSize;

        dataAvailable = (txProcessedSize < bsc_usart_obj->txSize);
        dataRegisterEmpty = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);

        while (dataRegisterEmpty && dataAvailable)
        {
            /* 8-bit mode */
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA = ((uint8_t *)bsc_usart_obj->txBuffer)[txProcessedSize];
            /* Increment processed size */
            txProcessedSize++;

            dataAvailable = (txProcessedSize < bsc_usart_obj->txSize);
            dataRegisterEmpty = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);
        }

        bsc_usart_obj->txProcessedSize = txProcessedSize;

        if (txProcessedSize >= bsc_usart_obj->txSize)
        {
            bsc_usart_obj->txBusyStatus = false;
            bsc_usart_obj->txSize = 0U;
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR = (uint8_t)SERCOM_USART_INT_INTENCLR_DRE_Msk;

            if (bsc_usart_obj->txCallback != NULL)
            {
                uintptr_t txContext = bsc_usart_obj->txContext;
                bsc_usart_obj->txCallback(txContext);
            }
        }
    }
}

void __attribute__((used)) BSC_USART_InterruptHandler(BSC_USART_OBJECT *bsc_usart_obj)
{
    bool testCondition;
    if (bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET != 0U)
    {
        /* Checks for error flag */
        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk) == SERCOM_USART_INT_INTFLAG_ERROR_Msk);
        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_ERROR_Msk) == SERCOM_USART_INT_INTENSET_ERROR_Msk) && testCondition;
        if (testCondition)
        {
            BSC_USART_ISR_ERR_Handler(bsc_usart_obj);
        }

        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk);
        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_DRE_Msk) == SERCOM_USART_INT_INTENSET_DRE_Msk) && testCondition;
        /* Checks for data register empty flag */
        if (testCondition)
        {
            BSC_USART_ISR_TX_Handler(bsc_usart_obj);
        }

        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk) == SERCOM_USART_INT_INTFLAG_RXC_Msk);
        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_RXC_Msk) == SERCOM_USART_INT_INTENSET_RXC_Msk) && testCondition;
        /* Checks for receive complete empty flag */
        if (testCondition)
        {
            BSC_USART_ISR_RX_Handler(bsc_usart_obj);
        }
    }
}


void __attribute__((used)) SERCOM0_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[0]);
}
void __attribute__((used)) SERCOM1_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[1]);
}
void __attribute__((used)) SERCOM2_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[2]);
}
void __attribute__((used)) SERCOM3_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[3]);
}
void __attribute__((used)) SERCOM4_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[4]);
}
void __attribute__((used)) SERCOM5_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[5]);
}

