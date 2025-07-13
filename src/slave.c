#include "slave.h"

// *****************************************************************************
void SLAVE_Tasks(SLAVE_OBJ *slave_data);

bool spotIn = true; // Simulate SPOT_CHECK_IN pull up input to first position bet spot

// *****************************************************************************
SLAVE_OBJ slave_objs[NUMBER_OF_SLAVES] =
{
    {
        .usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE0_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START, .op = BS_OP_SET_TEST, .data = SLAVE0_DATA_STR FAKE_CRC},
        },
        .state = SLAVE_STATE_INIT,
        .bs_object =
        {
            .registry = {
                .address = {.location = {.port = 0, .id = 1}}, .led_count = 3, .hw_version = 1,
                .serial_number = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C},
            },
            .sensor_values = {.data = 0x11, .int_flag = 0x22, .id = 0x33, .ac_data = 0x44},
        },
        .p_check_in = &spotIn, // Simulate SPOT_CHECK_IN pull up input from previous bet spot
    },
    {
        .usart_obj =
        {
            .bsc_usart_obj = NULL,
            .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE1_DATA_STR) - 1, .from_addr = SLAVES_ADDRESS_START + 1, .op = BS_OP_SET_TEST, .data = SLAVE1_DATA_STR FAKE_CRC},
        },
        .state = SLAVE_STATE_INIT,
        .bs_object =
        {
            .registry = {
                .address = {.location = {.port = 0, .id = 2}}, .led_count = 3, .hw_version = 1,
                .serial_number = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C},
            },
            .sensor_values = {.data = 0x55, .int_flag = 0x66, .id = 0x77, .ac_data = 0x88},
        },
        .p_check_in = &slave_objs[0].check_out, // Simulate SPOT_CHECK_IN pull up input from previous bet spot
    }
};

// *****************************************************************************
void SLAVE_Set_Test(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *rsp = &p_slave_obj->usart_obj.tx_buffer;
    rsp->data[0]++;
    print_rx_buffer(&p_slave_obj->usart_obj);
    begin_write(&p_slave_obj->usart_obj);
}

