#include "app.h"
#include "sys_tasks.h"
#include "slave.h"

#define FAKE_CRC "\xFF\x00\x00\xFF"
// *****************************************************************************
MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX] =
{
    [DRV_USART_INDEX_MASTER] =
    {
        .index = DRV_USART_INDEX_MASTER,
        .dmac_channel_tx = USART_DMA_CHANNEL_MASTER_TX,
        .dmac_channel_rx = USART_DMA_CHANNEL_MASTER_RX,
        .tx_buffer = {.to_addr = GLOBAL_ADDRESS, .data_len = sizeof(MASTER_DATA) - 1,  .from_addr = MASTER_ADDRESS, .data = MASTER_DATA FAKE_CRC}
    },
    [DRV_USART_INDEX_SLAVE0] = {
        .index = DRV_USART_INDEX_SLAVE0,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE0_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE0_TX,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE0_DATA) - 1, .from_addr = SLAVE0_ADDRESS, .data = SLAVE0_DATA FAKE_CRC}
    },
    [DRV_USART_INDEX_SLAVE1] = {
        .index = DRV_USART_INDEX_SLAVE1,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE1_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE1_TX,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .data_len = sizeof(SLAVE1_DATA) - 1, .from_addr = SLAVE1_ADDRESS, .data = SLAVE1_DATA FAKE_CRC}
    }
};

APP_DATA appData;

// *****************************************************************************
void USART_TX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
void USART_RX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
void USART_TE_Clear(DRV_USART_INDEX index);
void prepare_to_receive_message(MY_USART_OBJ *p_usart_obj);


// *****************************************************************************
void APP_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */

    //SLAVE_Initialize();
    // MY_USART_OBJ *p_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    // DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_rx, (DMAC_CHANNEL_CALLBACK)USART_RX_DMA_Callback, (uintptr_t)p_usart_obj);
    // DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_tx, (DMAC_CHANNEL_CALLBACK)USART_TX_DMA_Callback, (uintptr_t)p_usart_obj);
    //TODO create TXC callback rather clr te line in isr directly
}

// *****************************************************************************

void USART_TX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{

    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)contextHandle;

    //from isr signal app task to process the next transfer
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t ulValue = USART_SIGNAL_COMPLETE_TX(p_usart_obj->index);

    //printf("TX DMA transfer completed for USART index %d\n", p_usart_obj->index);
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        // Transfer compldeted successfully
    }
    else if (event == DMAC_TRANSFER_EVENT_ERROR)
    {
        ulValue |= USART_SIGNAL_ERROR_FLAG; // Set error bit
        // Handle transfer error
        printf("error DMA TX");
        return;
    }
    xTaskNotifyFromISR(p_usart_obj->task_handle, ulValue, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        // If a higher priority task was woken, yield to it
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// *****************************************************************************

void USART_RX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    MY_USART_OBJ *p_usart_obj = (MY_USART_OBJ *)contextHandle;
    prepare_to_receive_message(p_usart_obj);
    printf("R%d", p_usart_obj->index);

    //from isr signal app task to process the next transfer
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t ulValue = USART_SIGNAL_COMPLETE_RX(p_usart_obj->index);

    //printf("RX DMA transfer completed for USART index %d\n", p_usart_obj->index);
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        // Transfer completed successfully
    }
    else if (event == DMAC_TRANSFER_EVENT_ERROR)
    {
        ulValue |= USART_SIGNAL_ERROR_FLAG; // Set error bit
        // Handle transfer error
        printf("error DMA RX");
        return;
    }

    //signal task if its for our addres or global address
//    if ((p_usart_obj->rx_buffer.to_addr == p_usart_obj->address) || (p_usart_obj->rx_buffer.to_addr == GLOBAL_ADDRESS))
    {
        xTaskNotifyFromISR(p_usart_obj->task_handle, ulValue, eSetBits, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken == pdTRUE)
        {
            // If a higher priority task was woken, yield to it
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

// *****************************************************************************

void USART_TE_Set(DRV_USART_INDEX index)
{
    switch (index)
    {
    case DRV_USART_INDEX_MASTER:
        MASTER_TE_Set();
        break;
    case DRV_USART_INDEX_SLAVE0:
        SLAVE0_TE_Set();
        break;
    case DRV_USART_INDEX_SLAVE1:
        SLAVE1_TE_Set();
        break;
    default:
        // Handle error or unsupported index
        break;
    }
}

// *****************************************************************************

void USART_TE_Clear(DRV_USART_INDEX index)
{
    switch (index)
    {
    case DRV_USART_INDEX_MASTER:
        MASTER_TE_Clear();
        break;
    case DRV_USART_INDEX_SLAVE0:
        SLAVE0_TE_Clear();
        break;
    case DRV_USART_INDEX_SLAVE1:
        SLAVE1_TE_Clear();
        break;
    default:
        // Handle error or unsupported index
        break;
    }
}

// *****************************************************************************

void prepare_to_receive_message(MY_USART_OBJ *p_usart_obj)
{
    if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_rx, (uint8_t *)&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, &p_usart_obj->rx_buffer, BS_USART_BUFFER_SIZE) != true)
    {
        printf("%d<- error\n", p_usart_obj->index);
    }
}

