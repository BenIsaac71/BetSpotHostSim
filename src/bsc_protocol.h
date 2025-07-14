//put the standard start ifdef guard here
#ifndef _BSC_PROTOCOL_H
#define _BSC_PROTOCOL_H

#include "definitions.h"

// *****************************************************************************
#define MAX_BET_SPOTS 30
#define MAX_RGB_COLORS 20

// *****************************************************************************
// Host to Bet Spot Controller (BSC) operations
typedef enum
{
    BSC_OP_SET_TEST = 0x00,
    BSC_OP_RESET_REGISTRY,
    
    BSC_OP_SET_SENSOR_PARAMETERS,
    BSC_OP_SET_SENSOR_MODE,
    BSC_OP_SET_LED_COLORS,

    BSC_OP_GET_REGISTRY = 0x80,
    BSC_OP_GET_SENSOR_VALUES,
    BSC_OP_GET_SENSOR_STATE,

    BSC_OP_NACK = 0xFF,
} BSC_OP_t;

typedef enum
{
    BS_OP_SET_TEST = 0x00,
    BS_OP_RESET_ADDRESS,
    BS_OP_SET_ADDRESS,
    
    BS_OP_SET_SENSOR_PARAMETERS,
    BS_OP_SET_SENSOR_MODE,
    BS_OP_SET_LED_COLORS,

    BS_OP_GET_REGISTRY = 0x81,
    BS_OP_GET_SENSOR_VALUES,
    BS_OP_GET_SENSOR_STATE
} BS_OP_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t addr;
    uint8_t serial_number[12];
} bs_set_address_t;


// *****************************************************************************
typedef enum
{
    BSC_SENSOR_STATE_NONE,      //no presence
    BSC_SENSOR_STATE_IMMEDIATE, //immediate mode
    BSC_SENSOR_STATE_HAND,      //hand detected
    BSC_SENSOR_STATE_CHIP,      //chip detected
} bsc_sensor_state_t;

// *****************************************************************************
typedef enum
{
    BSC_SENSOR_MODE_IMMEDIATE, // Sensor mode: immediate
    BSC_SENSOR_MODE_HAND,      // Sensor mode: hand
    BSC_SENSOR_MODE_CHIP,      // Sensor mode: chip
    BSC_SENSOR_MODE_MAX        // Maximum sensor mode value
} bsc_sensor_mode_t;

// *****************************************************************************
typedef __PACKED_UNION
{
    uint8_t addr;
    struct
    {
        uint8_t id: 4;
        uint8_t port: 4;
    } location;
} bsc_address_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

#define SIZEOF_SERIAL_NUMBER 12
// *****************************************************************************
typedef __PACKED_STRUCT
{
    bsc_address_t address;
    uint8_t led_count;
    uint8_t hw_version;
    uint8_t serial_number[SIZEOF_SERIAL_NUMBER];
} bsc_get_registry_t;

typedef bsc_get_registry_t  bs_registry_entry_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint16_t conf1;
    uint16_t conf2;
    uint16_t conf3;
    uint16_t thdl;
    uint16_t thdh;
    uint16_t canc;
    uint16_t conf4;
} sensor_parameters_t;

typedef __PACKED_STRUCT
{
    uint8_t index;
    bsc_sensor_mode_t mode;
    sensor_parameters_t parameters; // sensor parameters for the mode
} bsc_set_sensor_parameters_t;

typedef struct
{
    uint16_t data;
    uint16_t int_flag;
    uint16_t id;
    uint16_t ac_data;
} sensor_values_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t index;
    uint16_t data;
    uint16_t int_flag;
    uint16_t id;
    uint16_t ac_data;
} bsc_get_get_sensor_values_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t index;
    bsc_sensor_mode_t mode;
} bsc_set_sensor_mode_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t index;
    bsc_sensor_mode_t mode;
    bsc_sensor_state_t state;
} bsc_get_sensor_state_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t index;
    color_t colors;  // all pixels same color
} bsc_set_led_colors_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    uint8_t index;
    BSC_OP_t command;
} bsc_get_nack_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    BSC_OP_t command;
    uint8_t count;  //count of zero indicates all devices in the registry
    __PACKED_UNION
    {
        bsc_set_sensor_parameters_t sensor_parameter[MAX_BET_SPOTS];
        bsc_set_sensor_mode_t sensor_mode[MAX_BET_SPOTS];
        bsc_set_led_colors_t led_colors[MAX_BET_SPOTS];
    } set;
} bsc_multicast_set_messages_t;

