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
#include "slave.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_USART_INDEX_MAX sizeof(usart_objs)/sizeof(MY_USART_OBJ)

/* Task handles for slave tasks */
TaskHandle_t xSlave0TaskHandle;
TaskHandle_t xSlave1TaskHandle;


// *****************************************************************************
void SLAVE_TX_Callback(uintptr_t context)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)context;

    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->tx_buffer;
    printf("STXC %d<-%d[%s]\n", msg->to_addr, msg->from_addr, msg->data);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(p_usart_obj->task_handle, USART_SIGNAL_TX_COMPLETE, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
void SLAVE_RX_Callback(uintptr_t context)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)context;

    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->tx_buffer;
    printf("SRXC %d<-%d[%s]\n", msg->to_addr, msg->from_addr, msg->data);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(p_usart_obj->task_handle, USART_SIGNAL_TX_COMPLETE, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// *****************************************************************************
void SLAVE0_Initialize(void)
{
    xTaskCreate(SLAVE0_Tasks, "Slave0Task", 1024, NULL, tskIDLE_PRIORITY + 1, &xSlave0TaskHandle);

    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_SLAVE0];
    BSC_USART_OBJECT *bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM5, SLAVE0_ADDRESS);

    my_usart_obj->bsc_usart_obj = bsc_usart_obj;
    my_usart_obj->task_handle = xSlave0TaskHandle;

    BSC_USART_WriteCallbackRegister(bsc_usart_obj, SLAVE_TX_Callback, (uintptr_t)my_usart_obj);
    BSC_USART_ReadCallbackRegister(bsc_usart_obj, SLAVE_RX_Callback, (uintptr_t)my_usart_obj);

}
void SLAVE1_Initialize(void)
{
    xTaskCreate(SLAVE1_Tasks, "Slave1Task", 1024, NULL, tskIDLE_PRIORITY + 1, &xSlave1TaskHandle);

    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_SLAVE1];
    BSC_USART_OBJECT *bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM4, SLAVE1_ADDRESS);

    my_usart_obj->bsc_usart_obj = bsc_usart_obj;
    my_usart_obj->task_handle = xSlave0TaskHandle;

    BSC_USART_WriteCallbackRegister(bsc_usart_obj, SLAVE_TX_Callback, (uintptr_t)my_usart_obj);
    BSC_USART_ReadCallbackRegister(bsc_usart_obj, SLAVE_RX_Callback, (uintptr_t)my_usart_obj);
}


// *****************************************************************************
void SlaveTasks(SLAVE_DATA *slave_data, MY_USART_OBJ *my_usart_obj)
{

    while (true)
    {
        /* Check the application's current state. */
        switch (slave_data->state)
        {
        /* Application's initial state. */
        case SLAVE_STATE_INIT:
            printf("Slave %d Initialized\n", my_usart_obj->bsc_usart_obj->address);
            slave_data->state = SLAVE_STATE_WAITING_FOR_MESSAGE;
            break;
        case SLAVE_STATE_WAITING_FOR_MESSAGE:
            printf("Slave %d Waiting for Message\n", my_usart_obj->bsc_usart_obj->address);
            slave_data->state = SLAVE_STATE_WAITING_FOR_MESSAGE;
            break;
        case SLAVE_STATE_TRANSMITTING_RESPONSE:
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to allow other tasks to run
    }
}

SLAVE_DATA slave_data[NUMBER_OF_SLAVES] =
{
    {SLAVE_STATE_INIT}, // Slave 0 data
    {SLAVE_STATE_INIT}  // Slave 1 data;
};

// *****************************************************************************

void SLAVE0_Tasks(void *pvParameters)
{
    SlaveTasks(&slave_data[0], &usart_objs[DRV_USART_INDEX_SLAVE0]);
}

void SLAVE1_Tasks(void *pvParameters)
{
    SlaveTasks(&slave_data[1], &usart_objs[DRV_USART_INDEX_SLAVE1]);
}

/*******************************************************************************
 End of File
 */
