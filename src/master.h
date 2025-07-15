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
#define MASTER_SERCOM_ID  BSC_USART_SERCOM1_ID
#define SLAVE_SERCOM_ID_START BSC_USART_SERCOM4_ID
#define NUMBER_OF_SLAVES 2

SERCOM_DMA_MAP(1, DMAC_CHANNEL_4,    DMAC_CHANNEL_5)        // harmony assignment
SERCOM_DMA_MAP(4, DMAC_CHANNEL_6,    DMAC_CHANNEL_7)        // harmony assignment
SERCOM_DMA_MAP(5, DMAC_CHANNEL_8,    DMAC_CHANNEL_9)        // harmony assignment

#define MASTER_TEST_DATA {0x00,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0}
#define SIZE_OF_TEST_DATA 5
// *****************************************************************************
typedef struct
{
    TaskHandle_t xTaskHandle;
    BSC_USART_OBJECT *p_usart_obj; // pointer to the BSC USART object
    BS_MESSAGE_BUFFER tx_buffer;
    BS_MESSAGE_BUFFER rx_buffer;
    MASTER_STATES state;
} MASTER_DATA;

// *****************************************************************************
void print_hex_data(const void *data, size_t size);
void print_color(color_t *color);
void print_tx_buffer(BS_MESSAGE_BUFFER *p_tx_buf);
void print_rx_buffer(BS_MESSAGE_BUFFER *p_rx_buf);

USART_ERROR block_read(BSC_USART_OBJECT *p_usart_obj);

void begin_read(BSC_USART_OBJECT* p_usart_obj, BS_MESSAGE_BUFFER *p_rsp_buf);
void begin_write(BSC_USART_OBJECT *p_usart_obj, BS_MESSAGE_BUFFER *p_tx_buf);
void rx_callback(BSC_USART_OBJECT *p_usart_obj);
void tx_callback(BSC_USART_OBJECT *p_usart_obj);
void build_packet(BS_MESSAGE_BUFFER *tx_buffer, BS_OP_t op, uint8_t to_addr, uint8_t from_addr, uint8_t *data, uint8_t data_len);
char *pcTaskGetCurrentTaskName(void);

void MASTER_Initialize(void);

extern QueueHandle_t master_message_queue;
extern QueueHandle_t master_response_queue;

#ifdef __cplusplus
}
#endif

#endif /* _MASTER_H */

