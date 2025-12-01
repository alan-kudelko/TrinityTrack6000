/**
 * @file USART1_Dma.c
 * @brief Implementation of USART1 with DMA functionality and dedicated ring buffer.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <USART1_Dma.h>

#include <stdbool.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Pinout.h>

uint8_t huart1_dma_rx_buffer[UART1_DMA_RX_BUFFER_SIZE]={0};
uint8_t huart1_dma_tx_buffer[UART1_DMA_TX_BUFFER_SIZE]={0};

volatile uint8_t huart1_dma_rx_ring_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
volatile uint8_t huart1_dma_tx_ring_buffer[UART1_DMA_TX_RING_BUFFER_SIZE]={0};

volatile uint16_t huart1_dma_tx_buffer_length=0;

volatile uint16_t huart1_dma_rx_ring_buffer_head=0;
volatile uint16_t huart1_dma_rx_ring_buffer_tail=0;
volatile uint16_t huart1_dma_rx_ring_buffer_length=0;

volatile uint16_t huart1_dma_tx_ring_buffer_head=0;
volatile uint16_t huart1_dma_tx_ring_buffer_tail=0;
volatile uint16_t huart1_dma_tx_ring_buffer_length=0;

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

bool usart1_dma_enq_data(const uint8_t*data,const uint16_t length){
    // Check if there is enough space in the transmit ring buffer
    uint16_t free_space=0;
    if(huart1_dma_tx_ring_buffer_head>=huart1_dma_tx_ring_buffer_tail){
        free_space=UART1_DMA_TX_RING_BUFFER_SIZE-(huart1_dma_tx_ring_buffer_head-huart1_dma_tx_ring_buffer_tail);
    }
    else{
        free_space=huart1_dma_tx_ring_buffer_tail-huart1_dma_tx_ring_buffer_head;
    }

    if(free_space>length){
        // There is enough space, enqueue the data
        __disable_irq();
        for(uint16_t i=0;i<length;i++){
            huart1_dma_tx_ring_buffer[huart1_dma_tx_ring_buffer_head]=data[i];
            huart1_dma_tx_ring_buffer_head=(huart1_dma_tx_ring_buffer_head+1)%UART1_DMA_TX_RING_BUFFER_SIZE;
        }
        huart1_dma_tx_ring_buffer_length+=length;
        // If DMA is not active, copy data to the DMA buffer and start transmission
        if(HAL_UART_GetState(&huart1)!=HAL_UART_STATE_BUSY_TX){
            // Check if data will fit into DMA buffer
            usart1_dma_copy_to_tx_buffer(huart1_dma_tx_buffer);
            HAL_UART_Transmit_DMA(&huart1,huart1_dma_tx_buffer,huart1_dma_tx_buffer_length);
        }
        __enable_irq();
        // If DMA is active, data will be sent upon DMA completion interrupt
        return true;
    }
    else{
        // Not enough space
        // Intented to use with RTOS where caller can retry later - when space is available
        return false;
    }

    return false;
}

void usart1_dma_copy_to_tx_buffer(uint8_t*dst){
    // Copy data from the transmit ring buffer to the provided destination DMA buffer
    // Remember to update ring buffer tail and length accordingly
    uint16_t bytes_to_copy=huart1_dma_tx_ring_buffer_length;
    // Crop to DMA buffer size
    if(bytes_to_copy>UART1_DMA_TX_BUFFER_SIZE){
        bytes_to_copy=UART1_DMA_TX_BUFFER_SIZE;
    }
    // Copy data
    uint16_t i=0;
    for(i=0;i<bytes_to_copy;i++){
        dst[i]=huart1_dma_tx_ring_buffer[huart1_dma_tx_ring_buffer_tail];
        huart1_dma_tx_ring_buffer_tail=(huart1_dma_tx_ring_buffer_tail+1)%UART1_DMA_TX_RING_BUFFER_SIZE;
    }
    // Update ring buffer tail and length
    huart1_dma_tx_buffer_length=i;
    huart1_dma_tx_ring_buffer_length-=i;
}

void usart1_dma_tx_complete(void){
    if(huart1_dma_tx_ring_buffer_length>0){
        // More data to send
        usart1_dma_copy_to_tx_buffer(huart1_dma_tx_buffer);
        HAL_UART_Transmit_DMA(&huart1,huart1_dma_tx_buffer,huart1_dma_tx_buffer_length);
    }
}

void usart1_dma_rx_init(void){
    // Initialize DMA for USART1 RX
    // Start the first DMA reception
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,huart1_dma_rx_buffer,UART1_DMA_RX_BUFFER_SIZE);
    // Check HAL status
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
}

bool usart1_dma_read_data(uint8_t*dst,uint8_t*length,const uint16_t maxLength){
    // Disable only USART1 and DMA1_Channel7 interrupts to avoid inconsistencies during buffer update
    __NVIC_DisableIRQ(USART1_IRQn);
    __NVIC_DisableIRQ(DMA1_Channel7_IRQn);
    // Check if there is data available in the receive ring buffer
    if(huart1_dma_rx_ring_buffer_length==0){
        // No data available
        *length=0;
        __NVIC_EnableIRQ(USART1_IRQn);
        __NVIC_EnableIRQ(DMA1_Channel7_IRQn);
        return false;
    }
    // Determine how much data to read
    uint16_t i=0;
    uint16_t new_tail=huart1_dma_rx_ring_buffer_tail;
    for(;i<huart1_dma_rx_ring_buffer_length;i++){
        // Iterate until /r or /n is found or maxLength is reached
    }
    // Re-enable USART1 and DMA1_Channel7 interrupts
    __NVIC_EnableIRQ(USART1_IRQn);
    __NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    return false;
}

void usart1_dma_copy_from_rx_buffer(const uint16_t dma_transfer_size){
    // Copy data from the receive ring
    // Called in callback when data transfer is complete
    // Called inside usart1_dma_rx_complete function
    for(uint16_t i=0;i<dma_transfer_size;i++){
        huart1_dma_rx_ring_buffer[huart1_dma_rx_ring_buffer_head]=huart1_dma_rx_buffer[i];
        huart1_dma_rx_ring_buffer_head=(huart1_dma_rx_ring_buffer_head+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
    }
    huart1_dma_rx_ring_buffer_length+=dma_transfer_size;
}

void usart1_dma_rx_complete(const uint16_t dma_transfer_size){
    // For now a debug diode
    // Called inside HAL_UARTEx_RxEventCallback when data reception is complete
    HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
    if((huart1_dma_rx_ring_buffer_length+dma_transfer_size)>UART1_DMA_RX_RING_BUFFER_SIZE){
        // Overflow - reset buffer
        // Don't copy new data
        return;
    }
    usart1_dma_copy_from_rx_buffer(dma_transfer_size);
    // Restart DMA reception
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,huart1_dma_rx_buffer,UART1_DMA_RX_BUFFER_SIZE);
    // Check HAL status
}