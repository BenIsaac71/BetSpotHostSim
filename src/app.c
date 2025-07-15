#include "definitions.h"

// *****************************************************************************
APP_DATA appData;


// *****************************************************************************
void cmd_com_test(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_set_messages_t msg =
    {
        .command = BSC_OP_SET_TEST,
        //convert arvg[1] to integer and set it as count
    };
    *(uint32_t *)(&msg.count) = (argc > 1) ? atoi(argv[1]) : 1;
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}


// *****************************************************************************
void cmd_set_reset_registry(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_set_messages_t msg =
    {
        .command = BSC_OP_RESET_REGISTRY,
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_set_sensor_parameters(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_set_messages_t msg =
    {
        .command = BSC_OP_SET_SENSOR_PARAMETERS,
        .count = 2,
        .set.sensor_parameter = {
            {
                .index = 1,
                .sensor = {
                    .mode = BSC_SENSOR_MODE_IMMEDIATE,
                    .parameters = {
                        .conf1 = 0x01,
                        .conf2 = 0x02,
                        .conf3 = 0x03,
                        .thdl = 0x04,
                        .thdh = 0x05,
                        .canc = 0x06,
                        .conf4 = 0x07
                    }
                }
            },
            {
                .index = 0,
                .sensor = {
                    .mode = BSC_SENSOR_MODE_HAND,
                    .parameters = {
                        .conf1 = 0x08,
                        .conf2 = 0x09,
                        .conf3 = 0x0A,
                        .thdl = 0x0B,
                        .thdh = 0x0C,
                        .canc = 0x0D,
                        .conf4 = 0x0E
                    }
                }
            }
        }
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_set_sensor_mode(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_set_messages_t msg =
    {
        .command = BSC_OP_SET_SENSOR_MODE,
        .count = 2,
        .set.sensor_mode = {
            {
                .index = 1,
                .mode = BSC_SENSOR_MODE_HAND
            },
            {
                .index = 0,
                .mode = BSC_SENSOR_MODE_CHIP
            }
        }
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_set_led_colors(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_set_messages_t msg =
    {
        .command = BSC_OP_SET_LED_COLORS,
        .count = 2,
        .set.led_colors =
        {
            {
                .index = 1,
                .color = {0xFF, 0x00, 0x00}
            },
            {
                .index = 0,
                .color = {0x00, 0x00, 0xFF}
            }
        }
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_get_registry(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_get_messages_t msg =
    {
        .command = BSC_OP_GET_REGISTRY,
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_get_sensor_values(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_get_messages_t msg =
    {
        .command = BSC_OP_GET_SENSOR_VALUES,
    };
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
void cmd_get_sensor_state(SYS_CMD_DEVICE_NODE *pCmdIO, int argc, char **argv)
{
    bsc_multicast_get_messages_t msg =
    {
        .command = BSC_OP_GET_SENSOR_STATE,
    };
    // Send the get sensor state message to the master task
    xQueueSend(master_message_queue, &msg, OSAL_WAIT_FOREVER);
}

// *****************************************************************************
static const SYS_CMD_DESCRIPTOR comm_cmds[] =
{
    {"ctst", cmd_com_test,              ": Comm test command      "},
    {"sreg", cmd_set_reset_registry,    ": Reset bet spot registry"},
    {"spar", cmd_set_sensor_parameters, ": Set sensor parameters  "},
    {"smod", cmd_set_sensor_mode,       ": Set sensor mode        "},
    {"sled", cmd_set_led_colors,        ": Set leds color         "},
    {"greg", cmd_get_registry,          ": Get bet spot registry  "},
    {"gval", cmd_get_sensor_values,     ": Get sensor values      "},
    {"gsta", cmd_get_sensor_state,      ": Get sensor state       "},
};

// *****************************************************************************
void APP_Initialize(void)
{
    TC0_TimerStart();
    MASTER_Initialize();
    SLAVE_Initialize();
    SYS_CMD_ADDGRP(comm_cmds, sizeof(comm_cmds) / sizeof(SYS_CMD_DESCRIPTOR), "bsc", ": Bet Spot Controller protocol commands");

    appData.state = APP_STATE_INIT;
}

void APP_State_Service_Tasks(void)
{
    bsc_multicast_get_messages_t rsp;

    // Wait for the master task to respond (timeout 100ms)
    if (xQueueReceive(master_response_queue, &rsp, pdMS_TO_TICKS(100)) == pdPASS)
    {
        switch (rsp.command)
        {
        case BSC_OP_GET_REGISTRY:
            printu("Get Bet Spot Registry for %d bet spots:\n", rsp.count);
            for (int i = 0; i < rsp.count; i++)
            {
                bsc_get_registry_t *registry = &rsp.get.registry[i];
                printu(" Address: %d.%d, LED Count: %d, HW Version: %02X, Serial Number: ",
                       registry->address.location.port, registry->address.location.id,
                       registry->led_count,
                       registry->hw_version);
                print_hex_data(registry->serial_number, sizeof(registry->serial_number));
            }
            break;
        case BSC_OP_GET_SENSOR_VALUES:
            printu("Get Sensor Values for %d bet spots:\n", rsp.count);
            for (int i = 0; i < rsp.count; i++)
            {
                bsc_get_get_sensor_values_t *sensor_values = &rsp.get.sensor_values[i];
                printu(" Index: %d, Data: %04X, Int Flag: %04X, ID: %04X, AC Data: %04X\n",
                       sensor_values->index,
                       sensor_values->values.data,
                       sensor_values->values.int_flag,
                       sensor_values->values.id,
                       sensor_values->values.ac_data);
            }
            break;
        case BSC_OP_GET_SENSOR_STATE:
            printu("Get Sensor States for %d bet spots:\n", rsp.count);
            for (int i = 0; i < rsp.count; i++)
            {
                bsc_get_sensor_state_t *sensor_state = &rsp.get.sensor_state[i];
                printu(" Index: %d, State: %d\n",
                       sensor_state->index,
                       sensor_state->state);
            }
            break;
        case BSC_OP_ERROR:
            printu("Invalid operation response received.\n");
            printu(" Command: %d\n, Index: %d\n", rsp.get.error.command, rsp.get.error.index);
            break;
        default:
            printu("Unknown command response: %d\n", rsp.command);
            break;
        }
    }
}

// *****************************************************************************
void APP_Tasks(void)
{
    switch (appData.state)
    {
    case APP_STATE_INIT:
        appData.state = APP_STATE_SERVICE_TASKS;
        break;
    case APP_STATE_SERVICE_TASKS:
        APP_State_Service_Tasks();
        break;
    default:
        break;
    }
}
