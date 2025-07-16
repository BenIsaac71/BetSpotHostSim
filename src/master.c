#include "master.h"
#include "sys_tasks.h"
#include "slave.h"
#include "bsc_protocol.h"

// *****************************************************************************
// Data Structures
// *****************************************************************************
QueueHandle_t master_message_queue;
QueueHandle_t master_response_queue;

bs_object_t bs_objects[NUMBER_OF_SLAVES];

int bs_count = NUMBER_OF_SLAVES; // Number of bet spots initialized

MASTER_DATA masterData =
{
    .state = MASTER_STATE_INIT,
};

// *****************************************************************************
// Function Prototypes
// *****************************************************************************
void MASTER_Tasks(MASTER_DATA *pMasterData);

// *****************************************************************************
// BSC_USART Functions
// *****************************************************************************
char *pcTaskGetCurrentTaskName(void)
{
    return pcTaskGetName(xTaskGetCurrentTaskHandle());
}

// *****************************************************************************
void print_hex_data(const void *data, size_t size)
{
#define MAX_HEX_PRINT 16
    const uint8_t *byte_data = (const uint8_t *)data;
    for (size_t i = 0; i < size; i++)
    {
        if ((i % MAX_HEX_PRINT == MAX_HEX_PRINT - 1) || (i == size - 1))
        {
            printu("%02X\n", byte_data[i]);
        }
        else
        {
            printu("%02X, ", byte_data[i]);
        }
    }
    if (size == 0)
    {
        printu("\n");
    }
}

// *****************************************************************************
void print_color(color_t *color)
{
    printu("Color: R:%02X G:%02X B:%02X\n", color->r, color->g, color->b);
}

// *****************************************************************************
char *bs_op_to_string(BS_OP_t op)
{
    switch (op)
    {
    case BS_OP_SET_TEST:
        return "SET_TEST";
    case BS_OP_RESET_ADDRESS:
        return "RESET_ADDRESS";
    case BS_OP_SET_ADDRESS:
        return "SET_ADDRESS";
    case BS_OP_SET_SENSOR_PARAMETERS:
        return "SET_SENSOR_PARAMETERS";
    case BS_OP_SET_SENSOR_MODE:
        return "SET_SENSOR_MODE";
    case BS_OP_SET_LED_COLORS:
        return "SET_LED_COLORS";
    case BS_OP_GET_REGISTRY:
        return "GET_REGISTRY";
    case BS_OP_GET_SENSOR_VALUES:
        return "GET_SENSOR_VALUES";
    case BS_OP_GET_SENSOR_STATE:
        return "GET_SENSOR_STATE";
    default:
        return "UNKNOWN";
    }
}

// *****************************************************************************
void print_tx_buffer(BS_MESSAGE_BUFFER *p_tx_buf)
{
    printu(" TX:%s %02X->%02X:%s = ", pcTaskGetCurrentTaskName(),
           p_tx_buf->from_addr,
           p_tx_buf->to_addr,
           bs_op_to_string(p_tx_buf->op));
    print_hex_data(p_tx_buf->data, p_tx_buf->data_len);
}

// *****************************************************************************
void print_rx_buffer(BS_MESSAGE_BUFFER *p_rx_buf)
{
    printu(" RX:%s %02X->%02X:%s = ", pcTaskGetCurrentTaskName(),
           p_rx_buf->from_addr,
           p_rx_buf->to_addr,
           bs_op_to_string(p_rx_buf->op));
    print_hex_data(p_rx_buf->data, p_rx_buf->data_len);
}

// *****************************************************************************
void begin_read(BSC_USART_OBJECT *p_usart_obj, BS_MESSAGE_BUFFER *p_rx_buf)
{
    BSC_USART_Read(p_usart_obj, p_rx_buf, sizeof(BS_MESSAGE_BUFFER));
}

