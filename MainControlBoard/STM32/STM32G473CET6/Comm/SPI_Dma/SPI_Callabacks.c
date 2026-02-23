/**
 * @file SPI_Callbacks.c
 * @brief Implementation for SPI DMA Callbacks.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

 #include <SPI_Callbacks.h>

 extern void spi1_dma_tx_complete(void);

 void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef*hspi){
    if(hspi->Instance==SPI1){
        spi1_dma_tx_complete();
    }
    else if(hspi->Instance==SPI2){
        //spi2_dma_tx_complete();
    }
 }

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef*hspi){
    if(hspi->Instance==SPI1){
        spi1_dma_tx_complete();
    }
    else if(hspi->Instance==SPI2){
        
    }
}