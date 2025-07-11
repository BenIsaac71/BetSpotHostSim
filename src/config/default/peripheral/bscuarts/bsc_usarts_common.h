#ifndef BSC_USART_COMMON_H // Guards against multiple inclusion
#define BSC_USART_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "device.h"
#include "peripheral/sercom/usart/plib_sercom_usart_common.h"
#include "peripheral/dmac/plib_dmac.h"
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef __cplusplus // Provide C++ Compatibility
extern "C" {
#endif

// *****************************************************************************
typedef enum
{
    BSC_USART_SERCOM1,
    BSC_USART_SERCOM4,
    BSC_USART_SERCOM5,
    BSC_USART_SERCOM_MAX
} BSC_USART_SERCOM_ID;

// *****************************************************************************
typedef enum
{
    BSC_USART_BANK_A,
    BSC_USART_BANK_B,
    BSC_USART_BANK_MAX
} BSC_USART_BANK;

// *****************************************************************************
typedef void (*TE_PIN_FUNCTION)(void);

// *****************************************************************************
typedef struct
{
    BSC_USART_SERCOM_ID                 bsc_usart_id;

    TE_PIN_FUNCTION                     te_set;

    TE_PIN_FUNCTION                     te_clr;

    DMAC_CHANNEL                        dmac_channel_tx;

    DMAC_CHANNEL                        dmac_channel_rx;

    uint8_t                             address;

    sercom_registers_t                 *sercom_regs;

    uint32_t                            peripheral_clk_freq;

    void                               *txBuffer;

    size_t                              txSize;

    size_t                              txProcessedSize;

    SERCOM_USART_CALLBACK               txCallback;

    uintptr_t                           txContext;

    bool                                txBusyStatus;

    void                               *rxBuffer;

    size_t                              rxSize;

    size_t                              rxProcessedSize;

    SERCOM_USART_CALLBACK               rxCallback;

    uintptr_t                           rxContext;

    bool                                rxBusyStatus;

    USART_ERROR                         errorStatus;

    SemaphoreHandle_t                   rx_semaphore;

    SemaphoreHandle_t                   tx_semaphore;

} BSC_USART_OBJECT;




// *****************************************************************************
// Used by BSC_USART_OBJECT_INIT to map SERCOM_NUM to SERCOM DMAC TX amd RX channels assigned in mplabx
#define SERCOM_DMA_MAP(SERCOM_NUM, TX_CH, RX_CH)           \
    enum { SERCOM##SERCOM_NUM##_DMAC_TX_CHANNEL = TX_CH, SERCOM##SERCOM_NUM##_DMAC_RX_CHANNEL = RX_CH };

// *****************************************************************************
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //BSC_USART_COMMON_H
