/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#ifndef USE_EIDE
#pragma config BOD33_DIS = SET
#pragma config BOD33USERLEVEL = 0x1cU
#pragma config BOD33_ACTION = RESET
#pragma config BOD33_HYST = 0x2U
#pragma config NVMCTRL_BOOTPROT = 0
#pragma config NVMCTRL_SEESBLK = 0x0U
#pragma config NVMCTRL_SEEPSZ = 0x0U
#pragma config RAMECC_ECCDIS = SET
#pragma config WDT_ENABLE = CLEAR
#pragma config WDT_ALWAYSON = CLEAR
#pragma config WDT_PER = CYC8192
#pragma config WDT_WINDOW = CYC8192
#pragma config WDT_EWOFFSET = CYC8192
#pragma config WDT_WEN = CLEAR
#pragma config NVMCTRL_REGION_LOCKS = 0xffffffffU
#endif




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Following MISRA-C rules are deviated in the below code block */
/* MISRA C-2012 Rule 7.2 - Deviation record ID - H3_MISRAC_2012_R_7_2_DR_1 */
/* MISRA C-2012 Rule 11.1 - Deviation record ID - H3_MISRAC_2012_R_11_1_DR_1 */
/* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
/* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 2 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART2ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX2];


static const DRV_USART_PLIB_INTERFACE drvUsart2PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)SERCOM4_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ)SERCOM4_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)SERCOM4_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)SERCOM4_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)SERCOM4_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)SERCOM4_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE)SERCOM4_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)SERCOM4_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)SERCOM4_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)SERCOM4_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)SERCOM4_USART_SerialSetup
};

static const uint32_t drvUsart2remapDataWidth[] = { 0x5, 0x6, 0x7, 0x0, 0x1 };
static const uint32_t drvUsart2remapParity[] = { 0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
static const uint32_t drvUsart2remapStopBits[] = { 0x0, 0xFFFFFFFFU, 0x40 };
static const uint32_t drvUsart2remapError[] = { 0x4, 0x0, 0x2 };

static const DRV_USART_INIT drvUsart2InitData =
{
    .usartPlib = &drvUsart2PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX2,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART2ClientObjPool[0],


    .remapDataWidth = drvUsart2remapDataWidth,

    .remapParity = drvUsart2remapParity,

    .remapStopBits = drvUsart2remapStopBits,

    .remapError = drvUsart2remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 1 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART1ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX1];


static const DRV_USART_PLIB_INTERFACE drvUsart1PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)SERCOM5_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ)SERCOM5_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)SERCOM5_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)SERCOM5_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)SERCOM5_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)SERCOM5_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE)SERCOM5_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)SERCOM5_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)SERCOM5_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)SERCOM5_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)SERCOM5_USART_SerialSetup
};

static const uint32_t drvUsart1remapDataWidth[] = { 0x5, 0x6, 0x7, 0x0, 0x1 };
static const uint32_t drvUsart1remapParity[] = { 0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
static const uint32_t drvUsart1remapStopBits[] = { 0x0, 0xFFFFFFFFU, 0x40 };
static const uint32_t drvUsart1remapError[] = { 0x4, 0x0, 0x2 };

static const DRV_USART_INIT drvUsart1InitData =
{
    .usartPlib = &drvUsart1PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX1,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART1ClientObjPool[0],


    .remapDataWidth = drvUsart1remapDataWidth,

    .remapParity = drvUsart1remapParity,

    .remapStopBits = drvUsart1remapStopBits,

    .remapError = drvUsart1remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 0 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART0ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX0];


static const DRV_USART_PLIB_INTERFACE drvUsart0PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)SERCOM1_USART_ReadCallbackRegister,
    .read_t = (DRV_USART_PLIB_READ)SERCOM1_USART_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)SERCOM1_USART_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)SERCOM1_USART_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)SERCOM1_USART_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)SERCOM1_USART_WriteCallbackRegister,
    .write_t = (DRV_USART_PLIB_WRITE)SERCOM1_USART_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)SERCOM1_USART_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)SERCOM1_USART_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)SERCOM1_USART_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)SERCOM1_USART_SerialSetup
};

static const uint32_t drvUsart0remapDataWidth[] = { 0x5, 0x6, 0x7, 0x0, 0x1 };
static const uint32_t drvUsart0remapParity[] = { 0x2, 0x0, 0x80000, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU };
static const uint32_t drvUsart0remapStopBits[] = { 0x0, 0xFFFFFFFFU, 0x40 };
static const uint32_t drvUsart0remapError[] = { 0x4, 0x0, 0x2 };

static const DRV_USART_INIT drvUsart0InitData =
{
    .usartPlib = &drvUsart0PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX0,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART0ClientObjPool[0],


    .remapDataWidth = drvUsart0remapDataWidth,

    .remapParity = drvUsart0remapParity,

    .remapStopBits = drvUsart0remapStopBits,

    .remapError = drvUsart0remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
/******************************************************
 * USB Driver Initialization
 ******************************************************/
 
