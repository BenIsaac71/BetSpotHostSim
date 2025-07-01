/*******************************************************************************
  SERCOM Universal Synchronous/Asynchronous Receiver/Transmitter PLIB

  Company
    Microchip Technology Inc.

  File Name
    bsc_usart_usart_common.h

  Summary
    Data Type definition of the BSC USART Peripheral Interface.

  Description
    This file defines the Data Types for the USART Plib.

  Remarks:
    None.
*******************************************************************************/

#ifndef BSC_USART_COMMON_H // Guards against multiple inclusion
#define BSC_USART_COMMON_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section:Preprocessor macros
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USART Error convenience macros */
// *****************************************************************************
// *****************************************************************************
    /* Error status when no error has occurred */
#define USART_ERROR_NONE 0U

    /* Error status when parity error has occurred */
#define USART_ERROR_PARITY SERCOM_USART_INT_STATUS_PERR_Msk

    /* Error status when framing error has occurred */
#define USART_ERROR_FRAMING SERCOM_USART_INT_STATUS_FERR_Msk

    /* Error status when overrun error has occurred */
#define USART_ERROR_OVERRUN SERCOM_USART_INT_STATUS_BUFOVF_Msk


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

typedef enum 
{
    BSC_USART_SERCOM0,
    BSC_USART_SERCOM1,
    BSC_USART_SERCOM2,
    BSC_USART_SERCOM3,
    BSC_USART_SERCOM4,
    BSC_USART_SERCOM5,
    BSC_USART_SERCOM_MAX
} BSC_USART_SERCOM_ID;


// *****************************************************************************
/* USART Errors

  Summary:
    Defines the data type for the USART peripheral errors.

  Description:
    This may be used to check the type of error occurred with the USART
    peripheral during error status.

  Remarks:
    None.
*/

typedef uint16_t USART_ERROR;

// *****************************************************************************
/* USART DATA

  Summary:
    Defines the data type for the USART peripheral data.

  Description:
    This may be used to check the type of data with the USART
    peripheral during serial setup.

  Remarks:
    None.
*/

typedef enum
{
    USART_DATA_5_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_5_BIT,
    USART_DATA_6_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_6_BIT,
    USART_DATA_7_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_7_BIT,
    USART_DATA_8_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT,
    USART_DATA_9_BIT = SERCOM_USART_INT_CTRLB_CHSIZE_9_BIT,


    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_DATA_INVALID = 0xFFFFFFFFU

} USART_DATA;

// *****************************************************************************
/* USART PARITY

  Summary:
    Defines the data type for the USART peripheral parity.

  Description:
    This may be used to check the type of parity with the USART
    peripheral during serial setup.

  Remarks:
    None.
*/

typedef enum
{
    USART_PARITY_EVEN = SERCOM_USART_INT_CTRLB_PMODE_EVEN,

    USART_PARITY_ODD = SERCOM_USART_INT_CTRLB_PMODE_ODD,

    /* This enum is defined to set frame format only
     * This value won't be written to register
     */
    USART_PARITY_NONE = 0x2,

    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_PARITY_INVALID = 0xFFFFFFFFU

} USART_PARITY;

// *****************************************************************************
/* USART STOP

  Summary:
    Defines the data type for the USART peripheral stop bits.

  Description:
    This may be used to check the type of stop bits with the USART
    peripheral during serial setup.

  Remarks:
    None.
*/

typedef enum
{
    USART_STOP_0_BIT = SERCOM_USART_INT_CTRLB_SBMODE_1_BIT,
    USART_STOP_1_BIT = SERCOM_USART_INT_CTRLB_SBMODE_2_BIT,


    /* Force the compiler to reserve 32-bit memory for each enum */
    USART_STOP_INVALID = 0xFFFFFFFFU

} USART_STOP;

// *****************************************************************************
/* USART LIN Command

  Summary:
    Defines the data type for the USART peripheral LIN Command.

  Description:
    This may be used to set the USART LIN Master mode command.

  Remarks:
    None.
*/