// *****************************************************************************
typedef __PACKED_STRUCT
{
    BSC_OP_t command;
    uint8_t count;
    __PACKED_UNION
    {
        bsc_get_registry_t registry[MAX_BET_SPOTS];
        bsc_get_get_sensor_values_t sensor_values[MAX_BET_SPOTS];
        bsc_get_sensor_state_t sensor_state[MAX_BET_SPOTS];
        bsc_get_nack_t nack;
    } get;
} bsc_multicast_get_messages_t;


// *****************************************************************************
#define MASTER_ADDRESS 0xFF
#define GLOBAL_ADDRESS 0x00

#define MAX_DATA_SIZE 0xFF
typedef struct __packed
{
    uint8_t to_addr;
    uint8_t data_len;
    uint8_t from_addr;
    BS_OP_t op;
    uint8_t data[MAX_DATA_SIZE];
    uint32_t crc;
} BS_MESSAGE_BUFFER;
#define BS_MESSAGE_META_SIZE (sizeof(BS_MESSAGE_BUFFER) - MAX_DATA_SIZE)

// *****************************************************************************
#endif //_BSC_PROTOCOL_H

/*******************************************************************************
* BSC Protocol Overview
* This protocol is designed for communication with bus sensors (BS) in a system.
* It includes control requests for managing the bus sensors, retrieving their state,
* and setting parameters. The protocol is structured to allow for easy expansion
* and modification as new features are added.
* The protocol consists of several control requests and responses, each with a specific purpose.
* The requests are designed to be sent over a usb control transfers,
* and the responses are structured to provide the necessary information back to the requester.
* Events reported through bulk transfers allow for real-time updates on sensor states and other relevant information.
* The protocol supports multiple bus sensors, allowing for a flexible and scalable system.
* The following sections detail the specific control requests and their expected responses.
*******************************************************************************
control requests
*******************************************************************************
SET_RESET_MANIFEST
    clear all know addr to unique id mapping
    for calibration set up phase

GET_MANIFEST
response
    state,
    [bs_count]
        {port,id}

    port:
        0..9 of the uart ports
    id: 96bt or 12 byte unique identifier
    if engaged in querying bs devices
        state = busy, bs_count = 0
    else if bs_count == 0
        bsc will run a query
        state = busy, count = 0
    else
        if manifest matches bus discovery
            state = complete, bs_count, {port,id}
            can also occur if manifest was reset and query return bs_count=0
        else
            state = mismatch and list of devices could be added or removed
            can also occur if manifest was reset and query return bs_count > 0

SET_SENSOR_PARAMETER
response
    [bs_count]
        {mode , parameters}

    sensor mode:
        0 = disable event report for this bs sensor parameters ignored
        1 = immediate
        2 hand = non volatile on commit manifest
        3 chip = non volatile on commit manifest
    immediate/disable can be used to calibrate sensor and isolate one group/device
    sensor parameters:
        TBD

SET_COMMIT MANIFEST
    [bs_count]
        {addr}
    save manifest to non volatile for future boots

SET SENSOR MODE
    [bs_count]
        {sensor _mode}
    enables event report for non disabled

GET_SENSOR_STATE
response
    [bs_count]
        {mode:2,state:1}
    reports sensor state for all bs in manifest

GET_LED_COUNTS
response
    [bs_count]
        {led count}
    bet spots can detect how many pixels and report this info in case someone decides to change the number of the pixels in the future or have different button types

*******************************************************************************
endpoint control
*******************************************************************************
draw color
    [all pixels]
        {r,g,b}
    array indexed to manifest and pixels count of each bs device
    in normal operation
        or
    useful to guide user calibration
    prompt:touch first button in button group lit red.
        turn that button green when it has been detected.
    prompt:press the next button in the group

sensor state event
    [bs_count]
        {com_error:1:mode:2,sensor_state:1}
    mode enabled button will fire change event
    disabled ones will still report current value
*/