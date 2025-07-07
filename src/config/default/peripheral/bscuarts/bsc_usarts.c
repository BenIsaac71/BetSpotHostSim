/*******************************************************************************
  SERCOM Universal Synchronous/Asynchronous Receiver/Transmitter PLIB

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

void BSC_DMAC_RXChannelCallback(DMAC_TRANSFER_EVENT event, uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// // Section: Global Data
// *****************************************************************************
// *****************************************************************************
/* SERCOM1 USART baud value for 100000 Hz baud rate */
//#define BSC_USART_INT_BAUD_VALUE            (63788UL)
/* SERCOM1 USART baud value for 1000000 Hz baud rate */
#define BSC_USART_INT_BAUD_VALUE            (48059UL)


// *****************************************************************************
#define BSC_USART_OBJECT_INIT(SERCOM_NUM) \
    { \
        .bsc_usart_id = BSC_USART_SERCOM##SERCOM_NUM, \
        .dmac_channel_tx = SERCOM##SERCOM_NUM##_DMAC_TX_CHANNEL, \
        .dmac_channel_rx = SERCOM##SERCOM_NUM##_DMAC_RX_CHANNEL, \
        .sercom_regs = SERCOM##SERCOM_NUM##_REGS, \
        .peripheral_clk_freq = 60000000UL, \
    }

static BSC_USART_OBJECT bsc_usart_objs[BSC_USART_SERCOM_MAX] =
{
    BSC_USART_OBJECT_INIT(1),
    BSC_USART_OBJECT_INIT(4),
    BSC_USART_OBJECT_INIT(5)
};

// *****************************************************************************
// TE functions for SERCOM_USART#BANKA\B
// *****************************************************************************
#define BSC_USART_TE_funcs(SERCOM_NUM) \
    static void BSC_TE_SET_SERCOM##SERCOM_NUM##_BANKA(void) { SERCOM##SERCOM_NUM##_TE_A_Set(); } \
    static void BSC_TE_CLR_SERCOM##SERCOM_NUM##_BANKA(void) { SERCOM##SERCOM_NUM##_TE_A_Clear(); } \
    static void BSC_TE_SET_SERCOM##SERCOM_NUM##_BANKB(void) { SERCOM##SERCOM_NUM##_TE_B_Set(); } \
    static void BSC_TE_CLR_SERCOM##SERCOM_NUM##_BANKB(void) { SERCOM##SERCOM_NUM##_TE_B_Clear(); }

BSC_USART_TE_funcs(1)
BSC_USART_TE_funcs(4)
BSC_USART_TE_funcs(5)

