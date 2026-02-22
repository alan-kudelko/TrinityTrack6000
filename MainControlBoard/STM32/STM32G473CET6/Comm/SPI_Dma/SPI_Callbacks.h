/**
 * @file SPI_Callbacks.h
 * @brief Header for SPI DMA Callbacks.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef SPI_CALLBACKS_H_
    #define SPI_CALLBACKS_H_

#include <stm32g4xx_hal.h>

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

extern void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef*hspi);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // SPI_CALLBACKS_H_