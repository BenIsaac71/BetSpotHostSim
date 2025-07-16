#ifndef _APP_H
#define _APP_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
#define DEBUG_LOGGING_ENABLED 1

#if DEBUG_LOGGING_ENABLED == 1
#define printu(...) SYS_CONSOLE_Print(0, __VA_ARGS__)
#elif DEBUG_LOGGING_ENABLED == 2
#define printu(...) printf(__VA_ARGS__)
#else
#define printu(...) do { if (0) SYS_CONSOLE_Print(0, __VA_ARGS__); } while (0)
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
