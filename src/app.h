#ifndef _APP_H
#define _APP_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,
    APP_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} APP_STATES;

// *****************************************************************************
typedef enum
{
    DRV_USART_INDEX_MASTER,
    DRV_USART_INDEX_SLAVE0,
    DRV_USART_INDEX_SLAVE1,
    DRV_USART_INDEX_MAX
} DRV_USART_INDEX;

// *****************************************************************************
#define NUMBER_OF_SLAVES 2

#define DMAC_CHANNEL_NONE -1
#define SERCOM0_TE_Set
#define SERCOM0_TE_Clear
#define SERCOM0_DMAC_TX_CHANNEL -1
#define SERCOM0_DMAC_RX_CHANNEL -1

//MASTER
#define SERCOM1_TE_Set MASTER_TE_Set();
#define SERCOM1_TE_Clear MASTER_TE_Clear();
#define SERCOM1_DMAC_TX_CHANNEL DMAC_CHANNEL_0
#define SERCOM1_DMAC_RX_CHANNEL DMAC_CHANNEL_3

#define SERCOM2_TE_Set
#define SERCOM2_TE_Clear
#define SERCOM2_DMAC_TX_CHANNEL -1
#define SERCOM2_DMAC_RX_CHANNEL -1

#define SERCOM3_TE_Set
#define SERCOM3_TE_Clear
#define SERCOM3_DMAC_TX_CHANNEL -1
#define SERCOM3_DMAC_RX_CHANNEL -1

//SLAVE1
#define SERCOM4_TE_Set SLAVE1_TE_Set();
#define SERCOM4_TE_Clear SLAVE1_TE_Clear();
#define SERCOM4_DMAC_TX_CHANNEL DMAC_CHANNEL_2
#define SERCOM4_DMAC_RX_CHANNEL DMAC_CHANNEL_5

//SLAVE0
#define SERCOM5_TE_Set SLAVE0_TE_Set();
#define SERCOM5_TE_Clear SLAVE0_TE_Clear();
#define SERCOM5_DMAC_TX_CHANNEL DMAC_CHANNEL_1
#define SERCOM5_DMAC_RX_CHANNEL DMAC_CHANNEL_4

#define USART_SIGNAL_TX_COMPLETE (1 << 0)
#define USART_SIGNAL_RX_COMPLETE (1 << 1)
#define USART_SIGNAL_ERROR_FLAG  (1 << 3)


#define SLAVE0_ADDRESS  0x1
#define SLAVE1_ADDRESS  0x2

#define MASTER_DATA " -Master\0"
#define SLAVE0_DATA " -Slave0\0"
#define SLAVE1_DATA " -Slave1\0"


typedef enum
{
    MY_USART_PACKET_STATE_IDLE = 0,
    MY_USART_PACKET_WAIT_START_BYTE,     //must be MY_USART_PACKET_START_BYTE
    MY_USART_PACKET_WAIT_ADDRESS_BYTE,   //will never be MY_USART_PACKET_START_BYTE
    MY_USART_PACKET_WAIT_LENGTH_BYTE,    //will never be MY_USART_PACKET_START_BYTE, start dmac transfer
    MY_USART_PACKET_WAIT_DATA,           //DMA transfer in progress
    MY_USART_PACKET_COMPLETE,            //DMA transfer complete
} MY_USART_PACKET_STATE;

typedef struct

{
    DRV_USART_INDEX index;
    BSC_USART_OBJECT *bsc_usart_obj; // pointer to the BSC USART object
    BS_MESSAGE_BUFFER tx_buffer;
    BS_MESSAGE_BUFFER rx_buffer;
    TaskHandle_t task_handle;
} MY_USART_OBJ;


#define USART_0   (&usartObjs[0])
#define USART_1   (&usartObjs[1])

// *****************************************************************************
typedef struct
{
    /* The application's current state */
    APP_STATES state;

    /* TODO: Define any additional data used by the application. */

} APP_DATA;



// *****************************************************************************

void begin_read(MY_USART_OBJ *p_usart_obj);
void block_write(MY_USART_OBJ *p_usart_obj);
void block_rx_ready(MY_USART_OBJ *p_usart_obj);

void APP_Initialize(void);
void APP_Tasks(void);

// *****************************************************************************
extern MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX];


#ifdef __cplusplus
}
#endif

#endif /* _APP_H */

