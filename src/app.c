#include "app.h"
#include "sys_tasks.h"
#include "slave.h"

#define FAKE_CRC "\xFF\x00\x00\xFF"
// *****************************************************************************
MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX] =
{
    [DRV_USART_INDEX_MASTER] =
    {
        .index = DRV_USART_INDEX_MASTER,
        .tx_buffer = {.to_addr = GLOBAL_ADDRESS, .data_len = sizeof(MASTER_DATA) - 1,  .from_addr = MASTER_ADDRESS, .data = MASTER_DATA FAKE_CRC}
    },
    [DRV_USART_INDEX_SLAVE0] = {
        .index = DRV_USART_INDEX_SLAVE0,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE0_DATA) - 1, .from_addr = SLAVE0_ADDRESS, .data = SLAVE0_DATA FAKE_CRC}
    },
    [DRV_USART_INDEX_SLAVE1] = {
        .index = DRV_USART_INDEX_SLAVE1,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE1_DATA) - 1, .from_addr = SLAVE1_ADDRESS, .data = SLAVE1_DATA FAKE_CRC}
    }
};

APP_DATA appData;

// *****************************************************************************
void begin_read(MY_USART_OBJ *p_usart_obj)
{
    // Start the read operation
    BSC_USART_Read(p_usart_obj->bsc_usart_obj, &p_usart_obj->rx_buffer, sizeof(p_usart_obj->rx_buffer));
}

void block_write(MY_USART_OBJ *p_usart_obj)
{
    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_META_SIZE);
    while (BSC_USART_WriteIsBusy(p_usart_obj->bsc_usart_obj)); // Wait for TX to complete
    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error != USART_ERROR_NONE)
    {
        printf("USART Error: %d\n", error);
        // Handle error as needed
    }
}

void block_rx_ready(MY_USART_OBJ *p_usart_obj)
{
    // Wait for RX to complete
    while (BSC_USART_ReadIsBusy(p_usart_obj->bsc_usart_obj)); // Wait for RX to complete
    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error != USART_ERROR_NONE)
    {
        printf("USART Error: %d\n", error);
        // Handle error as needed
    }
}

// *****************************************************************************
void MASTER_TX_Callback(uintptr_t context)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)context;

    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->tx_buffer;
    printf("MTXC %d<-%d[%s]\n", msg->to_addr, msg->from_addr, msg->data);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(p_usart_obj->task_handle, USART_SIGNAL_TX_COMPLETE, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
void MASTER_RX_Callback(uintptr_t context)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)context;

    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
    printf("MRXC %d<-%d[%s]\n", msg->to_addr, msg->from_addr, msg->data);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(p_usart_obj->task_handle, USART_SIGNAL_RX_COMPLETE, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// *****************************************************************************
void APP_Initialize(void)
{
    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    BSC_USART_OBJECT *bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM1, MASTER_ADDRESS);
    my_usart_obj->bsc_usart_obj = bsc_usart_obj;
    BSC_USART_WriteCallbackRegister(bsc_usart_obj, MASTER_TX_Callback, (uintptr_t)my_usart_obj);
    BSC_USART_ReadCallbackRegister(bsc_usart_obj, MASTER_RX_Callback, (uintptr_t)my_usart_obj);

    appData.state = APP_STATE_INIT;
}


// *****************************************************************************
void APP_Tasks(void)
{

    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    my_usart_obj->task_handle = xTaskGetCurrentTaskHandle();

    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize

    char count = 'A';


    /* Check the application's current state. */
    switch (appData.state)
    {
    /* Application's initial state. */
    case APP_STATE_INIT:

        while (true)
        {
            printf("  \nTransaction %c\n", count);
            for (int slave_index = DRV_USART_INDEX_SLAVE0; slave_index < DRV_USART_INDEX_SLAVE0 + NUMBER_OF_SLAVES; slave_index++)
            {
                printf("M->S%d\n", slave_index);
                // build packet
                usart_objs[DRV_USART_INDEX_MASTER].tx_buffer.data[0] = count;
                usart_objs[DRV_USART_INDEX_MASTER].tx_buffer.to_addr = slave_index;

                // message from host to slaves
                begin_read(&usart_objs[DRV_USART_INDEX_MASTER]);
                block_write(&usart_objs[DRV_USART_INDEX_MASTER]);

                // response from slave to host
                block_rx_ready(&usart_objs[DRV_USART_INDEX_MASTER]);

            }
            LED_GREEN_Toggle();
            count++; // Increment count for next transaction
            if(count =='A'+5)
            {
                vTaskDelay(pdMS_TO_TICKS(5000));
                count = 'A'; // Reset count after 5 transactions
            }
        }
        break;
    case APP_STATE_SERVICE_TASKS:
        break;
    default:
        break;
    }
}