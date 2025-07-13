#include "master.h"
#include "sys_tasks.h"
#include "slave.h"
#include "bsc_protocol.h"

// *****************************************************************************
// Data Structures
// *****************************************************************************
QueueHandle_t master_message_queue;
QueueHandle_t master_response_queue;


bs_object_t bs_objects[NUMBER_OF_SLAVES] =
{
    {
    },
    {
    },
};
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
void print_tx_buffer(const MY_USART_OBJ *p_usart_obj)
{
    printu("TX:%s %02X->%02X:%s = ", pcTaskGetCurrentTaskName(),
           p_usart_obj->tx_buffer.from_addr,
           p_usart_obj->tx_buffer.to_addr,
           bs_op_to_string(p_usart_obj->tx_buffer.op));
    print_hex_data(p_usart_obj->tx_buffer.data, p_usart_obj->tx_buffer.data_len);
}

// *****************************************************************************
void print_rx_buffer(const MY_USART_OBJ *p_usart_obj)
{
    printu("RX:%s %02X->%02X:%02X = ", pcTaskGetCurrentTaskName(),
           p_usart_obj->rx_buffer.from_addr,
           p_usart_obj->rx_buffer.to_addr,
           p_usart_obj->rx_buffer.op);
    print_hex_data(p_usart_obj->rx_buffer.data, p_usart_obj->rx_buffer.data_len);
}

// *****************************************************************************
void begin_read(MY_USART_OBJ *p_usart_obj)
{
    BSC_USART_Read(p_usart_obj->bsc_usart_obj, &p_usart_obj->rx_buffer, sizeof(p_usart_obj->rx_buffer));
}

// *****************************************************************************
void begin_write(MY_USART_OBJ *p_usart_obj)
{
    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_META_SIZE);
    print_tx_buffer(p_usart_obj);
}

