/**
 * @file SPI1_Dma.c
 * @brief Implementation of SPI1 with DMA functionality and dedicated ring buffer.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <SPI1_Dma.h>

#include <stdbool.h>
#include <string.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Pinout.h>

volatile hspi_data hspi1_transaction_buffer[SPI1_HSPI_DATA_BUFFER_SIZE];

volatile uint16_t hspi1_transaction_buffer_head=0;
volatile uint16_t hspi1_transaction_buffer_tail=0;
volatile uint16_t hspi1_transaction_buffer_length=0;

volatile bool hspi1_dma_active=false;
volatile bool hspi1_tx_processed=false;
// Decide if use of volatile is necessary for these variables
// This is a low level driver so it has to be as efficient as possible

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;

void spi1_dma_init(void){
    // Initialize ring buffer variables
    // all variables are stored in .dmaBuff section in CCSRAM memory region for faster access
    memset((void*)hspi1_transaction_buffer,0,sizeof(hspi1_transaction_buffer));
    hspi1_transaction_buffer_head=0;
    hspi1_transaction_buffer_tail=0;
    hspi1_transaction_buffer_length=0;
    hspi1_dma_active=false;
    hspi1_tx_processed=false;
}

bool spi1_dma_enq_data(hspi_data*transactionData){
    // 1. Check if there is enough space in the buffer for the new transaction
    uint16_t free_space=0;
    if(hspi1_transaction_buffer_head>=hspi1_transaction_buffer_tail){
        free_space=SPI1_HSPI_DATA_BUFFER_SIZE-(hspi1_transaction_buffer_head-hspi1_transaction_buffer_tail);
    }
    else{
        free_space=hspi1_transaction_buffer_head-hspi1_transaction_buffer_tail;
    }

    if(free_space>0){
        // There is enough space, enqueue the data
        __disable_irq();
        hspi1_transaction_buffer[hspi1_transaction_buffer_head]=*transactionData;
        hspi1_transaction_buffer_length++;
        hspi1_transaction_buffer_head=(hspi1_transaction_buffer_head+1)%SPI1_HSPI_DATA_BUFFER_SIZE;
        // If DMA is not active start transmission
        if(hspi1_dma_active==false){

            hspi1_dma_active=true;
            spi1_send_data();
            // Transmit SPI DMA
            // Ustawienie pinu
            // Operacja
            // No i tak kiedy przychodzi callback w takim razie?
            // Tutaj na pewno rozróżnienie, jeżeli rx jest null to poszło całe
            // Ale jeżeli rx!=null to trzeba powtórzyć operację
            // Ustawienie pinu
        }
        __enable_irq();
        // If DMA is active, data will be sent upon DMA completion interrupt
        return true;
    }
    else{
        // Not enough space
        // Intented to use with RTOS where caller can retry later - when space is available
        return false;
    }

    return false;
}

uint8_t spi1_send_data(void){
    // Tx frame was not processed
    if(hspi1_tx_processed==false){
        // First transmission is always tx
        // So that, we have to select slave by pulling CS pin LOW
        HAL_GPIO_WritePin(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,
            GPIO_PIN_RESET);
        HAL_SPI_Transmit_DMA(&hspi1,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txBuffer,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].txLength);
    }
    else{

    }
    return 0;
}

void spi1_dma_tx_complete(void){
    // Check if there was a read operation
    // Read operation requires rx buffer to be provided
    // If there is no rx buffer (NULL or nullptr) move on to the next package
    if(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxBuffer!=NULL){
        HAL_SPI_Receive_DMA(&hspi1,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxBuffer,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].rxLength);
    }
    else{
        // Notify the owner of the data that the transmission was complete
        *(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].transmissionStatus)++;
        HAL_GPIO_WritePin(hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_port,
            hspi1_transaction_buffer[hspi1_transaction_buffer_tail].gpio_pin,
            GPIO_PIN_SET);
        hspi1_transaction_buffer_tail++;
        hspi1_transaction_buffer_length--;
    }
    // If there is more data to send
    if(hspi1_transaction_buffer_length>0){
        hspi1_dma_active=true;
        spi1_send_data();
    }
    else{
        hspi1_dma_active=false;
    }
}


// This driver should also remember slave select pins when there are multiple slaves and set them accordingly when transmitting data
// For now we assume there is only one slave connected to SPI1 and it is always selected
// There should be a dedicated array or structure to store information about currently selected slave
// When transmission is over and there is more data to send, the driver should set slave select pins accordingly before starting next transmission
// I think that good idea is to pass CS pin information together with data to be transmitted in the spi1_dma_enq_data function
// so that driver can handle slave selection internally and user doesn't have to worry about it