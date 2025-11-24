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
extern uint8_t huart1_dma_tx_buffer[UART1_DMA_TX_BUFFER_SIZE];

extern volatile uint8_t huart1_dma_rx_ring_buffer[UART1_DMA_RX_RING_BUFFER_SIZE] __attribute((section(".dmaBuff")));
extern volatile uint8_t huart1_dma_tx_ring_buffer[UART1_DMA_TX_RING_BUFFER_SIZE] __attribute((section(".dmaBuff")));

extern volatile uint16_t huart1_dma_tx_buffer_length;

extern volatile uint16_t huart1_dma_rx_ring_buffer_head;
extern volatile uint16_t huart1_dma_rx_ring_buffer_tail;
extern volatile uint16_t huart1_dma_rx_ring_buffer_length;

extern volatile uint16_t huart1_dma_tx_ring_buffer_head;
extern volatile uint16_t huart1_dma_tx_ring_buffer_tail;
extern volatile uint16_t huart1_dma_tx_ring_buffer_length;

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Enqueues data to be sent over USART1 using DMA if the transmit ring buffer has enough space.
 * @param data Pointer to the data buffer to be sent.
 * @param length Length of the data to be sent in bytes.
 * @return true if the data was successfully queued for transmission, false otherwise.
 * @note This function does not block; it queues the data for DMA transmission.
 */
extern bool usart1_dma_enq_data(const uint8_t*data,const uint16_t length);

/**
 * @brief Copies data from the transmit ring buffer to the provided DMA buffer.
 * @param dst Pointer to the destination DMA buffer.
 * @note This function updates the ring buffer tail and length accordingly.
 * @warning This function is intended for internal use only.
 */
extern void usart1_dma_copy_to_tx_buffer(uint8_t*dst);

/**
 * @brief Callback function called when USART1 DMA transmission is complete.
 * @note This function is called from the HAL_UART_TxCpltCallback when USART1 transmission is complete.
 * It checks if there is more data in the transmit ring buffer
 * and initiates another DMA transmission if needed along with updating the ring buffer pointers and
 * dma_tx_buffer_length variable.
 * @warning This function is intended for internal use only.
 */
extern void usart1_dma_tx_complete(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // USART1_DMA_H_