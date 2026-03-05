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
#include <TrinityTrack6000_Pinout.h>

//extern UART_HandleTypeDef huart2;

extern void usart1_dma_tx_complete(void);
//extern void usart2_dma_tx_complete(void);

extern void usart1_dma_rx_complete(uint16_t dma_transfer_size);
//extern void usart2_dma_rx_complete(void);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef*huart){
    if(huart->Instance==USART1){
        usart1_dma_tx_complete();
    }
    else if(huart->Instance==USART2){
        //usart2_dma_tx_complete();
    }
}