// *****************************************************************************
void begin_write(BSC_USART_OBJECT *p_usart_obj, BS_MESSAGE_BUFFER *p_tx_buf)
{
    print_tx_buffer(p_tx_buf);
    BSC_USART_Write(p_usart_obj, p_tx_buf, p_tx_buf->data_len + BS_MESSAGE_META_SIZE);
}

// *****************************************************************************
void block_write_complete(BSC_USART_OBJECT *p_usart_obj)
{
    TP0_Set();
    xSemaphoreTake(p_usart_obj->tx_semaphore, portMAX_DELAY);
    TP0_Clear();
}

// *****************************************************************************
USART_ERROR block_read(BSC_USART_OBJECT *p_usart_obj)
{
    xSemaphoreTake(p_usart_obj->rx_semaphore, portMAX_DELAY);

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj); // Clear any errors
    if (error != USART_ERROR_NONE)
    {
        printu("%s, RD Error: %d\n", pcTaskGetCurrentTaskName(), error);
    }
    else
    {
        print_rx_buffer(p_usart_obj->rxBuffer);
    }
    return error;
}

// *****************************************************************************
void tx_callback(BSC_USART_OBJECT *p_usart_obj)
{
    if(p_usart_obj->bsc_usart_id == BSC_USART_SERCOM1_ID)
    {
        TP3_Toggle();
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(p_usart_obj->tx_semaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// *****************************************************************************
void rx_callback(BSC_USART_OBJECT *p_usart_obj)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(p_usart_obj->rx_semaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// *****************************************************************************
void build_packet(BS_MESSAGE_BUFFER *tx_buffer, BS_OP_t op, uint8_t to_addr, uint8_t from_addr, uint8_t *data, uint8_t data_len)
{
    tx_buffer->to_addr = to_addr;
    tx_buffer->from_addr = from_addr;
    tx_buffer->op = op;
    tx_buffer->data_len = data_len;
    memcpy(tx_buffer->data, data, data_len);
    *(uint32_t *)&tx_buffer->data[data_len] = 0xFF0000FF;
}
// *****************************************************************************
// BSC Object Functions
// *****************************************************************************
void MASTER_Error(BSC_OP_t command, int index, BSC_ERROR_t error)
{
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_ERROR,
        .count = 1, // Only send one error response
        .get.error = {.command = command, .index = (uint8_t)index, .error = error}
    };
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
bs_object_t *BS_Object_Get_Valid(BSC_OP_t command, int index)
{
    if (index < 0 || index >= bs_count)
    {
        MASTER_Error(command, index, BSC_ERROR_INVALID_ADDRESS);
        return NULL;
    }
    return &bs_objects[index];
}

// *****************************************************************************
bs_object_t *BS_Object_Get(int index)
{
    assert(index >= 0 && index < NUMBER_OF_SLAVES);
    return &bs_objects[index];
}

// *****************************************************************************
bool BS_Mode_Valid(BSC_OP_t command, int index, bsc_sensor_mode_t mode)
{
    if (mode < BSC_SENSOR_MODE_MAX)
    {
        return true;
    }
    MASTER_Error(command, index, BSC_ERROR_INVALID_ADDRESS);
    return false;
}

// *****************************************************************************
void MASTER_Reset_Registry(void)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    printu("Resetting registry for all bet spots\n");
    build_packet(p_tx_buffer, BS_OP_RESET_ADDRESS, GLOBAL_ADDRESS, MASTER_ADDRESS, NULL, 0);
    begin_write(p_usart_obj, p_tx_buffer);
    block_write_complete(p_usart_obj);
    bs_count = 0;

    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(5)); // wait a bit for command to complete
        printu("\nWorking on bet spot %d\n", i);
        bs_object_t *bs_object = BS_Object_Get(i);

        // ask bet spot with spot_in high for its registry
        bsc_get_registry_t *p_rsp_registry = (bsc_get_registry_t *) p_rx_buffer->data;
        begin_read(p_usart_obj, p_rx_buffer);
        build_packet(p_tx_buffer, BS_OP_GET_REGISTRY, GLOBAL_ADDRESS, MASTER_ADDRESS, NULL, 0);
        begin_write(p_usart_obj, p_tx_buffer);

        if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_rx_buffer->op == BS_OP_GET_REGISTRY) &&
                (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                (p_rx_buffer->from_addr == GLOBAL_ADDRESS) &&
                (p_rx_buffer->data_len == sizeof(bs_registry_entry_t)))
        {
            // store the registry data for later
            bsc_get_registry_t registry_data = *p_rsp_registry;

            // now tell slave its new address
            bs_set_address_t msg_set_address;
            begin_read(p_usart_obj, p_rx_buffer);
            uint8_t new_address = (uint8_t)(i + SLAVES_ADDRESS_START);
            msg_set_address.addr = new_address;
            memcpy(msg_set_address.serial_number, registry_data.serial_number, sizeof(registry_data.serial_number));
            build_packet(p_tx_buffer, BS_OP_SET_ADDRESS, GLOBAL_ADDRESS, MASTER_ADDRESS, (uint8_t *)&msg_set_address, sizeof(bs_set_address_t));
            begin_write(p_usart_obj, p_tx_buffer);

            if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                    (p_rx_buffer->op == BS_OP_SET_ADDRESS) &&
                    (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                    (p_rx_buffer->from_addr == new_address) &&
                    (p_rx_buffer->data_len == 0))
            {
                // store bet spot in the registry
                printu("Register bet spot address %02X -> registry[%d]\n", new_address, i);
                registry_data.address.addr = new_address;
                bs_object->registry = registry_data;
                bs_count++;
            }
        }

    }
}

