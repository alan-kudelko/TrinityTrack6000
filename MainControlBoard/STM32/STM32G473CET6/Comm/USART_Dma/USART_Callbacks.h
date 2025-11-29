/**
 * @file USART_Callbacks.h
 * @brief Header for USART DMA Callbacks.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef USART_CALLBACKS_H_
    #define USART_CALLBACKS_H_

#include <stm32g4xx_hal.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

extern void HAL_UART_TxCpltCallback(UART_HandleTypeDef*huart);

extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef*huart,uint16_t size);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // USART_CALLBACKS_H_