// *****************************************************************************

void transmit_message(MY_USART_OBJ *p_usart_obj, int to_addr)
{
    uint32_t ulNotificationValue;
    printf(" T%d", p_usart_obj->index);

    USART_TE_Set(p_usart_obj->index);

    //turn on the usart txc interrupt
    p_usart_obj->tx_buffer.to_addr = to_addr; // Set the destination address for the message
    if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_tx, &p_usart_obj->tx_buffer, (uint8_t *)&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, BS_USART_BUFFER_SIZE) != true)
    {
        printf("%d-> error\n", p_usart_obj->index);
        return;
    }
    //p_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_TXC_Msk; // Enable TXC interrupt

    xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_TX(p_usart_obj->index), &ulNotificationValue, OSAL_WAIT_FOREVER);

    //until we fix the TXC interrupt, we just have to poll the TXC before continuing because of half duplex
    while ((p_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0);

    USART_TE_Clear(p_usart_obj->index); // Clear the TE line after transmission

    printf("%dt\n", p_usart_obj->index);

    //just change the last byte of the data buffer string right before string terminator
//    p_usart_obj->tx_buffer.data[DATA_BUFFER_SIZE - 2]++;
}


// *****************************************************************************

bool process_message(MY_USART_OBJ *p_usart_obj)
{
    uint32_t ulNotificationValue;
    xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_RX(p_usart_obj->index), &ulNotificationValue, 1);
    if (ulNotificationValue == USART_SIGNAL_COMPLETE_RX(p_usart_obj->index))
    {

        BS_MESSAGE_BUFFER *msg = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
        printf("%s: %d<-A%d[%s]\n", p_usart_obj->tx_buffer.data, msg->to_addr, msg->from_addr, msg->data);
        if (msg->from_addr == MASTER_ADDRESS)
        {
            // if (msg->to_addr == p_usart_obj->address)
            // {
            //     printf(" ME\n");
            //     return true;
            // }
            // else if (msg->to_addr == GLOBAL_ADDRESS)
            // {
            //     printf(" G\n");
            // }
        }
        else
        {
            printf(" !from master\n");
        }
    }
    return false;
}

// *****************************************************************************

bool process_response(MY_USART_OBJ *p_usart_obj)
{
    uint32_t ulNotificationValue;
    xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_RX(p_usart_obj->index), &ulNotificationValue, 5);
    if (ulNotificationValue == USART_SIGNAL_COMPLETE_RX(p_usart_obj->index))
    {
        BS_MESSAGE_BUFFER *rsp = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
        printf("%d<-A%d[%s]\n", rsp->to_addr, rsp->from_addr, rsp->data);
        return true;
    }
    return false;
}

void begin_read(MY_USART_OBJ *p_usart_obj)
{
    // Start the read operation
    BSC_USART_Read(p_usart_obj->bsc_usart_obj, &p_usart_obj->rx_buffer, sizeof(p_usart_obj->rx_buffer));
}

void block_write(MY_USART_OBJ *p_usart_obj)
{
    USART_TE_Set(p_usart_obj->index);

    BSC_USART_Write(p_usart_obj->bsc_usart_obj, &p_usart_obj->tx_buffer, p_usart_obj->tx_buffer.data_len + BS_MESSAGE_ADDITIONAL_SIZE);
    while (BSC_USART_WriteIsBusy(p_usart_obj->bsc_usart_obj)); // Wait for TX to complete

    USART_TE_Clear(p_usart_obj->index);
}

void block_rx_ready(MY_USART_OBJ *p_usart_obj)
{
    // Wait for RX to complete
    while (BSC_USART_ReadIsBusy(p_usart_obj->bsc_usart_obj)); // Wait for RX to complete
    printf("%d<-%d:%s\n", p_usart_obj->rx_buffer.to_addr, p_usart_obj->rx_buffer.from_addr, (char *)p_usart_obj->rx_buffer.data);
}