// *****************************************************************************
void MASTER_Set_Sensor_Parameters(bsc_multicast_set_messages_t *set)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    printu("Setting sensor parameters for %d bet spots\n", set->count);
    for (int i = 0; i < set->count; i++)
    {
        bsc_set_sensor_parameters_t *p_parms = &set->set.sensor_parameter[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, p_parms->index);

        if ((bs_object != NULL) && !BS_Mode_Valid(set->command, p_parms->index, p_parms->sensor.mode))
        {
            break;
        }

        uint8_t slave_addr = bs_object->registry.address.addr;
        begin_read(p_usart_obj, p_rx_buffer);
        build_packet(p_tx_buffer, BS_OP_SET_SENSOR_PARAMETERS, slave_addr, MASTER_ADDRESS, (uint8_t *)&p_parms->sensor, sizeof(bs_set_sensor_parameters_t));
        begin_write(p_usart_obj, p_tx_buffer); // Send the set address command to the bet spot

        if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_rx_buffer->op == BS_OP_SET_SENSOR_PARAMETERS) &&
                (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                (p_rx_buffer->from_addr == slave_addr) &&
                (p_rx_buffer->data_len == 0))
        {
            printu("Bet spot %d sensor parameters set\n", slave_addr);
        }
    }
}

// *****************************************************************************
void MASTER_Set_Sensor_Mode(bsc_multicast_set_messages_t *set)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    printu("Setting sensor mode for %d bet spots\n", set->count);
    for (int i = 0; i < set->count; i++)
    {
        bsc_set_sensor_mode_t *p_mode = &set->set.sensor_mode[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, p_mode->index);
        if ((bs_object != NULL) && !BS_Mode_Valid(set->command, p_mode->index, p_mode->mode))
        {
            break;
        }

        if (p_mode->mode < BSC_SENSOR_MODE_MAX)
        {
            uint8_t slave_addr = bs_object->registry.address.addr;
            begin_read(p_usart_obj, p_rx_buffer);
            build_packet(p_tx_buffer, BS_OP_SET_SENSOR_MODE, slave_addr, MASTER_ADDRESS, (uint8_t *)&p_mode->mode, sizeof(bsc_sensor_mode_t));
            begin_write(p_usart_obj, p_tx_buffer); // Send the set address command to the bet spot

            if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                    (p_rx_buffer->op == BS_OP_SET_SENSOR_MODE) &&
                    (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                    (p_rx_buffer->from_addr == slave_addr) &&
                    (p_rx_buffer->data_len == 0))
            {
                printu("Bet spot %d sensor mode set\n", slave_addr);
            }
        }
    }
}

