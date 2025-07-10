#include "master.h"
#include "sys_tasks.h"
#include "slave.h"
#include "bsc_protocol.h"

// *****************************************************************************
// Data Structures
// *****************************************************************************
QueueHandle_t master_message_queue;
QueueHandle_t master_response_queue;
typedef struct
{
    bs_registry_entry_t registry; // Pointer to the registry entry
    color_t colors[MAX_RGB_COLORS]; // RGB colors for the LEDs
    bsc_sensor_state_t sensor_state; // Sensor state for the bet spot
    bsc_sensor_mode_t sensor_mode; // Sensor mode for the bet spot
    sensor_parameters_t sensor_parameters[3]; // Sensor parameters for the bet spot immediate, hand, chip modes
    sensor_values_t sensor_values; // Sensor values for the bet spot
} bs_object_t;

bs_object_t bs_objects[NUMBER_OF_SLAVES] =
{
    {
        .registry = {.address = {.port = 0, .id = 1}, .led_count = 20, .hw_version = 0x01, .serial_number = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C}},
        .sensor_values = {.data = 1, .int_flag = 2, .id = 3, .ac_data = 4} // Initial sensor values
    },
    {
        .registry =     {.address = {.port = 0, .id = 2}, .led_count = 20, .hw_version = 0x01, .serial_number = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C}},
        .sensor_values = {.data = 17, .int_flag = 18, .id = 19, .ac_data = 20} // Initial sensor values
    },
};
int bs_count = NUMBER_OF_SLAVES; // Number of bet spots initialized

MASTER_DATA masterData =
{
    .my_usart_obj =
    {
        .tx_buffer = {.to_addr = GLOBAL_ADDRESS, .data_len = sizeof(MASTER_DATA_STR) - 1,  .from_addr = MASTER_ADDRESS, .data = MASTER_DATA_STR FAKE_CRC}
    },
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
void begin_read(MY_USART_OBJ *p_usart_obj)
{
    BSC_USART_Read(p_usart_obj->bsc_usart_obj, &p_usart_obj->rx_buffer, sizeof(p_usart_obj->rx_buffer));
}

// *****************************************************************************
void begin_write(MY_USART_OBJ *p_usart_obj)
{
    BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->tx_buffer;

    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_META_SIZE);
    printu("TX:%s %02X->%02X[%s]\n", pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);
}

// *****************************************************************************
void block_read(MY_USART_OBJ *p_usart_obj)
{
    while (xSemaphoreTake(p_usart_obj->bsc_usart_obj->rx_semaphore, portMAX_DELAY) != pdTRUE); // Wait for RX semaphore

    USART_ERROR error = BSC_USART_ErrorGet(p_usart_obj->bsc_usart_obj); // Clear any errors
    if (error == USART_ERROR_NONE)
    {
        BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
        printu("RX:%s %02X->%02X[%s]\n", pcTaskGetCurrentTaskName(), msg->from_addr, msg->to_addr, msg->data);
    }
    else
    {
        printu("RD Error: %d\n", error);
    }
}

