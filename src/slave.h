#ifndef _SLAVE_H
#define _SLAVE_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"
#include "bsc_protocol.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    SLAVE_STATE_INIT = 0,
    SLAVE_STATE_WAITING_FOR_MESSAGE,
    SLAVE_STATE_TRANSMITTING_RESPONSE,
} SLAVE_STATES;


typedef struct
{
    bs_registry_entry_t registry; // Pointer to the registry entry
} bs_object_t;

// *****************************************************************************
typedef struct
{
    TaskHandle_t xTaskHandle;

    BSC_USART_OBJECT *p_usart_obj; // pointer to the BSC USART object
    BS_MESSAGE_BUFFER tx_buffer;
    BS_MESSAGE_BUFFER rx_buffer;
    SLAVE_STATES state;

    bool* p_check_in;    //simulate SPOT_CHECK_IN pull up input from previous bet spot
    bool check_out;      // simulate SPOT_CHECK_OUT output to next bet spot

    uint8_t test_count;

    bs_registry_entry_t registry; // Pointer to the registry entry
    color_t colors[MAX_RGB_COLORS]; // RGB colors for the LEDs
    bsc_sensor_state_t sensor_state; // Sensor state for the bet spot
    bsc_sensor_mode_t sensor_mode; // Sensor mode for the bet spot
    sensor_parameters_t sensor_parameters[3]; // Sensor parameters for the bet spot immediate, hand, chip modes
    sensor_values_t sensor_values; // Sensor values for the bet spot

} SLAVE_OBJ;

// *****************************************************************************
void SLAVE_Initialize(void);


#ifdef __cplusplus
}
#endif

#endif /* _SLAVE_H */