// *****************************************************************************
void MASTER_Set_LED_Colors(bsc_multicast_set_messages_t *set)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    printu("Setting LED color for %d bet spots\n", set->count);
    for (int i = 0; i < set->count; i++)
    {
        bsc_set_led_colors_t *p_led_colors = &set->set.led_colors[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, p_led_colors->index);

        if (bs_object == NULL)
        {
            break;
        }
        uint8_t slave_addr = bs_object->registry.address.addr;
        begin_read(p_usart_obj, p_rx_buffer);
        build_packet(p_tx_buffer, BS_OP_SET_LED_COLORS, slave_addr, MASTER_ADDRESS, (uint8_t *)&p_led_colors->color, sizeof(color_t));
        begin_write(p_usart_obj, p_tx_buffer); // Send the set address command to the bet spot

        if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_rx_buffer->op == BS_OP_SET_LED_COLORS) &&
                (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                (p_rx_buffer->from_addr == slave_addr) &&
                (p_rx_buffer->data_len == 0))
        {
            printu("Bet spot %d sensor mode set\n", slave_addr);
        }
    }
}

// *****************************************************************************
void MASTER_Get_Registry()
{
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_GET_REGISTRY,
        .count = (uint8_t)bs_count
    };

    for (int i = 0; i < bs_count; i++)
    {
        response.get.registry[i] = BS_Object_Get(i)->registry;
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
void MASTER_Get_Sensor_Values(void)
{
    // This function is called to get sensor values for all bet spots
    // The implementation will depend on the specific requirements of the application
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_GET_SENSOR_VALUES,
    };

    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    for (int i = 0; i < bs_count; i++)
    {
        uint8_t slave_addr = BS_Object_Get(i)->registry.address.addr;
        build_packet(p_tx_buffer, BS_OP_GET_SENSOR_VALUES, slave_addr, MASTER_ADDRESS, NULL, 0);
        begin_read(p_usart_obj, p_rx_buffer);
        begin_write(p_usart_obj, p_tx_buffer); // Send the get sensor values command to the bet spot

        if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_rx_buffer->op == BS_OP_GET_SENSOR_VALUES) &&
                (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                (p_rx_buffer->from_addr == slave_addr) &&
                (p_rx_buffer->data_len == sizeof(sensor_values_t)))
        {
            bsc_get_get_sensor_values_t *sensor_values = &response.get.sensor_values[i];
            sensor_values->index = (uint8_t)i; // Set the index of the bet spot
            sensor_values->values = *(sensor_values_t *)p_rx_buffer->data;
            response.count++;
        }
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
void MASTER_Get_Sensor_State(void)
{
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_GET_SENSOR_STATE,
        .count = 0
    };

    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    for (int i = 0; i < bs_count; i++)
    {
        uint8_t slave_addr = BS_Object_Get(i)->registry.address.addr;
        build_packet(p_tx_buffer, BS_OP_GET_SENSOR_STATE, slave_addr, MASTER_ADDRESS, NULL, 0);
        begin_read(p_usart_obj, p_rx_buffer);
        begin_write(p_usart_obj, p_tx_buffer); // Send the get sensor values command to the bet spot

        if ((block_read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_rx_buffer->op == BS_OP_GET_SENSOR_STATE) &&
                (p_rx_buffer->to_addr == MASTER_ADDRESS) &&
                (p_rx_buffer->from_addr == slave_addr) &&
                (p_rx_buffer->data_len == sizeof(bsc_sensor_state_t)))
        {
            bsc_get_sensor_state_t *sensor_state = &response.get.sensor_state[i];
            sensor_state->index = (uint8_t)i;
            sensor_state->state = *(bsc_sensor_state_t *)p_rx_buffer->data;
            response.count++;
        }
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}


// *****************************************************************************
void MASTER_Com_Test(bsc_multicast_set_messages_t *set)
{
    BS_MESSAGE_BUFFER *p_tx_buffer = &masterData.tx_buffer;
    BS_MESSAGE_BUFFER *p_rx_buffer = &masterData.rx_buffer;
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;

    static uint8_t test_data[] = MASTER_TEST_DATA;
    int count = *(uint32_t *)(&set->count); // Get the count from the set message

    for (int i = 0; i < count; i++)
    {
        printu("\nTransaction %02X\n", test_data[0]);
        for (int slave_address = SLAVES_ADDRESS_START; slave_address < SLAVES_ADDRESS_START + NUMBER_OF_SLAVES; slave_address++)
        {
            printu("M->S%d\n", slave_address);
            build_packet(p_tx_buffer, BS_OP_SET_TEST, (uint8_t)slave_address, MASTER_ADDRESS, test_data, SIZE_OF_TEST_DATA);
            // message from host to slaves
            begin_read(p_usart_obj, p_rx_buffer);
            begin_write(p_usart_obj, p_tx_buffer);
            block_write_complete(p_usart_obj);
            // response from slave to host
            block_read(p_usart_obj);
        }
        test_data[0]++;
        LED_GREEN_Toggle();
    }
}

// *****************************************************************************
void MASTER_Initialize(void)
{
    xTaskCreate((TaskFunction_t)MASTER_Tasks, "MASTER_Task", 1024, &masterData, tskIDLE_PRIORITY + 2, &masterData.xTaskHandle);

    masterData.p_usart_obj = BSC_USART_Initialize(MASTER_SERCOM_ID, MASTER_ADDRESS);
    BSC_USART_OBJECT *p_usart_obj = masterData.p_usart_obj;
    BSC_USART_WriteCallbackRegister(p_usart_obj, (SERCOM_USART_CALLBACK)tx_callback, (uintptr_t)p_usart_obj);
    BSC_USART_ReadCallbackRegister(p_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)p_usart_obj);

    master_message_queue = xQueueCreate(2, sizeof(bsc_multicast_set_messages_t));
    master_response_queue = xQueueCreate(2, sizeof(bsc_multicast_get_messages_t));
}

