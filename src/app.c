/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"
#include "app.h"
#include "stdio.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


typedef enum
{
    DRV_USART_INDEX_MASTER,
    DRV_USART_INDEX_SLAVE0,
    DRV_USART_INDEX_SLAVE1,
    DRV_USART_INDEX_MAX
} DRV_USART_INDEX;

/* DMAC Channel 0 */
typedef enum
{
    USART_DMA_CHANNEL_TX_START,
    USART_DMA_CHANNEL_MASTER_TX = USART_DMA_CHANNEL_TX_START,
    USART_DMA_CHANNEL_SLAVE0_TX,
    USART_DMA_CHANNEL_SLAVE1_TX,

    USART_DMA_CHANNEL_RX_START,
    USART_DMA_CHANNEL_MASTER_RX = USART_DMA_CHANNEL_RX_START,
    USART_DMA_CHANNEL_SLAVE0_RX,
    USART_DMA_CHANNEL_SLAVE1_RX
} USART_DMAC_CHANNELS;

typedef enum
{
    BUF_TYPE_TX,
    BUF_TYPE_RX,
    BUF_TYPE_MAX
} BUF_TYPE;

#define USART_BUFFER_SIZE 65
typedef struct
{
    DRV_USART_INDEX index;
    sercom_registers_t *sercom_regs;
    DMAC_CHANNEL dmac_channel_tx;
    DMAC_CHANNEL dmac_channel_rx;
    uint8_t tx_buffer[USART_BUFFER_SIZE];
    uint8_t rx_buffer[USART_BUFFER_SIZE];
} MY_USART_OBJ;

MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX] =
{
    [DRV_USART_INDEX_MASTER] = {
        .index = DRV_USART_INDEX_MASTER,
        .sercom_regs = SERCOM1_REGS,
        .dmac_channel_rx = USART_DMA_CHANNEL_MASTER_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_MASTER_TX,
    },
    [DRV_USART_INDEX_SLAVE0] = {
        .index = DRV_USART_INDEX_SLAVE0,
        .sercom_regs = SERCOM5_REGS,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE0_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE0_TX,
    },
    [DRV_USART_INDEX_SLAVE1] = {
        .index = DRV_USART_INDEX_SLAVE1,
        .sercom_regs = SERCOM4_REGS,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE0_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE0_TX,
    }

};



// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


void USART_TX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)contextHandle;

    //from isr signal app task to process the next transfer
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t ulValue = 1 << p_usart_obj->index;

    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        // Transfer completed successfully
        //printf("TX DMA transfer completed for USART index %d\n", p_usart_obj->index);
    }
    else if (event == DMAC_TRANSFER_EVENT_ERROR)
    {
        ulValue |= (1 << (p_usart_obj->index + DRV_USART_INDEX_MAX)); // Set error bit
        // Handle transfer error
        //printf("TX DMA transfer error for USART index %d\n", p_usart_obj->index);
    }
    xTaskNotifyFromISR(xAPP_Tasks, ulValue, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        // If a higher priority task was woken, yield to it
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void USART_RX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)contextHandle;

    //from isr signal app task to process the next transfer
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t ulValue = 1 << p_usart_obj->index;

    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        // Transfer completed successfully
        printf("RX DMA transfer completed for USART index %d\n", p_usart_obj->index);
    }
    else if (event == DMAC_TRANSFER_EVENT_ERROR)
    {
        ulValue |= (1 << (p_usart_obj->index + DRV_USART_INDEX_MAX)); // Set error bit
        // Handle transfer error
        //printf("RX DMA transfer error for USART index %d\n", p_usart_obj->index);
    }
    xTaskNotifyFromISR(xAPP_Tasks, ulValue, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        // If a higher priority task was woken, yield to it
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}



/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
    /* Application's initial state. */
    case APP_STATE_INIT:
    {
        bool appInitialized = true;


        if (appInitialized)
        {

            //for each usart object, initialize the trasnmit buffer to a counter value + index of the // usart object
            for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
            {
                for (int j = 0; j < USART_BUFFER_SIZE; j++)
                {
                    //usart_objs[i].tx_buffer[j] = j + i;
                    usart_objs[i].tx_buffer[j] = 0;

                }
            }
            appData.state = APP_STATE_SERVICE_TASKS;
        }
        break;
    }

    case APP_STATE_SERVICE_TASKS:
    {

        printf("hi\n");
        LED_RED_Toggle();
        vTaskDelay(pdMS_TO_TICKS(100));


        for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
        {
            MY_USART_OBJ *p_usart_obj = &usart_objs[i];

            // set up dmac transfer conmplete callback
            DMAC_ChannelCallbackRegister(i + USART_DMA_CHANNEL_RX_START, (DMAC_CHANNEL_CALLBACK)USART_RX_DMA_Callback, (uintptr_t)p_usart_obj);
            if (DMAC_ChannelTransfer(i + USART_DMA_CHANNEL_RX_START, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, p_usart_obj->rx_buffer,  USART_BUFFER_SIZE))
            {
            }
            else
            {
                // Handle error
                printf("DMA transfer failed for Master RX\n");
            }
        }

        for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
        {
            MY_USART_OBJ *p_usart_obj = &usart_objs[i];

            // set up dmac transfer conmplete callback
            DMAC_ChannelCallbackRegister(i + USART_DMA_CHANNEL_TX_START, (DMAC_CHANNEL_CALLBACK)USART_TX_DMA_Callback, (uintptr_t)p_usart_obj);
            if (DMAC_ChannelTransfer(i + USART_DMA_CHANNEL_TX_START, p_usart_obj->tx_buffer, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, USART_BUFFER_SIZE))
            {
            }
            else
            {
                // Handle error
                printf("DMA transfer failed for Master TX\n");
            }
            while (DMAC_ChannelIsBusy(i + USART_DMA_CHANNEL_TX_START))
            {
                // Wait for the transfer to complete
            }
            //wait for task signal from isr
            uint32_t ulNotificationValue;
            xTaskNotifyWait(0, 1 << p_usart_obj->index, &ulNotificationValue, pdMS_TO_TICKS(1));
            //            printf("Task notified for USART index %d with value: %lu\n", i, ulNotificationValue);
        }
        break;
    }

    /* TODO: implement your application state machine.*/


    /* The default state should never be executed. */
    default:
    {
        /* TODO: Handle error in application's state machine. */
        break;
    }
    }
}


/*******************************************************************************
 End of File
 */
