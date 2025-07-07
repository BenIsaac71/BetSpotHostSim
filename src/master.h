#ifndef _MASTER_H
#define _MASTER_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    MASTER_STATE_INIT = 0,
    MASTER_STATE_SERVICE_TASKS,
} MASTER_STATES;


// *****************************************************************************
#define SLAVES_ADDRESS_START  0x1
#define NUMBER_OF_SLAVES 2
#define FAKE_CRC "\xFF\x00\x00\xFF"

SERCOM_DMA_MAP(1, DMAC_CHANNEL_0,    DMAC_CHANNEL_3)        // harmony assignment
SERCOM_DMA_MAP(4, DMAC_CHANNEL_2,    DMAC_CHANNEL_5)        // harmony assignment
SERCOM_DMA_MAP(5, DMAC_CHANNEL_1,    DMAC_CHANNEL_4)        // harmony assignment

#define MASTER_DATA_STR "A-Master\0"
#define SLAVE0_DATA_STR "a-Slave0\0"
#define SLAVE1_DATA_STR "a-Slave1\0"

// *****************************************************************************
typedef struct
{
    TaskHandle_t xTaskHandle;
    MY_USART_OBJ my_usart_obj;
    MASTER_STATES state;
} MASTER_DATA;

// *****************************************************************************
void begin_read(MY_USART_OBJ *p_usart_obj);
void begin_write(MY_USART_OBJ *p_usart_obj);
void block_read(MY_USART_OBJ *p_usart_obj);
void rx_callback(MY_USART_OBJ *p_usart_obj);

void MASTER_Initialize(void);


#ifdef __cplusplus
}
#endif

#endif /* _MASTER_H */

