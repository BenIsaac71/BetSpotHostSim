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
char * pcTaskGetCurrentTaskName(void)
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

    

    printf("TXC:%s %d->%d[%s]\n",pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);

    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_META_SIZE);
    while (BSC_USART_WriteIsBusy(p_usart_obj->bsc_usart_obj)) // Wait for TX to complete
    {
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to allow other tasks to run
    }
}

void block_rx_ready(MY_USART_OBJ *p_usart_obj)
{
    // Wait for RX to complete
    while (BSC_USART_ReadIsBusy(p_usart_obj->bsc_usart_obj)) // Wait for RX to complete
    {
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to allow other tasks to run
    }

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error != USART_ERROR_NONE)
    {
        printf("RD Error: %d\n", error);
        // Handle error as needed
    }

    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
    printf("RXC:%s %d->%d[%s]\n",pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);
}

volatile uint32_t ulHighFrequencyTimerTicks = 0;


// *****************************************************************************
void APP_Initialize(void)
{
    TC0_TimerStart();      // <-- Make sure this is called!

    MY_USART_OBJ *my_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    BSC_USART_OBJECT *bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM1, MASTER_ADDRESS);
    my_usart_obj->bsc_usart_obj = bsc_usart_obj;

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
            vTaskDelay(pdMS_TO_TICKS(500));
            LED_GREEN_Toggle();
        }
        break;
    case APP_STATE_SERVICE_TASKS:
        break;
    default:
        break;
    }
}