typedef enum
{
    USART_LIN_MASTER_CMD_NONE = SERCOM_USART_INT_CTRLB_LINCMD_NONE,

    USART_LIN_MASTER_CMD_SOFTWARE_CONTROLLED = SERCOM_USART_INT_CTRLB_LINCMD_SOFTWARE_CONTROL_TRANSMIT_CMD,

    USART_LIN_MASTER_CMD_AUTO_TRANSMIT = SERCOM_USART_INT_CTRLB_LINCMD_AUTO_TRANSMIT_CMD

} USART_LIN_MASTER_CMD;

// *****************************************************************************
/* USART Serial Configuration

  Summary:
    Defines the data type for the USART serial configurations.

  Description:
    This may be used to set the serial configurations for USART.

  Remarks:
    None.
*/

typedef struct
{
    uint32_t baudRate;

    USART_PARITY parity;

    USART_DATA dataWidth;

    USART_STOP stopBits;

} USART_SERIAL_SETUP;

// *****************************************************************************
/* Callback Function Pointer

  Summary:
    Defines the data type and function signature for the USART peripheral
    callback function.

  Description:
    This data type defines the function signature for the USART peripheral
    callback function. The USART peripheral will call back the client's
    function with this signature when the USART buffer event has occurred.

  Remarks:
    None.
*/

typedef void (*SERCOM_USART_CALLBACK)( uintptr_t context );
typedef void (*TE_PIN_FUNCTION)(void);

// *****************************************************************************
/* BSC USART Object

  Summary:
    Defines the data type for the data structures used for
    peripheral operations.

  Description:
    This may be for used for peripheral operations.

  Remarks:
    None.
*/

typedef struct
{
    BSC_USART_SERCOM_ID                 bsc_usart_id;

    TE_PIN_FUNCTION                     te_set;

    TE_PIN_FUNCTION                     te_clr;

    uint8_t                             address;

    sercom_registers_t*                 sercom_regs;
    
    uint32_t                            peripheral_clk_freq;

    void *                              txBuffer;

    size_t                              txSize;

    size_t                              txProcessedSize;

    SERCOM_USART_CALLBACK               txCallback;

    uintptr_t                           txContext;

    bool                                txBusyStatus;

    void *                              rxBuffer;

    size_t                              rxSize;

    size_t                              rxProcessedSize;

    SERCOM_USART_CALLBACK               rxCallback;

    uintptr_t                           rxContext;

    bool                                rxBusyStatus;

    USART_ERROR                         errorStatus;

} BSC_USART_OBJECT;


typedef enum
{
    /* Threshold number of bytes are available in the receive ring buffer */
    SERCOM_USART_EVENT_READ_THRESHOLD_REACHED = 0,

    /* Receive ring buffer is full. Application must read the data out to avoid missing data on the next RX interrupt. */
    SERCOM_USART_EVENT_READ_BUFFER_FULL,

    /* USART error. Application must call the SERCOMx_USART_ErrorGet API to get the type of error and clear the error. */
    SERCOM_USART_EVENT_READ_ERROR,

    /* Threshold number of free space is available in the transmit ring buffer */
    SERCOM_USART_EVENT_WRITE_THRESHOLD_REACHED,

    /* Recevie break signal is detected */
    SERCOM_USART_EVENT_BREAK_SIGNAL_DETECTED,
}SERCOM_USART_EVENT;


#define MASTER_ADDRESS 0xFF
#define GLOBAL_ADDRESS 0x00
typedef struct __packed
{
    uint8_t to_addr;
    uint8_t data_len;
    uint8_t from_addr;
    uint8_t op;
    uint8_t data[0xFF];
    uint32_t crc;//run via dma peripheral as it is rxed. length is'nt part of dma so could include after dmac complete and validate rxed crc
} BS_MESSAGE_BUFFER;
#define BS_MESSAGE_ADDITIONAL_SIZE sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t)
#define BS_USART_BUFFER_SIZE (sizeof(BS_MESSAGE_BUFFER))


// *****************************************************************************
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //BSC_USART_COMMON_H
