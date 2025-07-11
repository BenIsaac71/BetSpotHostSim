#include "slave.h"

// *****************************************************************************
void SLAVE_Tasks(SLAVE_DATA *slave_data);

// *****************************************************************************
SLAVE_DATA slavesData[NUMBER_OF_SLAVES] =
{
    {
        .usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE0_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START, .op = BS_OP_SET_TEST, .data = SLAVE0_DATA_STR FAKE_CRC},
        },
        .state = SLAVE_STATE_INIT,
        .registry = {
            .address = {.location = {.port = 0, .id = 1}}, .led_count = 3, .hw_version = 1,
            .serial_number = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C},
        },
        .sensor_values = {.data = 0x11, .int_flag = 0x22, .id = 0x33, .ac_data = 0x44},
    },
    {
        .usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE1_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START + 1, .op = BS_OP_SET_TEST, .data = SLAVE1_DATA_STR FAKE_CRC},
        },
        .state = SLAVE_STATE_INIT,
        .registry = {
            .address = {.location = {.port = 0, .id = 2}}, .led_count = 3, .hw_version = 1,
            .serial_number = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C},
        },
        .sensor_values = {.data = 0x55, .int_flag = 0x66, .id = 0x77, .ac_data = 0x88},
    }
};

// *****************************************************************************
void SLAVE_Set_Test(MY_USART_OBJ *p_usart_obj)
{
    BS_MESSAGE_BUFFER *msg = &p_usart_obj->rx_buffer;

    printu("RX:%s %02X->%02X[%s]\n", pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, &msg->data);

    p_usart_obj->tx_buffer.data[0] = (p_usart_obj->tx_buffer.data[0] < 'z') ? (p_usart_obj->tx_buffer.data[0] + 1) : 'a';

    begin_write(p_usart_obj);
}

void SLAVE_Reset_Address(MY_USART_OBJ *p_usart_obj)
{
    // This function is called to reset the address of the slave
    // The implementation will depend on the specific requirements of the application
    printu("Resetting address for slave %02X\n", p_usart_obj->rx_buffer.to_addr);
    
    // Reset logic here, if applicable
}

void SLAVE_Set_Address(MY_USART_OBJ *p_usart_obj)
{
    // This function is called to set the address of the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting address for slave %02X\n", p_usart_obj->rx_buffer.to_addr);
    
    // Set logic here, if applicable
}

void SLAVE_Set_Sensor_Parameters(MY_USART_OBJ *p_usart_obj)
{
    // This function is called to set sensor parameters for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting sensor parameters for slave %02X\n", p_usart_obj->rx_buffer.to_addr);
    
    // Set sensor parameters logic here, if applicable
}
void SLAVE_Set_Sensor_Mode(MY_USART_OBJ *p_usart_obj)
{
    // This function is called to set the sensor mode for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting sensor mode for slave %02X\n", p_usart_obj->rx_buffer.to_addr);
    
    // Set sensor mode logic here, if applicable
}
void SLAVE_Set_LED_Colors(MY_USART_OBJ *p_usart_obj)
{
    // This function is called to set LED colors for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting LED colors for slave %02X\n", p_usart_obj->rx_buffer.to_addr);
    
    // Set LED colors logic here, if applicable
}
// *****************************************************************************
void SLAVE_Get_Registry(MY_USART_OBJ *p_usart_obj)
{
    bs_registry_entry_t *registry = &slavesData[p_usart_obj->rx_buffer.to_addr - SLAVES_ADDRESS_START].registry;

    build_packet(&p_usart_obj->tx_buffer, BS_OP_GET_REGISTRY, registry->address.addr, MASTER_ADDRESS, (char *)registry, sizeof(bs_registry_entry_t));

    begin_write(p_usart_obj);
}

void SLAVE_Get_Sensor_Values(MY_USART_OBJ *p_usart_obj)
{
    //TOOD:
}
void SLAVE_Get_Sensor_State(MY_USART_OBJ *p_usart_obj)
{
    //TOOD:
}



// *****************************************************************************
void SLAVE_Block_Read(MY_USART_OBJ *p_usart_obj)
{
    while (xSemaphoreTake(p_usart_obj->bsc_usart_obj->rx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for RX semaphore

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error == USART_ERROR_NONE)
    {
        // Process the received data
        BS_MESSAGE_BUFFER *msg = &p_usart_obj->rx_buffer;

        switch (msg->op)
        {
        case BS_OP_SET_TEST:
            SLAVE_Set_Test(p_usart_obj);
            break;
        case BS_OP_RESET_ADDRESS:
            break;
        case BS_OP_SET_ADDRESS:
            break;
        case BS_OP_SET_SENSOR_PARAMETERS:
            break;
        case BS_OP_SET_SENSOR_MODE:
            break;
        case BS_OP_SET_LED_COLORS:
            break;
        case BS_OP_GET_REGISTRY:
            SLAVE_Get_Registry(p_usart_obj);
            break;
        case BS_OP_GET_SENSOR_VALUES:
            SLAVE_Get_Sensor_Values(p_usart_obj);
            break;
        case BS_OP_GET_SENSOR_STATE:
            SLAVE_Get_Sensor_State(p_usart_obj);
            break;
        default:
            printu("Unknown operation received: %d\n", msg->op);
            break;
        }
    }
    else
    {
        printu("RD Error: %d\n", error);
    }
}

// *****************************************************************************
void SLAVE_Initialize(void)
{
    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        char slave_name[] = "SLAVE#_Task";
        slave_name[5] = (char)('0' + i);

        SLAVE_DATA *slave_data = &slavesData[i];

        xTaskCreate((TaskFunction_t)SLAVE_Tasks, slave_name, 1024, slave_data, tskIDLE_PRIORITY + 2, &slave_data->xTaskHandle);

        MY_USART_OBJ *p_usart_obj = &slave_data->usart_obj;

        p_usart_obj->bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM4 + i, (uint8_t)(SLAVES_ADDRESS_START + i));

        BSC_USART_WriteCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)tx_callback, (uintptr_t)p_usart_obj);
        BSC_USART_ReadCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)p_usart_obj);
    }
}
// *****************************************************************************
void SLAVE_Tasks(SLAVE_DATA *slave_data)
{
    MY_USART_OBJ *p_usart_obj = &slave_data->usart_obj;
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
                begin_read(p_usart_obj);
                SLAVE_Block_Read(p_usart_obj);
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