// *****************************************************************************
void MASTER_State_ServiceTask(void)
{
    bsc_multicast_set_messages_t set;

    if (xQueueReceive(master_message_queue, &set, portMAX_DELAY) == pdPASS)
    {
        switch (set.command)
        {
        case BSC_OP_RESET_REGISTRY:
            MASTER_Reset_Registry();
            break;
        case BSC_OP_SET_SENSOR_PARAMETERS:
            MASTER_Set_Sensor_Parameters(&set);
            break;
        case BSC_OP_SET_SENSOR_MODE:
            MASTER_Set_Sensor_Mode(&set);
            break;
        case BSC_OP_SET_LED_COLORS:
            MASTER_Set_LED_Colors(&set);
            break;
        case BSC_OP_GET_REGISTRY:
            MASTER_Get_Registry();
            break;
        case BSC_OP_GET_SENSOR_VALUES:
            MASTER_Get_Sensor_Values();
            break;
        case BSC_OP_GET_SENSOR_STATE:
            MASTER_Get_Sensor_State();
            break;
        case BSC_OP_SET_TEST:
            MASTER_Com_Test(&set);
            break;
        default:
            MASTER_Error(set.command, 0, BSC_ERROR_INVALID_OPERATION); // Send NACK for unknown command
            break;
        }
    }
}

// *****************************************************************************
void MASTER_Tasks(MASTER_DATA *master_data)
{
    while (true)
    {
        switch (master_data->state)
        {
        case MASTER_STATE_INIT:
            master_data->state = MASTER_STATE_SERVICE_TASKS;
            break;
        case MASTER_STATE_SERVICE_TASKS:
            MASTER_State_ServiceTask();
            break;
        default:
            break;
        }
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