#ifndef _SLAVE_H
#define _SLAVE_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"

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
    MY_USART_OBJ my_usart_obj;
    SLAVE_STATES state;
} SLAVE_DATA;

// *****************************************************************************
void SLAVE_Initialize(void);


#ifdef __cplusplus
}
#endif

#endif /* _SLAVE_H */

