/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    slave.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "SLAVE_Initialize" and "SLAVE_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "SLAVE_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _SLAVE_H
#define _SLAVE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"


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
    SLAVE_STATE_INIT=0,
    SLAVE_STATE_WAITING_FOR_MESSAGE,
    SLAVE_STATE_TRANSMITTING_RESPONSE,
    /* TODO: Define states used by the application state machine. */

} SLAVE_STATES;


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
    SLAVE_STATES state;

    /* TODO: Define any additional data used by the application. */

} SLAVE_DATA;

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
    void SLAVE_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    SLAVE_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SLAVE_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void SLAVE_Initialize ( void );


/*******************************************************************************
  Function:
    void SLAVE_Tasks ( void )

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
    SLAVE_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */


/* Declaration of  SLAVE_Tasks task handle */
extern TaskHandle_t xSlave0TaskHandle;
extern TaskHandle_t xSlave1TaskHandle;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _SLAVE_H */

/*******************************************************************************
 End of File
 */

