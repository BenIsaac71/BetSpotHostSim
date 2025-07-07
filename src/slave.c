#include "slave.h"

// *****************************************************************************
void SLAVE_Tasks(SLAVE_DATA *slave_data);

// *****************************************************************************
SLAVE_DATA slavesData[NUMBER_OF_SLAVES] =
{
    {
        .my_usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE0_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START, .data = SLAVE0_DATA_STR FAKE_CRC},
            .rx_buffer = {0}
        },
        .state = SLAVE_STATE_INIT,
    },
    {
        .my_usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE1_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START+1, .data = SLAVE1_DATA_STR FAKE_CRC},
            .rx_buffer = {0}
        },
        .state = SLAVE_STATE_INIT,
    }
};

// *****************************************************************************
void SLAVE_Initialize(void)
{
    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        char slave_name[] = "SLAVE#_Task";
        slave_name[5] = '0' + i;

        SLAVE_DATA *slave_data = &slavesData[i];

        xTaskCreate((TaskFunction_t)SLAVE_Tasks, slave_name, 1024, slave_data, tskIDLE_PRIORITY + 2, &slave_data->xTaskHandle);

        MY_USART_OBJ *my_usart_obj = &slave_data->my_usart_obj;

        my_usart_obj->bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM4 + i, SLAVES_ADDRESS_START+i);

        BSC_USART_ReadCallbackRegister(my_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)my_usart_obj);
    }
}
// *****************************************************************************
void SLAVE_Tasks(SLAVE_DATA *slave_data)
{
    MY_USART_OBJ *my_usart_obj = &slave_data->my_usart_obj;
    char count = 'a';
    while (true)
    {
        /* Check the application's current state. */
        switch (slave_data->state)
        {
        /* Application's initial state. */
        case SLAVE_STATE_INIT:
            while (true)
            {
                // wait for a message from the master
                begin_read(my_usart_obj);
                block_read(my_usart_obj);
                // send a response back to the master
                my_usart_obj->tx_buffer.data[0] = count;
                begin_write(my_usart_obj);
                count = (count < 'z') ? (count + 1) : 'a';
            }

            break;
        case SLAVE_STATE_WAITING_FOR_MESSAGE:
            break;
        case SLAVE_STATE_TRANSMITTING_RESPONSE:
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to allow other tasks to run
    }
}


/*******************************************************************************
 End of File
 */
