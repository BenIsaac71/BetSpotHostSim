#ifndef _SLAVE_H
#define _SLAVE_H

// *****************************************************************************
#include "definitions.h"


#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    /* Application's state machine's initial state. */
    SLAVE_STATE_INIT = 0,
    SLAVE_STATE_WAITING_FOR_MESSAGE,
    SLAVE_STATE_TRANSMITTING_RESPONSE,
    /* TODO: Define states used by the application state machine. */

} SLAVE_STATES;


// *****************************************************************************
typedef struct
{
    /* The application's current state */
    SLAVE_STATES state;

    /* TODO: Define any additional data used by the application. */

} SLAVE_DATA;


// *****************************************************************************
void SLAVE0_Initialize(void);
void SLAVE0_Tasks(void *pvParameters);

void SLAVE1_Initialize(void);
void SLAVE1_Tasks(void *pvParameters);

// *****************************************************************************


#ifdef __cplusplus
}
#endif

#endif /* _SLAVE_H */

