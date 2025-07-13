#ifndef _MASTER_H
#define _MASTER_H

// *****************************************************************************
#include "definitions.h"
#include "bsc_usarts.h"
#include "bsc_protocol.h"

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

#define MASTER_DATA_STR "\x00\x55\x55\x55\x55\0"
#define SLAVE0_DATA_STR "\xFF\x11\x11\x11\x11\0"
#define SLAVE1_DATA_STR "\xFF\x22\x22\x22\x22\0"

// *****************************************************************************
typedef struct
{
    TaskHandle_t xTaskHandle;
    MY_USART_OBJ usart_obj;
    MASTER_STATES state;
} MASTER_DATA;

// *****************************************************************************
void print_hex_data(const void *data, size_t size);
void print_tx_buffer(const MY_USART_OBJ *p_usart_obj);
void print_rx_buffer(const MY_USART_OBJ *p_usart_obj);

void begin_read(MY_USART_OBJ *p_usart_obj);
void begin_write(MY_USART_OBJ *p_usart_obj);
void rx_callback(MY_USART_OBJ *p_usart_obj);
void tx_callback(MY_USART_OBJ *p_usart_obj);
void build_packet(BS_MESSAGE_BUFFER *tx_buffer, BS_OP_t op, uint8_t to_addr, uint8_t from_addr, char *data, uint8_t data_len);
char *pcTaskGetCurrentTaskName(void);

void MASTER_Initialize(void);

extern QueueHandle_t master_message_queue;
extern QueueHandle_t master_response_queue;

#ifdef __cplusplus
}
#endif

#endif /* _MASTER_H */

