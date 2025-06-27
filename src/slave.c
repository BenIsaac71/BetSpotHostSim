/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    slave.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system,such as drivers,
    system services,and middleware.  However,it does not call any of the
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

#include "definitions.h"
#include "sys_tasks.h"
#include "app.h"
#include "slave.h"
#include "stdio.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_USART_INDEX_MAX sizeof(usart_objs)/sizeof(MY_USART_OBJ)

/* Task handles for slave tasks */
TaskHandle_t xSlave0TaskHandle;
TaskHandle_t xSlave1TaskHandle;

/* Forward declarations for slave task functions */
void vSlave0Task(void *pvParameters);
void vSlave1Task(void *pvParameters);


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the SLAVE_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

typedef struct
{
    SLAVE_DATA stateData;
    MY_USART_OBJ *usartObj;
} SLAVE_TASK_CONTEXT;
SLAVE_TASK_CONTEXT slaveContexts[DRV_USART_INDEX_MAX] =
{
    [0] = { .stateData = { .state = SLAVE_STATE_INIT }, .usartObj = &usart_objs[DRV_USART_INDEX_SLAVE0] },
    [1] = { .stateData = { .state = SLAVE_STATE_INIT }, .usartObj = &usart_objs[DRV_USART_INDEX_SLAVE1] }
};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void vSlaveTask(void *pvParameters);

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
    void SLAVE_Initialize ( void )

  Remarks:
    See prototype in slave.h.
 */

void SLAVE_Initialize ( void )
{
    MY_USART_OBJ *p_usart_obj;

    p_usart_obj = &usart_objs[DRV_USART_INDEX_SLAVE0];
    xTaskCreate(vSlaveTask, "Slave0Task", 1024, &slaveContexts[0], tskIDLE_PRIORITY + 1, &xSlave0TaskHandle);
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_tx, (DMAC_CHANNEL_CALLBACK)USART_TX_DMA_Callback, (uintptr_t)&usart_objs[DRV_USART_INDEX_SLAVE0]);
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_rx, (DMAC_CHANNEL_CALLBACK)USART_RX_DMA_Callback, (uintptr_t)&usart_objs[DRV_USART_INDEX_SLAVE0]);

    p_usart_obj = &usart_objs[DRV_USART_INDEX_SLAVE1];
    xTaskCreate(vSlaveTask, "Slave1Task", 1024, &slaveContexts[1], tskIDLE_PRIORITY + 1, &xSlave1TaskHandle);
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_tx, (DMAC_CHANNEL_CALLBACK)USART_TX_DMA_Callback, (uintptr_t)&usart_objs[DRV_USART_INDEX_SLAVE1]);
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_rx, (DMAC_CHANNEL_CALLBACK)USART_RX_DMA_Callback, (uintptr_t)&usart_objs[DRV_USART_INDEX_SLAVE1]);

    // /* TODO: Initialize your application's state machine and other
    //  * parameters.
    //  */
}


/******************************************************************************
  Function:
    void SLAVE_Tasks ( void )

  Remarks:
    See prototype in slave.h.
 */

void vSlaveTask(void *pvParameters)
{
    SLAVE_TASK_CONTEXT *ctx = (SLAVE_TASK_CONTEXT *)pvParameters;
    MY_USART_OBJ *p_usart_obj = ctx->usartObj;
    SLAVE_DATA *slaveData = &ctx->stateData;
    uint32_t ulNotificationValue;


    while (true)
    {
        /* Check the application's current state. */
        switch ( slaveData->state )
        {
        /* Application's initial state. */
        case SLAVE_STATE_INIT:
        {
            //test code to run dma testing out of app task
            usart_objs[DRV_USART_INDEX_SLAVE0].task_handle = xAPP_Tasks;
            usart_objs[DRV_USART_INDEX_SLAVE1].task_handle = xAPP_Tasks;
            while (true)
            {
                //wait a random delay
                vTaskDelay(pdMS_TO_TICKS(rand() % 256)); // Random delay, some where at start initialize the seed with the serial number
                LED_GREEN_Toggle();
            }

            usart_objs[DRV_USART_INDEX_SLAVE0].task_handle = xSlave0TaskHandle;
            usart_objs[DRV_USART_INDEX_SLAVE1].task_handle = xSlave1TaskHandle;

            slaveData->state = SLAVE_STATE_WAITING_FOR_MESSAGE;

            break;
        }
        case SLAVE_STATE_WAITING_FOR_MESSAGE:
        {
            //receive message from master
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_rx, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, &p_usart_obj->rx_buffer, USART_BUFFER_SIZE) != true)
            {
                printf("S%d<- error\n",p_usart_obj->index);
            }
            xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_RX(p_usart_obj->index), &ulNotificationValue, 10000);
            if (ulNotificationValue == 0)
            {
                printf("S%d<-timout\n", p_usart_obj->index);
            }

            LED_RED_Clear();

            BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;

            printf("S%d<-A%d[%s]\n", msg->to_addr, msg->from_addr, msg->data);
            if (msg->from_addr == MASTER_ADDRESS)
            {
                if (msg->to_addr == p_usart_obj->address)
                {
                    printf(" ME\n");
                    slaveData->state = SLAVE_STATE_TRANSMITTING_RESPONSE;
                    vTaskDelay(5); // Small delay to allow processing
                }
                else if (msg->to_addr == GLOBAL_ADDRESS)
                {
                    printf(" G\n");
                    //vTaskDelay(pdMS_TO_TICKS(rand() % 256)); // Random delay, some where at start initialize the seed with the serial number
                    slaveData->state = SLAVE_STATE_TRANSMITTING_RESPONSE;
                }
                else
                {
                    printf(" !4me\n");
                }
            }
            break;
        }
        case SLAVE_STATE_TRANSMITTING_RESPONSE:
        {
            //transmit response to master
            printf("S%d->M\n", p_usart_obj->index);
            USART_TE_Set(p_usart_obj->index);
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_tx, &p_usart_obj->tx_buffer, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, USART_BUFFER_SIZE) != true)
            {
                printf("fail\n");
            }
            xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_TX(p_usart_obj->index), &ulNotificationValue, 100);
            if (ulNotificationValue == 0)
            {
                printf("S->timout\n");
                while(true)
                    ;
            }

            while ((p_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0);

            slaveData->state = SLAVE_STATE_WAITING_FOR_MESSAGE;
        }
        break;
        default:
        {
            break;
        }
        }
    }
}



/*******************************************************************************
 End of File
 */
