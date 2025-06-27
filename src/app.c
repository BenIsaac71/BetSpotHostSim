#include "definitions.h"
#include "sys_tasks.h"
#include "app.h"
#include "slave.h"
#include "stdio.h"

// *****************************************************************************
MY_USART_OBJ usart_objs[DRV_USART_INDEX_MAX] =
{
    [DRV_USART_INDEX_MASTER] =
    {
        .index = DRV_USART_INDEX_MASTER,
        .address = MASTER_ADDRESS,
        .sercom_regs = DRV_USART_SERCOM_REGISTERS_MASTER,
        .dmac_channel_tx = USART_DMA_CHANNEL_MASTER_TX,
        .dmac_channel_rx = USART_DMA_CHANNEL_MASTER_RX,
        .tx_buffer = {.to_addr = GLOBAL_ADDRESS, .from_addr = MASTER_ADDRESS, .data = MASTER_DATA},
    },
    [DRV_USART_INDEX_SLAVE0] = {
        .index = DRV_USART_INDEX_SLAVE0,
        .address = DRV_USART_INDEX_SLAVE0,
        .sercom_regs = DRV_USART_SERCOM_REGISTERS_SLAVE0,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE0_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE0_TX,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .from_addr = DRV_USART_INDEX_SLAVE0, .data = SLAVE0_DATA}
    },
    [DRV_USART_INDEX_SLAVE1] = {
        .index = DRV_USART_INDEX_SLAVE1,
        .address = DRV_USART_INDEX_SLAVE1,
        .sercom_regs = DRV_USART_SERCOM_REGISTERS_SLAVE1,
        .dmac_channel_rx = USART_DMA_CHANNEL_SLAVE1_RX,
        .dmac_channel_tx = USART_DMA_CHANNEL_SLAVE1_TX,
        .tx_buffer = {.to_addr = MASTER_ADDRESS, .from_addr = DRV_USART_INDEX_SLAVE1, .data = SLAVE1_DATA}
    }
};

APP_DATA appData;

// *****************************************************************************
void USART_TX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
void USART_RX_DMA_Callback(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);
void USART_TE_Clear(DRV_USART_INDEX index);
void prepare_to_receive_message(MY_USART_OBJ *p_usart_obj);


// *****************************************************************************
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */

    SLAVE_Initialize();
    MY_USART_OBJ *p_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_rx, (DMAC_CHANNEL_CALLBACK)USART_RX_DMA_Callback, (uintptr_t)p_usart_obj);
    DMAC_ChannelCallbackRegister(p_usart_obj->dmac_channel_tx, (DMAC_CHANNEL_CALLBACK)USART_TX_DMA_Callback, (uintptr_t)p_usart_obj);
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
    //if message is not for us, we don't signal the task and restart the RX DMA transfer
    if (p_usart_obj->rx_buffer.to_addr != p_usart_obj->address && p_usart_obj->rx_buffer.to_addr != GLOBAL_ADDRESS)
    {
        printf("r%d", p_usart_obj->index);
//        return;
    }

    xTaskNotifyFromISR(p_usart_obj->task_handle, ulValue, eSetBits, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        // If a higher priority task was woken, yield to it
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
    if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_rx, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, &p_usart_obj->rx_buffer, USART_BUFFER_SIZE) != true)
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
    LED_RED_Clear();

    //turn on the usart txc interrupt
    p_usart_obj->tx_buffer.to_addr = to_addr; // Set the destination address for the message
    if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_tx, &p_usart_obj->tx_buffer, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, USART_BUFFER_SIZE) != true)
    {
        printf("%d-> error\n", p_usart_obj->index);
        return;
    }
    //p_usart_obj->sercom_regs->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_TXC_Msk; // Enable TXC interrupt

    xTaskNotifyWait(0, USART_SIGNAL_COMPLETE_TX(p_usart_obj->index), &ulNotificationValue, OSAL_WAIT_FOREVER);

    //until we fix the TXC interrupt, we just have to poll the TXC before continuing because of half duplex
    while ((p_usart_obj->sercom_regs->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_TXC_Msk) == 0);

    USART_TE_Clear(p_usart_obj->index); // Clear the TE line after transmission
    LED_RED_Set();

    printf("%dt\n", p_usart_obj->index);

    //just change the last byte of the data buffer string right before string terminator
    p_usart_obj->tx_buffer.data[DATA_BUFFER_SIZE - 2]++;
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
            if (msg->to_addr == p_usart_obj->address)
            {
                printf(" ME\n");
                return true;
            }
            else if (msg->to_addr == GLOBAL_ADDRESS)
            {
                printf(" G\n");
            }
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



// *****************************************************************************

void APP_Tasks ( void )
{
    MY_USART_OBJ *p_usart_obj = &usart_objs[DRV_USART_INDEX_MASTER];
    uint32_t ulNotificationValue;

    USART_TE_Set(p_usart_obj->index);//Driver needs a to warm up on startup
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to allow other tasks to initialize


    /* Check the application's current state. */
    switch ( appData.state )
    {
    /* Application's initial state. */
    case APP_STATE_INIT:
        usart_objs[DRV_USART_INDEX_MASTER].task_handle = xAPP_Tasks;
        //test code to run dma testing out of app task
        prepare_to_receive_message(&usart_objs[DRV_USART_INDEX_MASTER]);
        prepare_to_receive_message(&usart_objs[DRV_USART_INDEX_SLAVE0]);
        prepare_to_receive_message(&usart_objs[DRV_USART_INDEX_SLAVE1]);

        while (true)
        {
            for (int i = 0; i < DRV_USART_INDEX_MAX; i++)
            {
                MY_USART_OBJ *p_usart_obj = &usart_objs[i];
                transmit_message(p_usart_obj, MASTER_ADDRESS - 1); //transmit to no one

                if (xTaskNotifyWait(0,  0xFFFFFFFF, &ulNotificationValue, 5))
                {
                    for (int j = 0; j < DRV_USART_INDEX_MAX; j++)
                    {
                        if (ulNotificationValue & USART_SIGNAL_COMPLETE_RX(j))
                        {
                            printf("%d\n", j);
                        }
                    }
                }
            }
            portYIELD();
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
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_tx, &p_usart_obj->tx_buffer, (uint8_t * )&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, USART_BUFFER_SIZE) != true)
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
            if (DMAC_ChannelTransfer(p_usart_obj->dmac_channel_rx, (uint8_t *)&p_usart_obj->sercom_regs->USART_INT.SERCOM_DATA, &p_usart_obj->rx_buffer, USART_BUFFER_SIZE) != true)
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