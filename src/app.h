#ifndef _APP_H
#define _APP_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"

#define printu(...) SYS_CONSOLE_Print(0, __VA_ARGS__)

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    APP_STATE_INIT = 0,
    APP_STATE_SERVICE_TASKS,
} APP_STATES;

// *****************************************************************************
typedef struct
{
    /* The application's current state */
    APP_STATES state;
} APP_DATA;

// *****************************************************************************
void APP_Initialize(void);
void APP_Tasks(void);

#ifdef __cplusplus
}
#endif

#endif /* _APP_H */