// *****************************************************************************
USART_ERROR MASTER_Block_Read(MY_USART_OBJ *p_usart_obj)
{
    while (xSemaphoreTake(p_usart_obj->bsc_usart_obj->rx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for RX semaphore

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error != USART_ERROR_NONE)
    {
        printu("%s, RD Error: %d\n", pcTaskGetCurrentTaskName(), error);
    }
    return error;
}

// *****************************************************************************
void tx_callback(MY_USART_OBJ *p_usart_obj)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(p_usart_obj->bsc_usart_obj->tx_semaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// *****************************************************************************
void rx_callback(MY_USART_OBJ *p_usart_obj)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(p_usart_obj->bsc_usart_obj->rx_semaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// *****************************************************************************
void build_packet(BS_MESSAGE_BUFFER *tx_buffer, BS_OP_t op, uint8_t to_addr, uint8_t from_addr, char *data, uint8_t data_len)
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
void MASTER_Nack(BSC_OP_t command, int index)
{
    // This function is called to send a NACK response for a specific command
    // The implementation will depend on the specific requirements of the application
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_NACK,
        .count = 0, // No data to send
    };

    bsc_get_nack_t *nack = &response.get.nack;
    nack->command = command; // Set the command that caused the NACK
    nack->index = (uint8_t)index; // Set the index of the bet spot that caused the NACK

    xQueueSend(master_response_queue, &response, portMAX_DELAY); // Send the NACK response to the master response queue
}

// *****************************************************************************
bs_object_t *BS_Object_Get_Valid(BSC_OP_t command, int index)
{
    if (index < 0 || index >= bs_count)
    {
        MASTER_Nack(command, index); // Send a NACK response for invalid index
        return NULL; // Invalid address
    }
    return &bs_objects[index]; // Return the corresponding bet spot object
}

// *****************************************************************************
bs_object_t *BS_Object_Get(int index)
{
    assert(index >= 0 && index < NUMBER_OF_SLAVES); // Ensure index is within bounds
    return &bs_objects[index]; // Return the corresponding bet spot object
}

// *****************************************************************************
void MASTER_Reset_Registry(MASTER_DATA *master_data)
{
    MY_USART_OBJ *p_usart_obj = &master_data->usart_obj;
    printu("Resetting registry for all bet spots\n");

    build_packet(&p_usart_obj->tx_buffer, BS_OP_RESET_ADDRESS, GLOBAL_ADDRESS, MASTER_ADDRESS, NULL, 0);
    begin_write(&masterData.usart_obj); // Send the reset address command to the bet spot
    while (xSemaphoreTake(masterData.usart_obj.bsc_usart_obj->tx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for TX semaphore, no slaves should respond

    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        bs_object_t *bs_object = BS_Object_Get(i);

        build_packet(&p_usart_obj->tx_buffer, BS_OP_GET_REGISTRY, GLOBAL_ADDRESS, MASTER_ADDRESS, NULL, 0);

        bsc_get_registry_t *rsp_registry = (bsc_get_registry_t *) &p_usart_obj->rx_buffer.data;
        begin_read(p_usart_obj);
        vTaskDelay(1); // Delay to allow the bet spot to process the command

        begin_write(&masterData.usart_obj);//send get registry command to the bet spot

        if ((MASTER_Block_Read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_usart_obj->rx_buffer.op == BS_OP_GET_REGISTRY) &&
                (p_usart_obj->rx_buffer.to_addr == MASTER_ADDRESS) &&
                (p_usart_obj->rx_buffer.data_len == sizeof(bs_registry_entry_t)))
        {

            bs_set_address_t msg_set_address;
            msg_set_address.addr = (uint8_t)(i + SLAVES_ADDRESS_START);
            memcpy(msg_set_address.serial_number, rsp_registry->serial_number, sizeof(msg_set_address.serial_number));

            bsc_get_registry_t registry_data;
            memcpy(&registry_data, rsp_registry, sizeof(bsc_get_registry_t));

            build_packet(&p_usart_obj->tx_buffer, BS_OP_SET_ADDRESS, GLOBAL_ADDRESS, MASTER_ADDRESS, (char *)&msg_set_address, sizeof(msg_set_address));

            begin_read(p_usart_obj);
            vTaskDelay(1); // Delay to allow the bet spot to process the command
            begin_write(&masterData.usart_obj);

            if ((MASTER_Block_Read(p_usart_obj) == USART_ERROR_NONE) &&
                    (p_usart_obj->rx_buffer.op == BS_OP_SET_ADDRESS) &&
                    (p_usart_obj->rx_buffer.to_addr == MASTER_ADDRESS) &&
                    (p_usart_obj->rx_buffer.data_len == 0))
            {
                registry_data.address.addr = msg_set_address.addr;
                bs_object->registry = registry_data;
                printu("Bet spot %d address set to %02X\n", i, bs_object->registry.address.addr);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1)); // Delay to allow the bet spot to process the command
    }
    bs_count = 0;
}

// *****************************************************************************
void MASTER_Set_Sensor_Parameters(bsc_multicast_set_messages_t *set)
{
    printu("Setting sensor parameters for %d bet spots\n", set->count);

    for (int i = 0; i < set->count; i++)
    {
        bsc_set_sensor_parameters_t *params = &set->set.sensor_parameter[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, params->index);

        if ((bs_object == NULL) && (params->mode < BSC_SENSOR_MODE_MAX))
        {
            bs_object->sensor_parameters[params->mode] = params->parameters;
            printu(" Index: %d, Mode: %d, Conf1: %02X, Conf2: %02X, Conf3: %02X, Thdl: %02X, Thdh: %02X, Canc: %02X, Conf4: %02X\n",
                   params->index,
                   params->mode,
                   params->parameters.conf1,
                   params->parameters.conf2,
                   params->parameters.conf3,
                   params->parameters.thdl,
                   params->parameters.thdh,
                   params->parameters.canc,
                   params->parameters.conf4);
        }
        else
        {
            MASTER_Nack(set->command, params->index);
            break;
        }
    }
}

// *****************************************************************************
void MASTER_Set_Sensor_Mode(bsc_multicast_set_messages_t *set)
{
    // This function is called to set sensor mode for all bet spots
    // The implementation will depend on the specific requirements of the application
    printu("Setting sensor mode for %d bet spots\n", set->count);

    for (int i = 0; i < set->count; i++)
    {
        bsc_set_sensor_mode_t *mode = &set->set.sensor_mode[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, mode->index);

        if (bs_object == NULL)
        {
            break;
        }

        bs_object->sensor_mode = mode->mode; // Set the sensor mode for the bet spot

        printu(" Index: %d, Mode: %d\n",
               mode->index, mode->mode);

        // Here you would typically set the mode for the corresponding bet spot
        // For now, we just print the mode
    }
}

// *****************************************************************************
void print_color(color_t *color)
{
    printu("Color: R:%02X G:%02X B:%02X\n", color->r, color->g, color->b);
}
// *****************************************************************************
void MASTER_Set_LED_Colors(bsc_multicast_set_messages_t *set)
{
    printu("Setting LED colors for %d bet spots\n", set->count);

    for (int i = 0; i < set->count; i++)
    {
        bsc_set_led_colors_t *led_colors = &set->set.led_colors[i];
        bs_object_t *bs_object = BS_Object_Get_Valid(set->command, led_colors->index);

        if (bs_object == NULL)
        {
            break;
        }
        printu(" Index: %d, ", led_colors->index);
        print_color(&led_colors->colors);

        for (int j = 0; j < bs_object->registry.led_count; j++)
        {
            bs_object->colors[j] = led_colors->colors;
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
void MASTER_Get_Sensor_Values(MASTER_DATA *master_data)
{
    // This function is called to get sensor values for all bet spots
    // The implementation will depend on the specific requirements of the application
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_GET_SENSOR_VALUES,
        .count = (uint8_t)bs_count,
    };

    MY_USART_OBJ *p_usart_obj = &master_data->usart_obj;

    for (int i = 0; i < bs_count; i++)
    {
        bs_object_t *bs_object = BS_Object_Get(i);
        uint8_t slave_addr = bs_object->registry.address.addr;

        build_packet(&masterData.usart_obj.tx_buffer, BS_OP_GET_SENSOR_VALUES, GLOBAL_ADDRESS, slave_addr, NULL, 0);
        begin_read(p_usart_obj);
        vTaskDelay(1);
        begin_write(&masterData.usart_obj);

        if ((MASTER_Block_Read(p_usart_obj) == USART_ERROR_NONE) &&
                (p_usart_obj->rx_buffer.op == BS_OP_GET_SENSOR_VALUES) &&
                (p_usart_obj->rx_buffer.to_addr == MASTER_ADDRESS) &&
                (p_usart_obj->rx_buffer.from_addr == slave_addr) &&
                (p_usart_obj->rx_buffer.data_len == sizeof(sensor_values_t)))
        {
            bsc_get_get_sensor_values_t *sensor_values = &response.get.sensor_values[i];

            bs_object->sensor_values = *(sensor_values_t *)p_usart_obj->rx_buffer.data;

            sensor_values->index = (uint8_t)i;
            sensor_values->data = bs_object->sensor_values.data;
            sensor_values->int_flag = bs_object->sensor_values.int_flag;
            sensor_values->id = bs_object->sensor_values.id;
            sensor_values->ac_data = bs_object->sensor_values.ac_data;
        }
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
void MASTER_Get_Sensor_State()
{
    bsc_multicast_get_messages_t response =
    {
        .command = BSC_OP_GET_SENSOR_STATE,
        .count = (uint8_t)bs_count,
    };

    for (int i = 0; i < bs_count; i++)
    {
        bsc_get_sensor_state_t *sensor_state = &response.get.sensor_state[i];
        bs_object_t *bs_object = BS_Object_Get(i);

        sensor_state->mode = bs_object->sensor_mode;
        sensor_state->state = bs_object->sensor_state;
        sensor_state++;
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);

}


// *****************************************************************************
void MASTER_Com_Test(MASTER_DATA *master_data, bsc_multicast_set_messages_t *set)
{
    MY_USART_OBJ *p_usart_obj = &master_data->usart_obj;
    static char master_string[] = MASTER_DATA_STR;
    int count = *(uint32_t *)(&set->count); // Get the count from the set message

    for (int i = 0; i < count; i++)
    {
        printu("  \nTransaction %02X\n", master_string[0]);

        build_packet(&p_usart_obj->tx_buffer, BS_OP_SET_TEST, GLOBAL_ADDRESS, MASTER_ADDRESS, master_string, sizeof(master_string) - 1);

        for (int slave_address = SLAVES_ADDRESS_START; slave_address < SLAVES_ADDRESS_START + NUMBER_OF_SLAVES; slave_address++)
        {
            printu("M->S%d\n", slave_address);
            p_usart_obj->tx_buffer.to_addr = (uint8_t)slave_address;

            // message from host to slaves
            begin_read(p_usart_obj);
            begin_write(p_usart_obj);

            // response from slave to host
            MASTER_Block_Read(p_usart_obj);
        }

        master_string[0]++;

        //vTaskDelay(pdMS_TO_TICKS(10));
        LED_GREEN_Toggle();
    }
}

// *****************************************************************************
void MASTER_Initialize(void)
{
    xTaskCreate((TaskFunction_t)MASTER_Tasks, "MASTER_Task", 1024, &masterData, tskIDLE_PRIORITY + 2, &masterData.xTaskHandle);

    MY_USART_OBJ *p_usart_obj = &masterData.usart_obj;

    p_usart_obj->bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM1, MASTER_ADDRESS);

    BSC_USART_WriteCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)tx_callback, (uintptr_t)p_usart_obj);
    BSC_USART_ReadCallbackRegister(p_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)p_usart_obj);

    master_message_queue = xQueueCreate(2, sizeof(bsc_multicast_set_messages_t));
    master_response_queue = xQueueCreate(2, sizeof(bsc_multicast_get_messages_t));
}

// *****************************************************************************
void MASTER_Tasks(MASTER_DATA *master_data)
{
    bsc_multicast_set_messages_t set;

    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize

    while (true)
    {
        switch (master_data->state)
        {
        case MASTER_STATE_INIT:
            master_data->state = MASTER_STATE_SERVICE_TASKS;
            break;
        case MASTER_STATE_SERVICE_TASKS:
            if (xQueueReceive(master_message_queue, &set, portMAX_DELAY) == pdPASS)
            {
                switch (set.command)
                {
                case BSC_OP_RESET_REGISTRY:
                    MASTER_Reset_Registry(master_data);
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
                    MASTER_Get_Sensor_Values(master_data);
                    break;
                case BSC_OP_GET_SENSOR_STATE:
                    MASTER_Get_Sensor_State();
                    break;
                case BSC_OP_SET_TEST:
                    MASTER_Com_Test(master_data, &set);
                    break;
                default:
                    MASTER_Nack(set.command, -1); // Send NACK for unknown command
                    break;
                }
            }
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