void SLAVE_Reset_Address(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to reset the address of the slave
    // The implementation will depend on the specific requirements of the application
    printu("Resetting address for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    p_slave_obj->usart_obj.bsc_usart_obj->address = 0; // Reset the address to 0
    p_slave_obj->bs_object.registry.address.addr = 0; // Reset the address to 0
    p_slave_obj->check_out = false; // Reset the check out status
}

void SLAVE_Set_Address(SLAVE_OBJ *p_slave_obj)
{
    bs_set_address_t *msg_set_address = (bs_set_address_t *)&p_slave_obj->usart_obj.rx_buffer.data;
    bs_registry_entry_t *registry = &p_slave_obj->bs_object.registry;
    MY_USART_OBJ *p_usart_obj = &p_slave_obj->usart_obj;

    // This function is called to set the address of the slave
    // The implementation will depend on the specific requirements of the application
    if (memcmp(msg_set_address->serial_number, registry->serial_number, sizeof(registry->serial_number)) == 0)
    {
        printu("Setting Slave address to %02X\n", msg_set_address->addr);
        p_slave_obj->bs_object.registry.address.addr = msg_set_address->addr;

        p_usart_obj->bsc_usart_obj->address = msg_set_address->addr; // Set the address in the USART object
        build_packet(&p_usart_obj->tx_buffer, BS_OP_SET_ADDRESS, MASTER_ADDRESS, p_usart_obj->bsc_usart_obj->address, NULL, 0);
        begin_write(p_usart_obj); // Begin writing the response to the USART
        //now set your output to high to simulate SPOT_CHECK_OUT now next guy can respond
        p_slave_obj->check_out = true; // Set the check out status to true
    }
}

void SLAVE_Set_Sensor_Parameters(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to set sensor parameters for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting sensor parameters for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    // Set sensor parameters logic here, if applicable
}
void SLAVE_Set_Sensor_Mode(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to set the sensor mode for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting sensor mode for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    // Set sensor mode logic here, if applicable
}
void SLAVE_Set_LED_Colors(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to set LED colors for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Setting LED colors for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    // Set LED colors logic here, if applicable
}
// *****************************************************************************
void SLAVE_Get_Registry(SLAVE_OBJ *p_slave_obj)
{
    uint8_t my_addr = p_slave_obj->bs_object.registry.address.addr;
    uint8_t req_addr = p_slave_obj->usart_obj.rx_buffer.to_addr;

    if (req_addr == GLOBAL_ADDRESS)
    {
        // Add random delay to reduce collision
        int delay_ms = 10 + rand() % 50; // 10-59 ms
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        printu("Getting registry for slave %02X (global, delayed %d ms)\n", my_addr, delay_ms);
    }
    else
    {
        printu("Getting registry for slave %02X (direct)\n", my_addr);
    }

    build_packet(&p_slave_obj->usart_obj.tx_buffer, BS_OP_GET_REGISTRY, MASTER_ADDRESS, my_addr, (char *)&p_slave_obj->bs_object.registry, sizeof(bs_registry_entry_t));
    begin_write(&p_slave_obj->usart_obj);
}

void SLAVE_Get_Sensor_Values(SLAVE_OBJ *p_slave_obj)
{
    uint8_t my_addr = p_slave_obj->bs_object.registry.address.addr;
    uint8_t req_addr = p_slave_obj->usart_obj.rx_buffer.to_addr;

    if (req_addr != GLOBAL_ADDRESS)
    {
        printu("Getting sensor values for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    build_packet(&p_slave_obj->usart_obj.tx_buffer, BS_OP_GET_SENSOR_VALUES, MASTER_ADDRESS, my_addr, (char *)&p_slave_obj->bs_object.sensor_values, sizeof(sensor_values_t));
    begin_write(&p_slave_obj->usart_obj);
    }
}
void SLAVE_Get_Sensor_State(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to get the sensor state for the slave
    // The implementation will depend on the specific requirements of the application
    printu("Getting sensor state for slave %02X\n", p_slave_obj->usart_obj.rx_buffer.to_addr);

    // Prepare the response message
}



// *****************************************************************************
void SLAVE_Block_Read(SLAVE_OBJ *p_slave_obj)
{
    // This function is called to read data from the USART
    // It waits for the RX semaphore to be available before proceeding)
    while (xSemaphoreTake(p_slave_obj->usart_obj.bsc_usart_obj->rx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for RX semaphore

    USART_ERROR error = BSC_USART_ErrorGet(p_slave_obj->usart_obj.bsc_usart_obj); // Clear any errors
    if (error == USART_ERROR_NONE)
    {
        // Process the received data
        BS_MESSAGE_BUFFER *msg = &p_slave_obj->usart_obj.rx_buffer;

        switch (msg->op)
        {
        case BS_OP_SET_TEST:
            SLAVE_Set_Test(p_slave_obj);
            break;
        case BS_OP_RESET_ADDRESS:
            SLAVE_Reset_Address(p_slave_obj);
            break;
        case BS_OP_SET_ADDRESS:
            SLAVE_Set_Address(p_slave_obj);
            break;
        case BS_OP_SET_SENSOR_PARAMETERS:
            SLAVE_Set_Sensor_Parameters(p_slave_obj);
            break;
        case BS_OP_SET_SENSOR_MODE:
            SLAVE_Set_Sensor_Mode(p_slave_obj);
            break;
        case BS_OP_SET_LED_COLORS:
            SLAVE_Set_LED_Colors(p_slave_obj);
            break;
        case BS_OP_GET_REGISTRY:
            SLAVE_Get_Registry(p_slave_obj);
            break;
        case BS_OP_GET_SENSOR_VALUES:
            SLAVE_Get_Sensor_Values(p_slave_obj);
            break;
        case BS_OP_GET_SENSOR_STATE:
            SLAVE_Get_Sensor_State(p_slave_obj);
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

        SLAVE_OBJ *p_slave_obj = &slave_objs[i];

        xTaskCreate((TaskFunction_t)SLAVE_Tasks, slave_name, 1024, p_slave_obj, tskIDLE_PRIORITY + 2, &p_slave_obj->xTaskHandle);

        MY_USART_OBJ *p_usart_obj = &p_slave_obj->usart_obj;

        p_usart_obj->bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM4 + i, (uint8_t)(SLAVES_ADDRESS_START + i));

        BSC_USART_WriteCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)tx_callback, (uintptr_t)p_usart_obj);
        BSC_USART_ReadCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)p_usart_obj);
    }
}
// *****************************************************************************
void SLAVE_Tasks(SLAVE_OBJ *p_slave_obj)
{
    MY_USART_OBJ *p_usart_obj = &p_slave_obj->usart_obj;
    while (true)
    {
        /* Check the application's current state. */
        switch (p_slave_obj->state)
        {
        /* Application's initial state. */
        case SLAVE_STATE_INIT:
            // wait for a message from the master
            begin_read(p_usart_obj);
            SLAVE_Block_Read(p_slave_obj);
            break;
        case SLAVE_STATE_WAITING_FOR_MESSAGE:
            break;
        case SLAVE_STATE_TRANSMITTING_RESPONSE:
            break;
        default:
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
