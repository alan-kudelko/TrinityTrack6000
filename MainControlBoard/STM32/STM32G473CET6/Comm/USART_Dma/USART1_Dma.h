/**
 * @file USART1_Dma.h
 * @brief Header for USART1 with DMA functionality and dedicated ring buffer.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef USART1_DMA_H_
    #define USART1_DMA_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief USART1 DMA Buffers Configuration
 * @defgroup DMA_Buffers_Config DMA Buffers Configuration
 * @{
 */

 #define UART1_DMA_RX_BUFFER_SIZE 128
 #define UART1_DMA_TX_BUFFER_SIZE 128

 #define UART1_DMA_RX_RING_BUFFER_SIZE 512
 #define UART1_DMA_TX_RING_BUFFER_SIZE 512

 /** @} */

extern uint8_t huart1_dma_rx_buffer[UART1_DMA_RX_BUFFER_SIZE] __attribute((section(".dmaBuff")));
extern uint8_t huart1_dma_tx_buffer[UART1_DMA_TX_BUFFER_SIZE] __attribute((section(".dmaBuff")));

extern uint8_t huart1_dma_rx_ring_buffer[UART1_DMA_RX_RING_BUFFER_SIZE] __attribute((section(".dmaBuff")));
extern uint8_t huart1_dma_tx_ring_buffer[UART1_DMA_TX_RING_BUFFER_SIZE] __attribute((section(".dmaBuff")));

extern uint16_t huart1_dma_rx_ring_buffer_head;
extern uint16_t huart1_dma_rx_ring_buffer_tail;

extern uint16_t huart1_dma_tx_ring_buffer_head;
extern uint16_t huart1_dma_tx_ring_buffer_tail;

/**
 * @brief Enqueues data to be sent over USART1 using DMA if the transmit ring buffer has enough space.
 * @param data Pointer to the data buffer to be sent.
 * @param length Length of the data to be sent in bytes.
 * @return true if the data was successfully queued for transmission, false otherwise.
 * @note This function does not block; it queues the data for DMA transmission.
 */
extern bool usart1_dma_send_data(const uint8_t*data,const uint16_t length);

#endif // USART1_DMA_H_