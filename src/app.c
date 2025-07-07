#include "definitions.h"
APP_DATA appData;

void APP_Initialize(void)

{
    TC0_TimerStart();
    MASTER_Initialize();
    SLAVE_Initialize();

    appData.state = APP_STATE_INIT;
}

void APP_Tasks(void)
{
    switch (appData.state)
    {
    case APP_STATE_INIT:
        appData.state = APP_STATE_SERVICE_TASKS;
        break;
    case APP_STATE_SERVICE_TASKS:
        break;
    default:
        break;
    }
}
