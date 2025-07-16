#include "slave.h"

// *****************************************************************************
void SLAVE_Tasks(SLAVE_OBJ *slave_data);

bool spotIn = true; // Simulate SPOT_CHECK_IN pull up input to first position bet spot

// *****************************************************************************
SLAVE_OBJ slave_objs[NUMBER_OF_SLAVES] =
{
    {
        .registry = {
            .address = {.location = {.port = 0, .id = 1}},
            .led_count = 3,
            .hw_version = 1,
            .serial_number = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C},
        },
        .sensor_values = {.data = 0x11, .int_flag = 0x22, .id = 0x33, .ac_data = 0x44},
        .sensor_state = BSC_SENSOR_STATE_HAND,
        .p_check_in = &spotIn, // Simulate SPOT_CHECK_IN pull up input from previous bet spot
    },
    {
        .registry = {
            .address = {.location = {.port = 0, .id = 2}}, .led_count = 3, .hw_version = 1,
            .serial_number = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C},
        },
        .sensor_values = {.data = 0x55, .int_flag = 0x66, .id = 0x77, .ac_data = 0x88},
        .sensor_state = BSC_SENSOR_STATE_CHIP,
        .p_check_in = &slave_objs[0].check_out, // Simulate SPOT_CHECK_IN pull up input from previous bet spot
    }
};

// *****************************************************************************
void SLAVE_Set_Test(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;

    uint8_t my_addr = p_slave_obj->registry.address.addr;
    p_tx_buffer->data[0] = p_slave_obj->test_count;
    for (int i = 1; i < SIZE_OF_TEST_DATA; i++)
    {
        p_tx_buffer->data[i] = p_rx_buffer->data[i] + my_addr;
    }
    build_packet(p_tx_buffer, BS_OP_SET_TEST, MASTER_ADDRESS, my_addr, p_tx_buffer->data, SIZE_OF_TEST_DATA);
    begin_write(p_usart_obj, p_tx_buffer);
    p_slave_obj->test_count++;
}

// *****************************************************************************
void SLAVE_Reset_Address(SLAVE_OBJ *p_slave_obj)
{
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    bs_registry_entry_t *p_registry = &p_slave_obj->registry;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    printu("  Resetting address for slave %02X sn:", my_addr);
    print_hex_data(p_registry->serial_number, SIZEOF_SERIAL_NUMBER);
    p_usart_obj->addr = 0;
    p_registry->address.addr = 0;
    p_slave_obj->check_out = false;
}

// *****************************************************************************
void SLAVE_Set_Address(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;

    bs_set_address_t *msg_set_address = (bs_set_address_t *)p_rx_buffer->data;
    bs_registry_entry_t *registry = &p_slave_obj->registry;
    if (memcmp(msg_set_address->serial_number, registry->serial_number, sizeof(registry->serial_number)) == 0)
    {
        printu("  Setting Slave address to %02X\n", msg_set_address->addr);
        p_slave_obj->registry.address.addr = msg_set_address->addr;
        p_usart_obj->addr = msg_set_address->addr;
        build_packet(p_tx_buffer, BS_OP_SET_ADDRESS, MASTER_ADDRESS, p_usart_obj->addr, NULL, 0);
        begin_write(p_usart_obj, p_tx_buffer);
        p_slave_obj->check_out = true; // Set the check out status to true
    }
}

// *****************************************************************************
void SLAVE_Set_Sensor_Parameters(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    bs_set_sensor_parameters_t *p_set_params = (bs_set_sensor_parameters_t *)p_rx_buffer->data;
    bsc_sensor_mode_t mode = p_set_params->mode;
    if (mode < BSC_SENSOR_MODE_MAX)
    {
        p_slave_obj->sensor_parameters[mode] = p_set_params->parameters; // Set the sensor parameters for the specified mode
        printu("  Setting sensor mode %d parameters for slave %02X: ", mode, my_addr);
        print_hex_data((uint8_t *)&p_slave_obj->sensor_parameters[mode], sizeof(sensor_parameters_t));
        build_packet(p_tx_buffer, BS_OP_SET_SENSOR_PARAMETERS, MASTER_ADDRESS, my_addr, NULL, 0);
        begin_write(p_usart_obj, p_tx_buffer);
    }
}

// *****************************************************************************
void SLAVE_Set_Sensor_Mode(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    bsc_sensor_mode_t *p_mode = (bsc_sensor_mode_t *)p_rx_buffer->data;
    bsc_sensor_mode_t mode = *p_mode;
    if (mode < BSC_SENSOR_MODE_MAX)
    {
        printu("  Setting sensor mode %d for slave %02X: ", mode, my_addr);
        p_slave_obj->sensor_mode = mode; // Set the sensor parameters for the specified mode
        build_packet(p_tx_buffer, BS_OP_SET_SENSOR_MODE, MASTER_ADDRESS, my_addr, NULL, 0);
        begin_write(p_usart_obj, p_tx_buffer);
    }
}

// *****************************************************************************
void SLAVE_Set_LED_Colors(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    color_t *p_color = (color_t *)p_rx_buffer->data;
    printu("  Setting color for slave %02X: ", my_addr);
    print_color(p_color);
    for (int i = 0; i < p_slave_obj->registry.led_count; i++)
    {
        p_slave_obj->colors[i] = *p_color; // Set the same color for each LED
    }
    build_packet(p_tx_buffer, BS_OP_SET_LED_COLORS, MASTER_ADDRESS, my_addr, NULL, 0);
    begin_write(p_usart_obj, p_tx_buffer);

}