/*  When designing a Self-powered USB Device, the application should make sure
    that USB_DEVICE_Attach() function is called only when VBUS is actively powered.
    Therefore, the firmware needs some means to detect when the Host is powering 
    the VBUS. A 5V tolerant I/O pin can be connected to VBUS (through a resistor)
    and can be used to detect when VBUS is high or low. The application can specify
    a VBUS Detect function through the USB Driver Initialize data structure. 
    The USB device stack will periodically call this function. If the VBUS is 
    detected, the USB_DEVICE_EVENT_POWER_DETECTED event is generated. If the VBUS 
    is removed (i.e., the device is physically detached from Host), the USB stack 
    will generate the event USB_DEVICE_EVENT_POWER_REMOVED. The application should 
    call USB_DEVICE_Detach() when VBUS is removed. 
    
    The following are the steps to generate the VBUS_SENSE Function through MHC     
        1) Navigate to MHC->Tools->Pin Configuration and Configure the pin used 
           as VBUS_SENSE. Set this pin Function as "GPIO" and set as "Input". 
           Provide a custom name to the pin.
        2) Select the USB Driver Component in MHC Project Graph and enable the  
           "Enable VBUS Sense" Check-box.     
        3) Specify the custom name of the VBUS SENSE pin in the "VBUS SENSE Pin Name" box.  */ 
static DRV_USB_VBUS_LEVEL DRV_USBFSV1_VBUS_Comparator(void)
{
    DRV_USB_VBUS_LEVEL retVal = DRV_USB_VBUS_LEVEL_INVALID;
    
    if(1U == USB_VBUS_SENSE_Get())
    {
        retVal = DRV_USB_VBUS_LEVEL_VALID;
    }
    return (retVal);

}

static const DRV_USBFSV1_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = USB_OTHER_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource1 = USB_SOF_HSOF_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource2 = USB_TRCPT0_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource3 = USB_TRCPT1_IRQn,

    /* System module initialization */
    .moduleInit = {0},

    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBFSV1_OPMODE_DEVICE,

    /* USB Full Speed Operation */
    .operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .runInStandby = true,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_REGS,

    /* Function to check for VBus */
    .vbusComparator = DRV_USBFSV1_VBUS_Comparator

};




// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

static const SYS_CMD_INIT sysCmdInit =
{
    .moduleInit = {0},
    .consoleCmdIOParam = (uint8_t) SYS_CMD_SINGLE_CHARACTER_READ_CONSOLE_IO_PARAM,
	.consoleIndex = 0,
};

// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Instance 0 Initialization Data">


/* These buffers are passed to the USB CDC Function Driver */
static uint8_t CACHE_ALIGN sysConsole0USBCdcRdBuffer[SYS_CONSOLE_USB_CDC_READ_WRITE_BUFFER_SIZE];
static uint8_t CACHE_ALIGN sysConsole0USBCdcWrBuffer[SYS_CONSOLE_USB_CDC_READ_WRITE_BUFFER_SIZE];

/* These are the USB CDC Ring Buffers. Data received from USB layer are copied to these ring buffer. */
static uint8_t sysConsole0USBCdcRdRingBuffer[SYS_CONSOLE_USB_CDC_RD_BUFFER_SIZE_IDX0];
static uint8_t sysConsole0USBCdcWrRingBuffer[SYS_CONSOLE_USB_CDC_WR_BUFFER_SIZE_IDX0];

static const SYS_CONSOLE_USB_CDC_INIT_DATA sysConsole0USBCdcInitData =
{
    .cdcInstanceIndex           = 0,
    .cdcReadBuffer              = sysConsole0USBCdcRdBuffer,
    .cdcWriteBuffer             = sysConsole0USBCdcWrBuffer,
    .consoleReadBuffer          = sysConsole0USBCdcRdRingBuffer,
    .consoleWriteBuffer         = sysConsole0USBCdcWrRingBuffer,
    .consoleReadBufferSize      = SYS_CONSOLE_USB_CDC_RD_BUFFER_SIZE_IDX0,
    .consoleWriteBufferSize     = SYS_CONSOLE_USB_CDC_WR_BUFFER_SIZE_IDX0,
};

static const SYS_CONSOLE_INIT sysConsole0Init =
{
    .deviceInitData = (const void*)&sysConsole0USBCdcInitData,
    .consDevDesc = &sysConsoleUSBCdcDevDesc,
    .deviceIndex = 0,
};


// </editor-fold>




// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/* MISRAC 2012 deviation block end */

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

    NVMCTRL_Initialize( );

  
    PORT_Initialize();

    CLOCK_Initialize();




    SERCOM1_USART_Initialize();

    EVSYS_Initialize();

    SERCOM5_USART_Initialize();

    SERCOM4_USART_Initialize();


    /* MISRAC 2012 deviation block start */
    /* Following MISRA-C rules deviated in this block  */
    /* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    /* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */

    sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2, (SYS_MODULE_INIT *)&drvUsart2InitData);

    sysObj.drvUsart1 = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT *)&drvUsart1InitData);

    sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);


    sysObj.sysCommand = (uint32_t) SYS_CMD_Initialize((SYS_MODULE_INIT*)&sysCmdInit);

    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&sysConsole0Init);
   /* MISRAC 2012 deviation block end */


    /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);


    /* Initialize USB Driver */ 
    sysObj.drvUSBFSV1Object = DRV_USBFSV1_Initialize(DRV_USBFSV1_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);


    /* MISRAC 2012 deviation block end */
    APP_Initialize();


    NVIC_Initialize();


    /* MISRAC 2012 deviation block end */
}

/*******************************************************************************
 End of File
*/
