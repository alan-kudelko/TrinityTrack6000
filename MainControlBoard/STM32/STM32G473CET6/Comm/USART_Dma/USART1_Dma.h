/**
 * @defgroup USART1_DMA USART1 LL Driver
 * @brief USART1 low level driver with DMA functionality with dedicated ring buffer
 * @note This module uses modified USART1_IRQHandler in stm32g4xx_it.c to handle USART1 rx interrupts.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 * 
 * @{
 */

#ifndef USART1_DMA_H_
    #define USART1_DMA_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdint.h>
#include <stdbool.h>

#include <tx_api.h>

/**
 * @brief USART1 DMA Buffers Configuration
 * @defgroup DMA_Buffers_Config DMA Buffers Configuration
 * @{
 */

#define UART1_DMA_RX_BUFFER_SIZE 128 //!< Size of the buffer for storing received data from USART1, can be adjusted as needed
#define UART1_DMA_TX_BUFFER_SIZE 128 //!< Size of the buffer for storing data to be transmitted over USART1, can be adjusted as needed

#define UART1_DMA_RX_RING_BUFFER_SIZE 512 //!< Size of the ring buffer for storing received data from USART1, can be adjusted as needed
#define UART1_DMA_TX_RING_BUFFER_SIZE 512 //!< Size of the ring buffer for storing data to be transmitted over USART1, can be adjusted as needed

#define UART1_DMA_RX_R_CHAR '\r' //!< Character indicating end of command in received data, can be adjusted as needed
#define UART1_DMA_RX_N_CHAR '\n' //!< Character indicating end of command in received data, can be adjusted as needed

#define UART1_DMA_RX_NO_COMMAND 0 //!< Value indicating that no complete command was found in the received data, can be adjusted as needed
#define UART1_DMA_RX_R_FOUND 1 //!< Value indicating that a \r character was found in the received data, can be adjusted as needed
#define UART1_DMA_RX_N_FOUND 2 //!< Value indicating that a \n character was found in the received data, can be adjusted as needed

 /** @} */

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Initializes USART1 DMA functionality, ring buffers and variables.
 * @note This function must be called before using any other USART1 DMA functions.
 * It sets up the necessary buffers and prepares USART1 for DMA operations.
 * @warning This function should be called only once during system initialization.
 */
extern void usart1_dma_init(void);

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

/**
 * @brief Initializes DMA for USART1 reception.
 * @note This function sets up the DMA to start receiving data into the RX DMA buffer.
 */
extern void usart1_dma_rx_init(void);

/**
 * @brief Reads data from the USART1 receive ring buffer into the provided destination buffer.
 * @param dst Pointer to the destination buffer where received data will be copied.
 * @param length Pointer to a variable where the length of the copied data will be stored.
 * @param maxLength Maximum length of data to be read in bytes.
 * @return true if data was successfully read, false if no complete command was available.
 * @note This function copies data until reaching a \r, \n or \r\n sequence or until maxLength is reached.
 * @warning This function is dedicated to process debug terminal incoming data.
 */
extern bool usart1_dma_read_data(uint8_t*dst,uint16_t*length,const uint16_t maxLength);

/**
 * @brief Callback function called when USART1 DMA reception is complete.
 * @param None
 * @note This function is called from the USART1_IRQHandler when USART1 reception is complete.
 * @warning This function is intended for internal use only.
 */
extern void usart1_dma_rx_complete(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // USART1_DMA_H_

/**@} */