#define BSC_USART_SET_BANK_ASSIGN(SERCOM_NUM, BANK) \
    bsc_usart_objs[BSC_USART_SERCOM##SERCOM_NUM].te_set = BSC_TE_SET_SERCOM##SERCOM_NUM##_##BANK; \
    bsc_usart_objs[BSC_USART_SERCOM##SERCOM_NUM].te_clr = BSC_TE_CLR_SERCOM##SERCOM_NUM##_##BANK;

#define RE_BANK_A() BANK_SEL_A_Clear(); BANK_SEL_B_Set();
#define RE_BANK_B() BANK_SEL_A_Set();   BANK_SEL_B_Clear(); 

void BSC_USART_SetBank(BSC_USART_BANK bank)
{
    if (bank == BSC_USART_BANK_A)
    {
        RE_BANK_A(); 
        BSC_USART_SET_BANK_ASSIGN(1, BANKA)
        BSC_USART_SET_BANK_ASSIGN(4, BANKA)
        BSC_USART_SET_BANK_ASSIGN(5, BANKA)
    }
    else
    {
        RE_BANK_B();
        BANK_SEL_A_Set();
        BANK_SEL_B_Clear();
        BSC_USART_SET_BANK_ASSIGN(1, BANKB)
        BSC_USART_SET_BANK_ASSIGN(4, BANKB)
        BSC_USART_SET_BANK_ASSIGN(5, BANKB)
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: BSC_USART Interface Routines
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

BSC_USART_OBJECT *BSC_USART_Initialize(BSC_USART_SERCOM_ID sercom_id, uint8_t address)
{
    /* Check if the usart index is valid */
    if (sercom_id >= BSC_USART_SERCOM_MAX)
    {
        return NULL; // Invalid index, do nothing
    }

    BSC_USART_OBJECT *bsc_usart_obj = &bsc_usart_objs[sercom_id];

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

    DMAC_ChannelCallbackRegister(bsc_usart_obj->dmac_channel_rx, BSC_DMAC_RXChannelCallback, (uintptr_t)bsc_usart_obj);

    /* Initialize instance object */
    bsc_usart_obj->address = address;
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

    bsc_usart_obj->rx_semaphore = xSemaphoreCreateBinary();

    BSC_USART_SetBank(BSC_USART_BANK_A); // Default to Bank A

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

    if (buffer != NULL)
    {
        if (bsc_usart_obj->txBusyStatus == false)
        {
            bsc_usart_obj->txBuffer = buffer;
            bsc_usart_obj->txSize = size;
            bsc_usart_obj->txBusyStatus = true;

            bsc_usart_obj->te_set();

            size_t txSize = bsc_usart_obj->txSize;

            /* Initiate the transfer by sending first byte */
            while (((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) == SERCOM_USART_INT_INTFLAG_DRE_Msk) &&
                    (processedSize < txSize))
            {
                if (processedSize != 0)
                {
                    /* 8-bit mode */
                    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA = ((uint8_t *)(buffer))[processedSize];
                }
                else
                {
                    /* 9-bit mode */
                    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA = ((uint8_t *)(buffer))[processedSize] | 0x100U;;
                    while ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk) != SERCOM_USART_INT_INTFLAG_DRE_Msk)
                    {
                        /* Wait for DRE flag to be set next xfer will clear 9th bit */
                    };

                }
                processedSize += 1U;
            }
            bsc_usart_obj->txProcessedSize = processedSize;

            uint8_t *dst = (uint8_t *)&bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA;
            uint8_t *src = &((uint8_t *)bsc_usart_obj->txBuffer)[processedSize];

            /* Set up the DMAC transfer */
            DMAC_ChannelTransfer(bsc_usart_obj->dmac_channel_tx, src, dst, bsc_usart_obj->txSize - processedSize);
            bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET = (uint8_t)SERCOM_USART_INT_INTENCLR_TXC_Msk;
            writeStatus = true;
        }
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

void BSC_DMAC_RXChannelCallback(DMAC_TRANSFER_EVENT event, uintptr_t context)
{
    BSC_USART_OBJECT *bsc_usart_obj = (BSC_USART_OBJECT *)context;

    bsc_usart_obj->rxProcessedSize += DMAC_ChannelGetTransferredCount(bsc_usart_obj->dmac_channel_rx);
    uint8_t address = ((uint8_t *)bsc_usart_obj->rxBuffer)[0];
    // If all bytes received, finish up
    if ((bsc_usart_obj->address == address || bsc_usart_obj->address == GLOBAL_ADDRESS) &&
            bsc_usart_obj->rxProcessedSize == bsc_usart_obj->rxSize)
    {
        bsc_usart_obj->rxBusyStatus = false;
        bsc_usart_obj->rxSize = 0U;
        if (bsc_usart_obj->rxCallback != NULL)
        {
            bsc_usart_obj->rxCallback(bsc_usart_obj->rxContext);
        }
    }
    else
    {
        // not for me or bad packet, start looking for new packet
        bsc_usart_obj->rxProcessedSize = 0;
        bsc_usart_obj->rxSize = 2U;//need at least 2 bytes to start looking for new packet
        bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET =
            (uint8_t)(SERCOM_USART_INT_INTENSET_RXC_Msk | SERCOM_USART_INT_INTENSET_ERROR_Msk);
    }
}

void static __attribute__((used)) BSC_USART_ISR_RX_Handler(BSC_USART_OBJECT *bsc_usart_obj)
{
    if (bsc_usart_obj->rxBusyStatus == true)
    {
        if (bsc_usart_obj->rxProcessedSize < bsc_usart_obj->rxSize)
        {
            uint16_t temp = bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA;

            if (temp & 0x0100U)
            {
                // address with 9th bit set, indicates start of packet
                ((uint8_t *)bsc_usart_obj->rxBuffer)[0] = temp;
                bsc_usart_obj->rxProcessedSize = 1;
            }
            else if (bsc_usart_obj->rxProcessedSize == 1)
            {
                // receive packet length = payload length + meta
                bsc_usart_obj->rxSize = temp + BS_MESSAGE_META_SIZE;
                /* Store received byte */
                ((uint8_t *)bsc_usart_obj->rxBuffer)[bsc_usart_obj->rxProcessedSize] = (uint8_t)temp;
                bsc_usart_obj->rxProcessedSize++;

                // Set up the DMAC transfer
                uint8_t *dst = &((uint8_t *)bsc_usart_obj->rxBuffer)[bsc_usart_obj->rxProcessedSize];
                uint8_t *src = (uint8_t *)&bsc_usart_obj->sercom_regs->USART_INT.SERCOM_DATA;
                DMAC_ChannelTransfer(bsc_usart_obj->dmac_channel_rx, src, dst, bsc_usart_obj->rxSize - bsc_usart_obj->rxProcessedSize);

                // Disable RX and error interrupts
                bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR =
                    (uint8_t)(SERCOM_USART_INT_INTENCLR_RXC_Msk | SERCOM_USART_INT_INTENCLR_ERROR_Msk);
            }
        }
    }
}

void static __attribute__((used)) BSC_USART_ISR_TXC_Handler(BSC_USART_OBJECT *bsc_usart_obj)
{
    bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENCLR = (uint8_t)SERCOM_USART_INT_INTENCLR_TXC_Msk;
    bsc_usart_obj->te_clr();
    bsc_usart_obj->txBusyStatus = false;
    bsc_usart_obj->txSize = 0U;

    if (bsc_usart_obj->txCallback != NULL)
    {
        uintptr_t txContext = bsc_usart_obj->txContext;
        bsc_usart_obj->txCallback(txContext);
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

        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == SERCOM_USART_INT_INTFLAG_TXC_Msk);
        testCondition = ((bsc_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTFLAG_TXC_Msk) == SERCOM_USART_INT_INTFLAG_TXC_Msk) && testCondition;
        /* Checks for data register empty flag */
        if (testCondition)
        {
            BSC_USART_ISR_TXC_Handler(bsc_usart_obj);
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

void BSC_USART_SetAddress(BSC_USART_OBJECT *bsc_usart_obj, uint8_t address)
{
    bsc_usart_obj->address = address;
}

void __attribute__((used)) SERCOM1_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[BSC_USART_SERCOM1]);
}

void __attribute__((used)) SERCOM4_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[BSC_USART_SERCOM4]);
}

void __attribute__((used)) SERCOM5_USART_InterruptHandler(void)
{
    /* Call the USART interrupt handler */
    BSC_USART_InterruptHandler(&bsc_usart_objs[BSC_USART_SERCOM5]);
}

