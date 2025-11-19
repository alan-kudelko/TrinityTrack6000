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

uint8_t huart1_dma_rx_buffer[UART1_DMA_RX_BUFFER_SIZE]={0};
uint8_t huart1_dma_tx_buffer[UART1_DMA_TX_BUFFER_SIZE]={0};

uint8_t huart1_dma_rx_ring_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
uint8_t huart1_dma_tx_ring_buffer[UART1_DMA_TX_RING_BUFFER_SIZE]={0};

uint16_t huart1_dma_rx_ring_buffer_head=0;
uint16_t huart1_dma_rx_ring_buffer_tail=0;

uint16_t huart1_dma_tx_ring_buffer_head=0;
uint16_t huart1_dma_tx_ring_buffer_tail=0;

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
        for(uint16_t i=0;i<length;i++){
            huart1_dma_tx_ring_buffer[huart1_dma_tx_ring_buffer_head]=data[i];
            huart1_dma_tx_ring_buffer_head=(huart1_dma_tx_ring_buffer_head+1)%UART1_DMA_TX_RING_BUFFER_SIZE;
        }
        return true;
    }
    else{
        // Not enough space
        return false;
    }

    return false;
}