/**
 * 
 */

#ifndef SPI_CALLBACKS_H_
    #define SPI_CALLBACKS_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stm32g4xx_hal.h>

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Redefinition of the HAL SPI transmit complete callback function.
 * Used to handle the completion of SPI1 DMA transmission and to manage the transaction buffer accordingly.
 * @param hspi Pointer to the SPI handle.
 * @return None
 */
extern void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef*hspi);

/**
 * @brief Redefinition of the HAL SPI receive complete callback function.
 * Used to handle the completion of SPI1 DMA reception and to manage the transaction buffer accordingly.
 * @param hspi Pointer to the SPI handle.
 * @return None
 */
extern void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef*hspi);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // SPI_CALLBACKS_H_