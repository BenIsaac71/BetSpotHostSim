/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "bsc_usarts.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,
    APP_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} APP_STATES;

typedef enum
{
    USART_DMA_CHANNEL_MASTER_TX = DMAC_CHANNEL_0,
    USART_DMA_CHANNEL_SLAVE0_TX = DMAC_CHANNEL_1,
    USART_DMA_CHANNEL_SLAVE1_TX = DMAC_CHANNEL_2,

    USART_DMA_CHANNEL_MASTER_RX = DMAC_CHANNEL_3,
    USART_DMA_CHANNEL_SLAVE0_RX = DMAC_CHANNEL_4,
    USART_DMA_CHANNEL_SLAVE1_RX = DMAC_CHANNEL_5,
} USART_DMAC_CHANNELS;

typedef enum
{
    DRV_USART_INDEX_MASTER,
    DRV_USART_INDEX_SLAVE0,
    DRV_USART_INDEX_SLAVE1,
    DRV_USART_INDEX_MAX
} DRV_USART_INDEX;

#define DRV_BSC_USART_MASTER BSC_USART_SERCOM1
#define DRV_BSC_USART_SLAVE0 BSC_USART_SERCOM5
#define DRV_BSC_USART_SLAVE1 BSC_USART_SERCOM4



#define NUMBER_OF_SLAVES 2

#define USART_SIGNAL_COMPLETE_TX(drv_usart_index) (0x01<<(drv_usart_index))
#define USART_SIGNAL_COMPLETE_RX(drv_usart_index) (0x10<<(drv_usart_index))

typedef enum
{
    USART_SIGNAL_COMPLETE_MASTER_TX = USART_SIGNAL_COMPLETE_TX(DRV_USART_INDEX_MASTER),
    USART_SIGNAL_COMPLETE_SLAVE0_TX = USART_SIGNAL_COMPLETE_TX(DRV_USART_INDEX_SLAVE0),
    USART_SIGNAL_COMPLETE_SLAVE1_TX = USART_SIGNAL_COMPLETE_TX(DRV_USART_INDEX_SLAVE1),
    USART_SIGNAL_COMPLETE_MASTER_RX = USART_SIGNAL_COMPLETE_RX(DRV_USART_INDEX_MASTER),
    USART_SIGNAL_COMPLETE_SLAVE0_RX = USART_SIGNAL_COMPLETE_RX(DRV_USART_INDEX_SLAVE0),
    USART_SIGNAL_COMPLETE_SLAVE1_RX = USART_SIGNAL_COMPLETE_RX(DRV_USART_INDEX_SLAVE1),
    USART_SIGNAL_ERROR_FLAG = 0x100
} USART_NOTIFICATION_VALUE;


#define SLAVE0_ADDRESS  0x1
#define SLAVE1_ADDRESS  0x2

#define MASTER_DATA " -Master\0"
#define SLAVE0_DATA " -Slave0\0"
#define SLAVE1_DATA " -Slave1\0"

#define USART_BUFFER_SIZE (sizeof(BS_MESSAGE_BUFFER)) // todo;remove

typedef enum
{
    MY_USART_EVENT_PACKET_READY,
    MY_USART_EVENT_PACKET_ERROR,
} MY_USART_EVENT;

typedef enum
{
    MY_USART_PACKET_STATE_IDLE = 0,
    MY_USART_PACKET_WAIT_START_BYTE,     //must be MY_USART_PACKET_START_BYTE
    MY_USART_PACKET_WAIT_ADDRESS_BYTE,   //will never be MY_USART_PACKET_START_BYTE
    MY_USART_PACKET_WAIT_LENGTH_BYTE,    //will never be MY_USART_PACKET_START_BYTE, start dmac transfer
    MY_USART_PACKET_WAIT_DATA,           //DMA transfer in progress
    MY_USART_PACKET_COMPLETE,            //DMA transfer complete
} MY_USART_PACKET_STATE;

typedef void (*MY_USART_OBJ_CALLBACK)(MY_USART_EVENT event, uintptr_t context );

typedef struct

{
    DRV_USART_INDEX index;
    BSC_USART_OBJECT* bsc_usart_obj; // pointer to the BSC USART object
    sercom_registers_t *sercom_regs;
    USART_DMAC_CHANNELS dmac_channel_tx;
    USART_DMAC_CHANNELS dmac_channel_rx;
    BS_MESSAGE_BUFFER tx_buffer;
    BS_MESSAGE_BUFFER rx_buffer;
    TaskHandle_t task_handle;
} MY_USART_OBJ;


#define USART_0   (&usartObjs[0])
#define USART_1   (&usartObjs[1])

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;

    /* TODO: Define any additional data used by the application. */

} APP_DATA;



// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );
void USART_RX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
void USART_TX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
extern MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX];
void USART_TE_Set(DRV_USART_INDEX index);

// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Handles
// *****************************************************************************
// *****************************************************************************

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

