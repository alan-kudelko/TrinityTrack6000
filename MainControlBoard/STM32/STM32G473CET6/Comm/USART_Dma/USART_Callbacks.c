/**
 * @file USART_Callbacks.c
 * @brief Implementation of USART DMA Callbacks.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <USART_Callbacks.h>

extern void usart1_dma_tx_complete(void);
//extern void usart2_dma_tx_complete(void);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef*huart){
    if(huart->Instance==USART1){
        usart1_dma_tx_complete();
    }
    else if(huart->Instance==USART2){
        //usart2_dma_tx_complete();
    }
}