/**
 * @file USART1_Dma.c
 * @brief Implementation of USART1 with DMA functionality and dedicated ring buffer.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <USART1_Dma.h>

#include <stdbool.h>
#include <string.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Pinout.h>

uint8_t huart1_dma_rx_buffer[UART1_DMA_RX_BUFFER_SIZE]={0};
uint8_t huart1_dma_tx_buffer[UART1_DMA_TX_BUFFER_SIZE]={0};

volatile uint8_t huart1_dma_rx_ring_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
volatile uint8_t huart1_dma_tx_ring_buffer[UART1_DMA_TX_RING_BUFFER_SIZE]={0};

volatile uint16_t huart1_dma_tx_buffer_length=0;

volatile uint16_t huart1_dma_rx_old_pos=0;
volatile uint16_t huart1_dma_rx_ring_buffer_head=0;
volatile uint16_t huart1_dma_rx_ring_buffer_tail=0;
volatile uint16_t huart1_dma_rx_ring_buffer_length=0;

volatile uint16_t huart1_dma_tx_ring_buffer_head=0;
volatile uint16_t huart1_dma_tx_ring_buffer_tail=0;
volatile uint16_t huart1_dma_tx_ring_buffer_length=0;

volatile bool huart1_dma_tx_active=false;
// Decide if use of volatile is necessary for these variables
// This is a low level driver so it has to be as efficient as possible

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern TX_SEMAPHORE sem_task_CLI_command_ready;

void usart1_dma_init(void){
    // Initialize ring buffer variables
    // all variables are stored in .dmaBuff section in CCSRAM memory region for faster access
    huart1_dma_rx_old_pos=0;
    huart1_dma_rx_ring_buffer_head=0;
    huart1_dma_rx_ring_buffer_tail=0;
    huart1_dma_rx_ring_buffer_length=0;

    huart1_dma_tx_ring_buffer_head=0;
    huart1_dma_tx_ring_buffer_tail=0;
    huart1_dma_tx_ring_buffer_length=0;

    huart1_dma_tx_buffer_length=0;
    huart1_dma_tx_active=false;

    memset((void*)huart1_dma_rx_buffer,0,UART1_DMA_RX_BUFFER_SIZE);
    memset((void*)huart1_dma_tx_buffer,0,UART1_DMA_TX_BUFFER_SIZE);
    memset((void*)huart1_dma_rx_ring_buffer,0,UART1_DMA_RX_RING_BUFFER_SIZE);
    memset((void*)huart1_dma_tx_ring_buffer,0,UART1_DMA_TX_RING_BUFFER_SIZE);
}

bool usart1_dma_enq_data(const uint8_t*data,const uint16_t length){
    // Need fixing
    // Possible deadlock
    // IRQ may be pending
    // If there is a pending IRQ, it will not be handled until this function exits
    
    // Check if there is enough space in the transmit ring buffer
    uint16_t free_space=0;
    if(huart1_dma_tx_ring_buffer_head>=huart1_dma_tx_ring_buffer_tail){
        free_space=UART1_DMA_TX_RING_BUFFER_SIZE-(huart1_dma_tx_ring_buffer_head-huart1_dma_tx_ring_buffer_tail);
    }
    else{
        free_space=huart1_dma_tx_ring_buffer_tail-huart1_dma_tx_ring_buffer_head;
    }

    if(free_space>length){
        // There is enough space, enqueue the data
        __disable_irq();
        for(uint16_t i=0;i<length;i++){
            huart1_dma_tx_ring_buffer[huart1_dma_tx_ring_buffer_head]=data[i];
            huart1_dma_tx_ring_buffer_head=(huart1_dma_tx_ring_buffer_head+1)%UART1_DMA_TX_RING_BUFFER_SIZE;
        }
        huart1_dma_tx_ring_buffer_length+=length;
        // If DMA is not active, copy data to the DMA buffer and start transmission
        if(huart1_dma_tx_active==false){
            // Check if data will fit into DMA buffer
            usart1_dma_copy_to_tx_buffer(huart1_dma_tx_buffer);
            huart1_dma_tx_active=true;
            HAL_UART_Transmit_DMA(&huart1,huart1_dma_tx_buffer,huart1_dma_tx_buffer_length);
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

void usart1_dma_copy_to_tx_buffer(uint8_t*dst){
    // Copy data from the transmit ring buffer to the provided destination DMA buffer
    // Remember to update ring buffer tail and length accordingly
    uint16_t bytes_to_copy=huart1_dma_tx_ring_buffer_length;
    // Crop to DMA buffer size
    if(bytes_to_copy>UART1_DMA_TX_BUFFER_SIZE){
        bytes_to_copy=UART1_DMA_TX_BUFFER_SIZE;
    }
    // Copy data
    uint16_t i=0;
    for(i=0;i<bytes_to_copy;i++){
        dst[i]=huart1_dma_tx_ring_buffer[huart1_dma_tx_ring_buffer_tail];
        huart1_dma_tx_ring_buffer_tail=(huart1_dma_tx_ring_buffer_tail+1)%UART1_DMA_TX_RING_BUFFER_SIZE;
    }
    // Update ring buffer tail and length
    huart1_dma_tx_buffer_length=i;
    huart1_dma_tx_ring_buffer_length-=i;
}

void usart1_dma_tx_complete(void){
    if(huart1_dma_tx_ring_buffer_length>0){
        // More data to send
        usart1_dma_copy_to_tx_buffer(huart1_dma_tx_buffer);
        huart1_dma_tx_active=true;
        HAL_UART_Transmit_DMA(&huart1,huart1_dma_tx_buffer,huart1_dma_tx_buffer_length);
    }
    else{
        // No more data to send
        huart1_dma_tx_active=false;
    }
}

void usart1_dma_rx_init(void){
    HAL_StatusTypeDef hal_status;
    // Initialize DMA for USART1 RX
    // Start the first DMA reception
    hal_status=HAL_UART_Receive_DMA(&huart1,huart1_dma_rx_buffer,UART1_DMA_RX_BUFFER_SIZE);

    // Check for errorrs
    if(hal_status!=HAL_OK){
        // Error handling
        // For now just a debug loop
        while(1){
            // Debug loop
        }
    }
    // If there is an error withing HAL
    // Try to reinitialize DMA reception
    // Change signature of function to return HAL_StatusTypeDef
    // To allow error handling by caller
    // Enable IDLE line detection interrupt
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    // Disable Half Transfer interrupt to avoid unnecessary interrupts
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
}

bool usart1_dma_read_data(uint8_t*dst,uint16_t*length,const uint16_t maxLength){
    // Check if parameters are valid
    if((dst==NULL)||(length==NULL)||(maxLength==0)){
        // Invalid parameters
        return false;
    }
    // Disable only USART1 and DMA1_Channel7 interrupts to avoid inconsistencies during buffer update
    __NVIC_DisableIRQ(DMA1_Channel7_IRQn);
    __NVIC_DisableIRQ(USART1_IRQn);
    // Check if there is data available in the receive ring buffer
    // This is correct
    if(huart1_dma_rx_ring_buffer_length==0){
        // No data available
        *length=0;
        __NVIC_EnableIRQ(DMA1_Channel7_IRQn);
        __NVIC_EnableIRQ(USART1_IRQn);
        return false;
    }
    // Determine if there is a complete command in the buffer
    // A complete command ends with \r or \n or \r\n
    // Iterate through the ring buffer to find the end of command
    uint16_t temp_tail=huart1_dma_rx_ring_buffer_tail;
    uint8_t command_found=UART1_DMA_RX_NO_COMMAND; // 0 - no command, 1 - \r found, 2 - \n found
    // Check if first character is end of command
    // In case someone sends only \r or \n or combination by clicking enter multiple times
    while(((huart1_dma_rx_ring_buffer[temp_tail]==UART1_DMA_RX_R_CHAR)||(huart1_dma_rx_ring_buffer[temp_tail]==UART1_DMA_RX_N_CHAR))&&
          (temp_tail!=huart1_dma_rx_ring_buffer_head)){
        // Skip end characters at the beginning
        // May be used if someone presses enter multiple times
        temp_tail=(temp_tail+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
    }
    // Now search for end character
    while(temp_tail!=huart1_dma_rx_ring_buffer_head){
        if(huart1_dma_rx_ring_buffer[temp_tail]==UART1_DMA_RX_R_CHAR){
            command_found=UART1_DMA_RX_R_FOUND;
            break;
        }
        else if(huart1_dma_rx_ring_buffer[temp_tail]==UART1_DMA_RX_N_CHAR){
            command_found=UART1_DMA_RX_N_FOUND;
            break;
        }
        temp_tail=(temp_tail+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
    }
    temp_tail=(temp_tail+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
    // FIX magic numbers according to MISRA C:2025
    // FIX/Check longer strings in putty
    switch(command_found){
        case UART1_DMA_RX_R_FOUND:
        case UART1_DMA_RX_N_FOUND:
            // Complete command found
            // Determine how much data to copy including the end character
            // End character is switched to \0
            uint16_t command_length=0;

            if(temp_tail>huart1_dma_rx_ring_buffer_tail){
                command_length=temp_tail-huart1_dma_rx_ring_buffer_tail;
            }
            else{
                command_length=UART1_DMA_RX_RING_BUFFER_SIZE-(huart1_dma_rx_ring_buffer_tail-temp_tail);
            }
            uint16_t i=0;
            for(;i<command_length;i++){
                dst[i]=huart1_dma_rx_ring_buffer[huart1_dma_rx_ring_buffer_tail];
                huart1_dma_rx_ring_buffer_tail=(huart1_dma_rx_ring_buffer_tail+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
            }
            dst[i-1]='\0';
            *length=i;
            // Check for \r\n sequence or \n\r sequence and adjust temp_tail accordingly
            // Or any other combination of two end characters to make this robust
            huart1_dma_rx_ring_buffer_length-=command_length;
            __NVIC_EnableIRQ(DMA1_Channel7_IRQn);
            __NVIC_EnableIRQ(USART1_IRQn);
            return true;
        case UART1_DMA_RX_NO_COMMAND:
            // No complete command found
        default:
            // Should not happen
            *length=0;
            huart1_dma_rx_ring_buffer_tail=huart1_dma_rx_ring_buffer_head;
            huart1_dma_rx_ring_buffer_length=0;
            __NVIC_EnableIRQ(DMA1_Channel7_IRQn);
            __NVIC_EnableIRQ(USART1_IRQn);
            return false;
    }
}

void usart1_dma_rx_complete(void){
    // For now a debug diode
    // Called inside USART1_IRQHandler when data reception is complete

    uint16_t dma_pos;
    uint16_t dma_transfer_size;
    // Calculate current DMA position
    dma_pos=UART1_DMA_RX_BUFFER_SIZE-__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

    if(dma_pos==huart1_dma_rx_old_pos){
        // No new data
        return;
    }
    if(dma_pos>huart1_dma_rx_old_pos){
        // Continuous data
        dma_transfer_size=dma_pos-huart1_dma_rx_old_pos;
    }
    else{
        // Wrap around
        dma_transfer_size=(UART1_DMA_RX_BUFFER_SIZE-huart1_dma_rx_old_pos)+dma_pos;
    }
    // Check if there is enough space in the ring buffer to copy new data
    if((huart1_dma_rx_ring_buffer_length+dma_transfer_size)>UART1_DMA_RX_RING_BUFFER_SIZE){
        // Not enough space, data will be lost
        // Don't copy data
        // Update old position to current DMA position to avoid copying the same data again in the next callback
        huart1_dma_rx_old_pos=dma_pos;
        
        return;
    }
    else{
        // Enough space, copy data to the ring buffer
        uint16_t i=0;
        for(;i<dma_transfer_size;i++){
            huart1_dma_rx_ring_buffer[huart1_dma_rx_ring_buffer_head]=huart1_dma_rx_buffer[(huart1_dma_rx_old_pos+i)%UART1_DMA_RX_BUFFER_SIZE];
            huart1_dma_rx_ring_buffer_head=(huart1_dma_rx_ring_buffer_head+1)%UART1_DMA_RX_RING_BUFFER_SIZE;
        }
        // Update ring buffer length
        huart1_dma_rx_ring_buffer_length+=dma_transfer_size;
        // Update old position to current DMA position
        huart1_dma_rx_old_pos=dma_pos;
    }
    // Signal diagnostics task that command is ready to parse
    // Note that USART1 is used only for diagnostics task and this semaphore is used only to signal that command is ready to parse
    // Parsing if left to diagnostics task to allow for more flexible command handling and avoid blocking USART1 interrupt handler
    if((huart1_dma_rx_ring_buffer[(huart1_dma_rx_ring_buffer_head-1)%UART1_DMA_RX_RING_BUFFER_SIZE]=='\n')||(huart1_dma_rx_ring_buffer[(huart1_dma_rx_ring_buffer_head-1)%UART1_DMA_RX_RING_BUFFER_SIZE]=='\r')){
        //HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
        tx_semaphore_put(&sem_task_CLI_command_ready);
    }
}
