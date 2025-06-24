/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"
#include "app.h"
#include "stdio.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


typedef enum
{
    DRV_USART_INDEX_MASTER,
    DRV_USART_INDEX_SLAVE0,
    DRV_USART_INDEX_SLAVE1,
    DRV_USART_INDEX_MAX
} DRV_USART_INDEX;

typedef enum
{
    BUF_TYPE_TX,
    BUF_TYPE_RX,
    BUF_TYPE_MAX
} BUF_TYPE;


#define USART_BUFFER_SIZE 3
typedef struct
{
    DRV_USART_INDEX index;
    DRV_HANDLE usart_handle;
    DRV_USART_BUFFER_HANDLE tx_buffer_handle;
    DRV_USART_BUFFER_HANDLE rx_buffer_handle;
    uint8_t tx_buffer[USART_BUFFER_SIZE];
    uint8_t rx_buffer[USART_BUFFER_SIZE];
} MY_USART_OBJ;

MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX] =
{
    [DRV_USART_INDEX_MASTER] = {
        .index = DRV_USART_INDEX_MASTER,
        .usart_handle = DRV_HANDLE_INVALID,
        .tx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .rx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .tx_buffer = {1, 2, 3},
        .rx_buffer = {0, 0, 0}  
    },
    [DRV_USART_INDEX_SLAVE0] = {
        .index = DRV_USART_INDEX_SLAVE0,
        .usart_handle = DRV_HANDLE_INVALID,
        .tx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .rx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .tx_buffer = {4, 5, 6},
        .rx_buffer = {0, 0, 0}  
    },
    [DRV_USART_INDEX_SLAVE1] = {
        .index = DRV_USART_INDEX_SLAVE1,
        .usart_handle = DRV_HANDLE_INVALID,
        .tx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .rx_buffer_handle = DRV_USART_BUFFER_HANDLE_INVALID,
        .tx_buffer = {7, 8, 9},
        .rx_buffer = {0, 0, 0}  
    }
    
};



// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void usart_event_handler(
    DRV_USART_BUFFER_EVENT event,
    DRV_USART_BUFFER_HANDLE bufferHandle,
    uintptr_t context
)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)context;

    if (event == DRV_USART_BUFFER_EVENT_COMPLETE)
    {
        if (bufferHandle == p_usart_obj->rx_buffer_handle)
        {
            // RX buffer is filled, process received data
            printf("RX complete: Index: %d\n", p_usart_obj->index);
        }
        else if (bufferHandle == p_usart_obj->tx_buffer_handle)
        {
            // TX buffer sent
            printf("TX complete: Index: %d\n", p_usart_obj->index);
        }
        else
        {
            // Unknown buffer handle (should not happen)
        }
    }
    else if (event == DRV_USART_BUFFER_EVENT_ERROR)
    {
        // Handle error
        printf("USART Error: Index: %d\n", p_usart_obj->index);
    }
}


/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */

    for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
    {
        DRV_HANDLE drv_handle;

        drv_handle = DRV_USART_Open(i, DRV_IO_INTENT_EXCLUSIVE | DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
        if (drv_handle == DRV_HANDLE_INVALID)
        {
            // Handle error
        }
        else
        {
            usart_objs[i].usart_handle = drv_handle;
            DRV_USART_BufferEventHandlerSet(
                drv_handle,
                usart_event_handler,
                (uintptr_t)&usart_objs[i]
            );  
        }
    }
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
    /* Application's initial state. */
    case APP_STATE_INIT:
    {
        bool appInitialized = true;


        if (appInitialized)
        {

            appData.state = APP_STATE_SERVICE_TASKS;
        }
        break;
    }

    case APP_STATE_SERVICE_TASKS:
    {
        printf("hi\n");
        LED_RED_Toggle();

        for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
        {
            MY_USART_OBJ *usart_rx_obj = &usart_objs[i];
            DRV_USART_ReadBufferAdd(
                usart_rx_obj->usart_handle,
                usart_rx_obj->rx_buffer,
                USART_BUFFER_SIZE,
                &usart_rx_obj->rx_buffer_handle
            );
        }

        for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
        {
            MY_USART_OBJ *usart_tx_obj = &usart_objs[i];

            DRV_USART_WriteBufferAdd(
                usart_tx_obj->usart_handle,
                usart_tx_obj->tx_buffer,
                USART_BUFFER_SIZE,
                &usart_tx_obj->tx_buffer_handle
            );
            vTaskDelay(pdMS_TO_TICKS(5));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        break;
    }
    /* TODO: implement your application state machine.*/


    /* The default state should never be executed. */
    default:
    {
        /* TODO: Handle error in application's state machine. */
        break;
    }
    }
}


/*******************************************************************************
 End of File
 */
