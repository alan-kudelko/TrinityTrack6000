#include <SPI_Callbacks.h>

extern void spi1_dma_tx_complete(void);
//extern void spi2_dma_tx_complete(void);
extern void spi1_dma_rx_complete(void);
//extern void spi2_dma_rx_complete(void);

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