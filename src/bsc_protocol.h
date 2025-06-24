// *****************************************************************************
#define MAX_BET_SPOTS 30

// *****************************************************************************
typedef enum
{
    BSC_SET_RESET_MANIFEST = 0x01,
    BSC_GET_MANIFEST = 0x02,
    BSC_SET_SENSOR_PARAMETER = 0x03,
    BSC_GET_SENSOR_VALUES = 0x03,
    BSC_SET_COMMIT_MANIFEST = 0x04,
    BSC_SET_SENSOR_MODE = 0x05,
    BSC_GET_SENSOR_STATE = 0x06,
    BSC_GET_LED_COUNT = 0x07,
} bsc_command_t;

typedef enum
{
    BSC_STATE_BUSY = 0,
    BSC_STATE_COMPLETE = 1,
    BSC_STATE_MISMATCH = 2,
} bsc_state_t;

// *****************************************************************************
// *****************************************************************************
typedef struct
{
    bsc_state_t state; // State of the bus discovery
    uint8_t bs_count; // Count of bet spots
    bsc_manifest_t devices[MAX_BET_SPOTS]; // Array of discovered devices, max 30
} bsc_get_manifest_t;

// *****************************************************************************
typedef enum
{
    BS_MODE_DISABLED = 0,
    BS_MODE_IMMEDIATE = 1,
    BS_MODE_HAND = 2,
    BS_MODE_CHIP = 3,
    BS_MODE_COM_ERROR = 4,
} bsc_mode_t;

typedef struct
{
    uint16_t PS_CONF1;
    uint16_t PS_CONF2;
    uint16_t PS_CONF3;
    uint16_t PS_THDL;
    uint16_t PS_THDH;
    uint16_t PS_CANC;
    uint16_t PS_CONF4;
} ps_sensor_set_parameters;

typedef struct
{
    bsc_sensor_mode_t mode; // Sensor mode: 0 = disabled, 1 = immediate, 2 = hand, 3 = chip
    uint8_t bs_count; // Count of bet spots
    ps_sensor_set_parameters registers;
} bsc_set_sensor_parameters_t;

// *****************************************************************************
typedef struct
{
    uint16_t PS_CH_OUT_DATA;
    uint16_t PS_INT_FLAG;
    uint16_t PS_ID;
    uint8_t PS_AC_DATA;
} ps_sensor_get_values_t;

typedef struct
{
    uint8_t bs_count; // Count of bet spots
    ps_sensor_get_values_t registers;
} bsc_get_sensor_values_t;

// *****************************************************************************
typedef
{
    uint8_t bs_count_t;
    bsc_mode_t mode; // Sensor mode: 0 = disabled, 1 = immediate, 2 = hand, 3 = chip
} bsc_sensor_mode_t;

// *****************************************************************************
typedef struct
{
    uint8_t sensor_state : 1; // Sensor state: 0 = inactive, 1 = active
    uint8_t reserved     : 3; // Reserved bits for future use
    bsc_mode_t mode      : 4; // Sensor mode: 0 = disabled, 1 = immediate, 2 = hand, 3 = chip, 4 = communication error
} bsc_sensor_state_t;

typedef struct
{
    uint8_t bs_count; // Count of bet spots
    bsc_sensor_state_t states[MAX_BET_SPOTS];
} bsc_get_sensor_state_t;

// *****************************************************************************
typedef struct
{
    uint8_t bs_count; // Count of bet spots
    uint8_t led_count[MAX_BET_SPOTS]; // Count of LEDs of bet spots
} bsc_get_led_count_t;


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
    clear all know address to unique id mapping
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
        {address}
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