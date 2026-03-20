/**
 * @defgroup SPI1_DMA SPI1 LL Driver
 * @brief SPI1 low level driver with DMA functionality with dedicated ring buffer
 * This module is designed only as a transport layer for SPI1 communication.
 * In order to provide high efficiency and low latency called has to provide all the necessary
 * parameters for SPI1 transaction, including pointers to the data to be transmitted and received, lengths of the data,
 * GPIO port and pin for slave selection, and minimal delay after transmission. This allows the driver to handle all the necessary
 * steps for SPI1 communication, including slave selection and timing, without requiring the user to manage
 * these details, which can be crucial for achieving optimal performance in a low-level driver.
 *  
 * @note This module uses modified SPI1_IRQHandler in stm32g4xx_it.c to handle SPI1 rx interrupts.
 * 
 * Caller's responsibilities:
 * - Ensure that the data buffers provided for transmission and reception are valid and properly sized.
 * - Ensure that the GPIO port and pin specified for slave selection are correctly configured and do not conflict with other peripherals.
 * - Handle any necessary synchronization or mutual exclusion if multiple tasks or interrupts may access the SPI1
 * driver concurrently, to prevent data corruption or conflicts in SPI1 transactions.
 * - Ensure that the SPI1 DMA is properly initialized by calling `spi1_dma_init()` before attempting to enqueue any data for transmission.
 * - Monitor the `hspi1_dma_active` flag to determine when SPI1 DMA is busy with a transaction
 * and to avoid queuing new transactions while the DMA is active, unless the driver is designed to handle multiple queued transactions.
 * 
 * @warning transmitted data should not be modified until transmission is complete and `hspi1_dma_active` flag is cleared
 * to prevent data corruption. It is the caller's responsibility to ensure that the data buffers remain valid and unchanged
 * until the SPI1 DMA transaction is complete, as modifying the data while it is being transmitted can lead to unpredictable
 * behavior and potential data corruption. The caller should also ensure that any necessary synchronization mechanisms are in place
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

 #ifndef SPI1_DMA_H_
    #define SPI1_DMA_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdint.h>
#include <stdbool.h>
#include <stm32g4xx_hal.h>

#define SPI1_HSPI_DATA_BUFFER_SIZE 100 //!< Size of the buffer for storing SPI1 transaction parameters, can be adjusted as needed

/**
 * @brief Structure to hold SPI1 transaction parameters
 * This structure is used to store the parameters for an SPI1 transaction, including pointers to the
 * data to be transmitted and received, lengths of the data, GPIO port and pin for slave selection, and minimal delay after transmission.
 * @note It is important to ensure that the structure is designed to be efficient for use in a low-level driver
 * and that it is stored in a memory region that allows for fast access, such as CCSRAM, to optimize the performance of SPI1 DMA operations.
 */

 // Optimize field and therefore structure size in the memory
typedef struct{
    uint8_t*txBuffer; /**< Pointer to the buffer containing data to be transmitted, should be allocated by the caller and have sufficient size to hold the data to be sent */
    uint8_t*rxBuffer; /**< Pointer to the buffer where received data will be stored, should be allocated by the caller and have sufficient size to hold the expected received data */
    uint16_t rxLength; /**< Length of the data to be received in bytes */
    uint16_t txLength; /**< Length of the data to be transmitted in bytes */
    uint16_t gpio_pin;  /**< GPIO pin number for slave selection */
    GPIO_TypeDef*gpio_port; /**< GPIO port for slave selection */
    uint8_t flags; /**< Flags for additional options */
    uint8_t callbackEvent; /** Add doxy */
    void(*callbackFn)(uint8_t event); /**< Callback function called when transmission is finished */
}hspi_data;

#define HSPI_FLAG_HALF_DUPLEX 0x00 //!< Flag indicating that the transaction is half-duplex, meaning that only transmission or reception will occur, but not both
#define HSPI_FLAG_FULL_DUPLEX 0x01 //!< Flag indicating that the transaction is full-duplex, meaning that both transmission and reception will occur at the same time

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Initializes DMA for SPI1 transmission and reception.
 * @note This function must be called before using any other functions in this module to set up the DMA for SPI1.
 * It sets up the necessary buffers and prepares SPI1 for DMA operations.
 * @warning This function should be called only once during system initialization.
 */
extern void spi1_dma_init(void);

/**
 * @brief Enqueues an SPI1 transaction for DMA transmission.
 * @param transactionData Pointer to a structure containing the parameters for the SPI1 transaction, including
 * pointer to the data to be enqueued for transmission provided by the caller
 * @return true if the transaction was successfully enqueued, false if the transaction buffer is full and the transaction cannot be enqueued at this time
 * @note This function does not block; it queues the transaction for DMA transmission.
 * The caller should check the return value to determine if the transaction was successfully enqueued and may need to retry if the buffer is full.
 */
extern bool spi1_dma_enq_data(hspi_data*transactionData);

/**
 * @brief Internal function to start SPI1 DMA transmission.
 * @param None
 * @return 0 if the transmission was successfully started, non-zero error code otherwise.
 * @note This function is called internally by the driver to initiate the SPI1 DMA transmission based on the parameters of the transaction at the head of the transaction buffer.
 */
extern uint8_t spi1_send_data(void);

/**
 * @brief Callback function called when SPI1 DMA transmission is complete.
 * @param None
 * @return None
 * @note This function is called from the HAL_SPI_TxCpltCallback when SPI1
 * transmission is complete. It checks if there is more data in the transaction buffer and initiates another DMA transmission
 * if needed along with updating the transaction buffer pointers and length variable.
 */
extern void spi1_dma_tx_complete(void);

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // SPI1_DMA_H_

/**@} */