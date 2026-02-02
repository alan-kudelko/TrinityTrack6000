/**
 * @file task_diagnostics.c
 * @brief Implementation of a diagnostics task for system monitoring with CLI interface.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <stm32g4xx_hal.h>

#include <task_diagnostics.h>

#include <TrinityTrack6000_Pinout.h>

#include <USART1_Dma.h>

const char task_diagnostics_name[]="Diagnostics Task";
TX_THREAD task_diagnostics_handle;
ULONG task_diagnostics_stack[TASK_DIAGNOSTICS_STACK_SIZE];

TX_SEMAPHORE sem_task_diagnostics_command_ready;

void task_diagnostics_init(void){
    // Initialize semaphore
    tx_semaphore_create(&sem_task_diagnostics_command_ready,
                        (char*)"DiagCmdReady",
                        0); // Initial count 0
}

void task_diagnostics(ULONG arg){
    UNUSED(arg);
    uint8_t rx_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
    uint16_t rx_length=0;
    uint16_t max_length=UART1_DMA_RX_RING_BUFFER_SIZE;
    while(1){
        tx_semaphore_get(&sem_task_diagnostics_command_ready,TX_WAIT_FOREVER);
        // Read data from CLI interface and process commands
        // For now just write received command back to UART
        if(usart1_dma_read_data(rx_buffer,&rx_length,max_length-2)){
            // Data is valid
            // Echo back received data
            rx_buffer[rx_length]='\r';
            rx_buffer[rx_length+1]='\n';
            usart1_dma_enq_data(rx_buffer,rx_length);
        }
        else{
            // Data corrupted or error
            const char msg_diagnostics_error_corrupted_data[]="[DIAG][ERROR] Corrupted data received\r\n";
            usart1_dma_enq_data((const uint8_t*)msg_diagnostics_error_corrupted_data,strlen(msg_diagnostics_error_corrupted_data));
        }
    }
}