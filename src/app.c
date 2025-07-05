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
char *pcTaskGetCurrentTaskName(void)
{
    return pcTaskGetName(xTaskGetCurrentTaskHandle());
}


// *****************************************************************************
void begin_read(MY_USART_OBJ *p_usart_obj)
{
    // Start the read operation
    BSC_USART_Read(p_usart_obj->bsc_usart_obj, &p_usart_obj->rx_buffer, sizeof(p_usart_obj->rx_buffer));
}

void block_write(MY_USART_OBJ *p_usart_obj)
{
    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->tx_buffer;

    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_META_SIZE);
    printf("TX:%s %02X->%02X[%s]\n", pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);
}

void block_rx_ready(MY_USART_OBJ *p_usart_obj)
{
    // Wait for RX to complete
    while (xSemaphoreTake(p_usart_obj->bsc_usart_obj->rx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for RX semaphore
    TP1_Clear();

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error == USART_ERROR_NONE)
    {
        BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
        printf("RX:%s %02X->%02X[%s]\n", pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);
    }
    else
    {
        printf("RD Error: %d\n", error);
    }

}

void rx_callback(MY_USART_OBJ *p_usart_obj)
{
    TP1_Set();

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
    // Give the semaphore to unblock the task waiting for RX data
    xSemaphoreGiveFromISR(p_usart_obj->bsc_usart_obj->rx_semaphore, &xHigherPriorityTaskWoken);    
    // If giving the semaphore unblocks a higher priority task, yield to that task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}   


// *****************************************************************************
void APP_Initialize(void)
{
    TC0_TimerStart();      // <-- Make sure this is called!

    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    BSC_USART_OBJECT *bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM1, MASTER_ADDRESS);

    my_usart_obj->bsc_usart_obj = bsc_usart_obj;
    bsc_usart_obj->rx_semaphore = xSemaphoreCreateBinary();
    BSC_USART_ReadCallbackRegister(bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)my_usart_obj);
   
    appData.state = APP_STATE_INIT;
}

// *****************************************************************************
void APP_Tasks(void)
{
    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];

    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize

    /* Check the application's current state. */
    switch (appData.state)
    {
    /* Application's initial state. */
    case APP_STATE_INIT:

        while (true)
        {
            printf("  \nTransaction %c\n", my_usart_obj->tx_buffer.data[0]);
            for (int slave_index = DRV_USART_INDEX_SLAVE0; slave_index < DRV_USART_INDEX_SLAVE0 + NUMBER_OF_SLAVES; slave_index++)
            {
                printf("M->S%d\n", slave_index);
                // build packet
                my_usart_obj->tx_buffer.to_addr = slave_index;

                // message from host to slaves
                begin_read(my_usart_obj);
                block_write(my_usart_obj);

                // response from slave to host
                block_rx_ready(my_usart_obj);
            }

            my_usart_obj->tx_buffer.data[0] = my_usart_obj->tx_buffer.data[0]  < 'Z' ? my_usart_obj->tx_buffer.data[0] + 1 : 'A';
            vTaskDelay(pdMS_TO_TICKS(2));
            LED_GREEN_Toggle();
        }
        break;
    case APP_STATE_SERVICE_TASKS:
        break;
    default:
        break;
    }
}

/*
          ┌─────────────────────────┐ ┌───────────┐          
          │      HALF DUPLEX        │ │FULL DUPLEX│          
          │ TX->A       TX->B       │ │TX->A TX->B│          
          │       RX<-A       RX<-B │ │RX<-B RX<-A│          
    ┌───┐ ├────┬──┬───┬─┬────┬──┬───┤ ├────┬──┬───┤          
    │SER│ │TX  │BS│TE#│ │RX  │BS│TE#│ │TxRx│BS│TE#│          
    │COM│ │PORT│AB│A/B│ │PORT│AB│A/B│ │PORT│AB│A/B│          
    ├───┤ ├────┼──┼───┤ ├────┼──┼───┤ ├────┼──┼───┤          
    │ 0 │ │ 0  │11│ 0A│ │ 0  │01│!0A│ │0 1 │10│ 0A│          
    │ 1 │ │ 2  │11│ 1A│ │ 2  │01│!1A│ │2 3 │10│ 1A│          
    │ 2 │ │ 4  │11│ 2A│ │ 4  │01│!2A│ │4 5 │10│ 2A│          
    │ 4 │ │ 6  │11│ 3A│ │ 6  │01│!3A│ │6 7 │10│ 3A│          
    │ 5 │ │ 8  │11│ 4A│ │ 8  │01│!4A│ │8 9 │10│ 4A│          
    ├───┤ ├────┼──┼───┤ ├────┼──┼───┤ ├────┼──┼───┤          
    │ 0 │ │ 1  │11│ 0B│ │ 1  │10│!0B│ │1 0 │01│ 0B│          
    │ 1 │ │ 3  │11│ 1B│ │ 3  │10│!1B│ │3 2 │01│ 1B│          
    │ 2 │ │ 5  │11│ 2B│ │ 5  │10│!2B│ │5 4 │01│ 2B│          
    │ 4 │ │ 7  │11│ 3B│ │ 7  │10│!3B│ │7 6 │01│ 3B│          
    │ 5 │ │ 9  │11│ 4B│ │ 9  │10│!4B│ │9 8 │01│ 4B│          
    └───┘ └────┴──┴───┘ └────┴──┴───┘ └────┴──┴───┘          
*/                                                           