// *****************************************************************************
void rx_callback(MY_USART_OBJ *p_usart_obj)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(p_usart_obj->bsc_usart_obj->rx_semaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
        .command =BSC_OP_NACK,
        .count = 0, // No data to send
    };

    bsc_get_nack_t *nack = &response.get.nack;
    nack->command = command; // Set the command that caused the NACK
    nack->index = index; // Set the index of the bet spot that caused the NACK

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
void MASTER_Reset_Registry()
{
    printu("Resetting registry for all bet spots\n");
    for (int i = 0; i < NUMBER_OF_SLAVES; i++)
    {
        memset(&bs_objects[i].registry, 0, sizeof(bs_registry_entry_t)); // Clear the bet spot registry entry
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

        if (bs_object == NULL)
        {
            break;
        }
        if (params->mode == BSC_SENSOR_MODE_IMMEDIATE)
        {
            bs_object->sensor_parameters[0] = params->parameters;
        }
        else if (params->mode == BSC_SENSOR_MODE_HAND)
        {
            bs_object->sensor_parameters[1] = params->parameters;
        }
        else if (params->mode == BSC_SENSOR_MODE_CHIP)
        {
            bs_object->sensor_parameters[2] = params->parameters;
        }
        else
        {
            MASTER_Nack(set->command, params->index);
            break;
        }
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
void MASTER_Get_Registry()
{
    bsc_multicast_get_messages_t response =
    {
        .command =BSC_OP_GET_REGISTRY,
        .count = bs_count
    };

    for (int i = 0; i < bs_count; i++)
    {
        response.get.registry[i] = bs_objects[i].registry;
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
void MASTER_Get_Sensor_Values()
{
    // This function is called to get sensor values for all bet spots
    // The implementation will depend on the specific requirements of the application
    bsc_multicast_get_messages_t response =
    {
        .command =BSC_OP_GET_SENSOR_VALUES,
        .count = bs_count,
    };

    for (int i = 0; i < bs_count; i++)
    {
        bsc_get_get_sensor_values_t *sensor_values = &response.get.sensor_values[i];
        bs_object_t *bs_object = BS_Object_Get(i);
        sensor_values->index = i;
        sensor_values->data = bs_object->sensor_values.data; 
        sensor_values->int_flag = bs_object->sensor_values.int_flag;
        sensor_values->id = bs_object->sensor_values.id;
        sensor_values->ac_data = bs_object->sensor_values.ac_data;
    }
    // Send the response to the master response queue
    xQueueSend(master_response_queue, &response, portMAX_DELAY);
}

// *****************************************************************************
void MASTER_Get_Sensor_State()
{
    bsc_multicast_get_messages_t response =
    {
        .command =BSC_OP_GET_SENSOR_STATE,
        .count = bs_count,
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
        printu(" Index: %d, ",led_colors->index);
        print_color(&led_colors->colors);

        for(int j = 0; j < bs_object->registry.led_count; j++)
        {
            bs_object->colors[j] = led_colors->colors;
        }
    }
}

// *****************************************************************************
void MASTER_Initialize(void)
{

    xTaskCreate((TaskFunction_t)MASTER_Tasks, "MASTER_Task", 1024, &masterData, tskIDLE_PRIORITY + 2, &masterData.xTaskHandle);

    MY_USART_OBJ *my_usart_obj = &masterData.my_usart_obj;

    my_usart_obj->bsc_usart_obj = BSC_USART_Initialize(BSC_USART_SERCOM1, MASTER_ADDRESS);

    BSC_USART_ReadCallbackRegister(my_usart_obj->bsc_usart_obj, (SERCOM_USART_CALLBACK)rx_callback, (uintptr_t)my_usart_obj);

    master_message_queue = xQueueCreate(2, sizeof(bsc_multicast_set_messages_t));
    master_response_queue = xQueueCreate(2, sizeof(bsc_multicast_get_messages_t));
}

// *****************************************************************************
void MASTER_Tasks(MASTER_DATA *master_data)
{
    MY_USART_OBJ *my_usart_obj = &master_data->my_usart_obj;
    bsc_multicast_set_messages_t set;

    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize

    while (true)
    {
        /* Check the masterlication's current state. */
        switch (master_data->state)
        {
        /* Application's initial state. */
        case MASTER_STATE_INIT:

            for (int i = 0; i < 1; i++)
            {
                printf("  \nTransaction %c\n", my_usart_obj->tx_buffer.data[0]);
                for (int slave_address = SLAVES_ADDRESS_START; slave_address < SLAVES_ADDRESS_START + NUMBER_OF_SLAVES; slave_address++)
                {
                    printf("M->S%d\n", slave_address);
                    // build packet
                    my_usart_obj->tx_buffer.to_addr = slave_address;

                    // message from host to slaves
                    begin_read(my_usart_obj);
                    begin_write(my_usart_obj);

                    // response from slave to host
                    block_read(my_usart_obj);
                }

                my_usart_obj->tx_buffer.data[0] = my_usart_obj->tx_buffer.data[0] < 'Z' ? my_usart_obj->tx_buffer.data[0] + 1 : 'A';
                vTaskDelay(pdMS_TO_TICKS(1000));
                LED_GREEN_Toggle();
            }
            master_data->state = MASTER_STATE_SERVICE_TASKS;
            break;
        case MASTER_STATE_SERVICE_TASKS:
            if (xQueueReceive(master_message_queue, &set, portMAX_DELAY) == pdPASS)
            {
                switch (set.command)
                {
                case BSC_OP_RESET_REGISTRY:
                    MASTER_Reset_Registry();
                    break;
                case BSC_OP_SET_SENSOR_PARAMETRS:
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