void APP_Tasks(void)
{
    MY_USART_OBJ *p_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    uint32_t ulNotificationValue;

    // Initialize the USART objects bsc usart object
    usart_objs[DRV_USART_INDEX_MASTER].bsc_usart_obj = BSC_USART_Initialize(DRV_BSC_USART_MASTER, MASTER_ADDRESS);

    usart_objs[DRV_USART_INDEX_SLAVE0].bsc_usart_obj = BSC_USART_Initialize(DRV_BSC_USART_SLAVE0, SLAVE0_ADDRESS);
    usart_objs[DRV_USART_INDEX_SLAVE1].bsc_usart_obj = BSC_USART_Initialize(DRV_BSC_USART_SLAVE1, SLAVE1_ADDRESS);


    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize

    char count = 'A';


    /* Check the application's current state. */
    switch (appData.state)
    {
    /* Application's initial state. */
    case APP_STATE_INIT:

        while (true)
        {
            printf("Transaction %c\n",count);
            for (int slave_index = DRV_USART_INDEX_SLAVE0; slave_index < DRV_USART_INDEX_SLAVE0 + NUMBER_OF_SLAVES; slave_index++)
            {
                // message from host to slaves
                begin_read(&usart_objs[slave_index]);

                usart_objs[DRV_USART_INDEX_MASTER].tx_buffer.data[0] = count;
                usart_objs[DRV_USART_INDEX_MASTER].tx_buffer.to_addr = slave_index;
                block_write(&usart_objs[DRV_USART_INDEX_MASTER]);

                block_rx_ready(&usart_objs[slave_index]);

                // response from slave to host
                begin_read(&usart_objs[DRV_USART_INDEX_MASTER]);

                usart_objs[slave_index].tx_buffer.data[0] = count;
                block_write(&usart_objs[slave_index]);

                block_rx_ready(&usart_objs[DRV_USART_INDEX_MASTER]);
            }
            count = (count < 'Z') ? (count + 1) : 'A';
            LED_GREEN_Toggle();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }

        while (true)
        {

            transmit_message(&usart_objs[DRV_USART_INDEX_MASTER], SLAVE0_ADDRESS); // Transmit to all slaves

            if (process_message(&usart_objs[DRV_USART_INDEX_SLAVE0]))
            {
                transmit_message(&usart_objs[DRV_USART_INDEX_SLAVE0], MASTER_ADDRESS);
            }
            if (process_message(&usart_objs[DRV_USART_INDEX_SLAVE1]))
            {
                transmit_message(&usart_objs[DRV_USART_INDEX_SLAVE0], MASTER_ADDRESS);
            }

            if (process_response(&usart_objs[DRV_USART_INDEX_MASTER]) == false)
            {
                printf("No response from slaves\n");
            }
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay to allow other tasks to initialize
        };

        appData.state = APP_STATE_SERVICE_TASKS;
        break;
    case APP_STATE_SERVICE_TASKS:
        for (int i = DRV_USART_INDEX_SLAVE0; i < DRV_USART_INDEX_SLAVE0 + NUMBER_OF_SLAVES; i++)
        {

            // transmit to slave
            printf("M->%d\n", i);
            p_usart_obj->tx_buffer.to_addr = i;
            USART_TE_Set(p_usart_obj->index);
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_tx, &p_usart_obj->tx_buffer, (uint8_t *)&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, BS_USART_BUFFER_SIZE) != true)
            {
                printf("M-> error\n");
                while (true)
                    ;
            }
            xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_TX(p_usart_obj->index), &ulNotificationValue, 100);
            if (ulNotificationValue == 0)
            {
                printf("M->timout\n");
                while (true)
                    ;
            }

            while ((p_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0);

            // receive from slave
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_rx, (uint8_t *)&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, &p_usart_obj->rx_buffer, BS_USART_BUFFER_SIZE) != true)
            {
                printf("M-> error\n");
            }
            xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_RX(p_usart_obj->index), &ulNotificationValue, 1000);
            if (ulNotificationValue == 0)
            {
                printf("M<-timout\n");
            }
            else
            {
                BS_MESSAGE_BUFFER *rsp = (BS_MESSAGE_BUFFER *)&p_usart_obj->rx_buffer;
                printf("M<-S%d[%s]\n", rsp->from_addr, rsp->data);
            }

            vTaskDelay(pdMS_TO_TICKS(10));
        }
        break;
    default:
        break;
    }
}