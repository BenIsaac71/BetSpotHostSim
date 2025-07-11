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

// *****************************************************************************
typedef struct
{
    TaskHandle_t xTaskHandle;
    MY_USART_OBJ usart_obj;
    SLAVE_STATES state;

    bs_registry_entry_t registry;
    sensor_parameters_t sensor_parameters[3]; // Sensor parameters for the bet spot immediate, hand
    color_t colors[MAX_RGB_COLORS]; // RGB colors for the LEDs

    sensor_values_t sensor_values; // Sensor values for the bet spot
    bsc_sensor_state_t sensor_state;
} SLAVE_DATA;

// *****************************************************************************
void SLAVE_Initialize(void);


#ifdef __cplusplus
}
#endif

#endif /* _SLAVE_H */