// *****************************************************************************
void SLAVE_Get_Registry(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;
    uint8_t req_addr = p_rx_buffer->to_addr;

    if (req_addr == GLOBAL_ADDRESS)
    {
        if ((my_addr != GLOBAL_ADDRESS) || (*p_slave_obj->p_check_in == false))
        {
            return;
        }
        int delay_ms = 10 + rand() % 50;
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        printu("  Getting registry for slave %02X (global, delayed %d ms)\n", my_addr, delay_ms);
    }
    else if (req_addr != my_addr)
    {
        return;
    }
    else
    {
        printu("  Getting registry for slave %02X (direct)\n", my_addr);
    }
    build_packet(p_tx_buffer,
                 BS_OP_GET_REGISTRY,
                 MASTER_ADDRESS,
                 my_addr,
                 (uint8_t *)&p_slave_obj->registry,
                 sizeof(bs_registry_entry_t));
    begin_write(p_usart_obj, p_tx_buffer);
}

// *****************************************************************************
void SLAVE_Get_Sensor_Values(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    printu("  Getting sensor values for slave %02X\n", my_addr);
    build_packet(p_tx_buffer, BS_OP_GET_SENSOR_VALUES, MASTER_ADDRESS, my_addr, (uint8_t *)&p_slave_obj->sensor_values, sizeof(sensor_values_t));
    begin_write(p_usart_obj, p_tx_buffer);
}

// *****************************************************************************
void SLAVE_Get_Sensor_State(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &p_slave_obj->tx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    uint8_t my_addr = p_slave_obj->registry.address.addr;

    printu("  Getting sensor state for slave %02X\n", my_addr);
    build_packet(p_tx_buffer, BS_OP_GET_SENSOR_STATE, MASTER_ADDRESS, my_addr, (uint8_t *)&p_slave_obj->sensor_state, sizeof(bsc_sensor_state_t));
    begin_write(p_usart_obj, p_tx_buffer);
}

// *****************************************************************************
void SLAVE_Block_Read(SLAVE_OBJ *p_slave_obj)
{
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;

    // This function is called to read data from the USART
    // It waits for the RX semaphore to be available before proceeding)
    USART_ERROR error = block_read(p_usart_obj); // Clear any errors
    if (error == USART_ERROR_NONE)
    {
        uint8_t req_addr = p_rx_buffer->to_addr;

        if (req_addr == GLOBAL_ADDRESS)
        {
            switch (p_rx_buffer->op)
            {
            case BS_OP_RESET_ADDRESS:
                SLAVE_Reset_Address(p_slave_obj);
                break;
            case BS_OP_SET_ADDRESS:
                SLAVE_Set_Address(p_slave_obj);
                break;
            case BS_OP_GET_REGISTRY:
                SLAVE_Get_Registry(p_slave_obj);
                break;
            default:
                printu("  Unknown operation received: %d\n", p_rx_buffer->op);
                break;
            }
        }
        else
        {
            switch (p_rx_buffer->op)
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
                printu("  Unknown operation received: %d\n", p_rx_buffer->op);
                break;
            }
        }
    }
}

// *****************************************************************************
void SLAVE_Initialize(void)
{
    char slave_name[] = "SLAVE#_Task";
    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        slave_name[5] = (char)('0' + i);

        SLAVE_OBJ *p_slave_obj = &slave_objs[i];

        xTaskCreate((TaskFunction_t)SLAVE_Tasks, slave_name, 1024, p_slave_obj, tskIDLE_PRIORITY + 2, &p_slave_obj->xTaskHandle);

        p_slave_obj->p_usart_obj = BSC_USART_Initialize(SLAVE_SERCOM_ID_START + i, (uint8_t)(SLAVES_ADDRESS_START + i)); // Initialize USART for each slave
        BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
        BSC_USART_WriteCallbackRegister(p_usart_obj, (SERCOM_USART_CALLBACK)tx_callback, (uintptr_t)p_usart_obj);
        BSC_USART_ReadCallbackRegister(p_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)p_usart_obj);
    }
}

// *****************************************************************************
void SLAVE_Tasks(SLAVE_OBJ *p_slave_obj)
{
    BSC_USART_OBJECT *p_usart_obj = p_slave_obj->p_usart_obj;
    BS_MESSAGE_BUFFER *p_rx_buffer = &p_slave_obj->rx_buffer;

    while (true)
    {
        /* Check the application's current state. */
        switch (p_slave_obj->state)
        {
        /* Application's initial state. */
        case SLAVE_STATE_INIT:
            // wait for a message from the master
            begin_read(p_usart_obj, p_rx_buffer);
            p_slave_obj->state = SLAVE_STATE_WAITING_FOR_MESSAGE;
            break;
        case SLAVE_STATE_WAITING_FOR_MESSAGE:
            SLAVE_Block_Read(p_slave_obj);
            p_slave_obj->state = SLAVE_STATE_INIT; // Reset state to wait for the next message
            break;
        case SLAVE_STATE_TRANSMITTING_RESPONSE:
            break;
        default:
            break;
        }
    }
}
