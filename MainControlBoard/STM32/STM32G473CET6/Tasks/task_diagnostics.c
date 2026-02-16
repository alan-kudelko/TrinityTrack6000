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

const char command_show[]="show";

void task_diagnostics_init(void){
    // Initialize semaphore
    tx_semaphore_create(&sem_task_diagnostics_command_ready,
                        (char*)"DiagCmdReady",
                        0); // Initial count 0
}

void parse_command(const uint8_t*command,uint16_t length){
    // Parse received command and execute corresponding actions
    // For now just a placeholder function

    // Parsing idea is following:
    // 1. Find all the spaces in the command and split the command into tokens based on spaces
    // 2. Compare the first token with known command strings to identify the command
    // 3. Based on the command, parse the rest of the tokens as arguments and execute corresponding actions
    // For instance if the command is "show mem", then the first token is "show" which indicates that we want to
    // show some information, and the second token is "mem" which indicates that we want to show memory information,
    // so we can call a function that retrieves memory information and sends it back to the terminal
    // If the command is not found or arguments are invalid, we can send an error message back to the terminal
    // Or we can send a hint message back to the terminal indicating available commands and their usage
    
}

void task_diagnostics(ULONG arg){
    UNUSED(arg);
    uint8_t rx_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
    uint16_t rx_length=0;
    while(1){
        // Wait for command ready semaphore
        // This semaphore is given in the USART1 DMA reception complete callback when a full command is received (indicated by \r or \n character)
        tx_semaphore_get(&sem_task_diagnostics_command_ready,TX_WAIT_FOREVER);
        // Read data from CLI interface and process commands
        // For now just write received command back to UART
        if(usart1_dma_read_data(rx_buffer,&rx_length,UART1_DMA_RX_RING_BUFFER_SIZE-1)){
            // Data is valid
            // Echo back received data
            // Add \r\n at the end of the command for better formatting in terminal
            rx_length++;
            rx_buffer[rx_length-1]='\n';
            usart1_dma_enq_data(rx_buffer,rx_length);
            // Parse command and execute corresponding actions
            parse_command(rx_buffer,rx_length);
        }
        else{
            // Data corrupted or error
            // Also possible that command is empty
            // For instance pressing enter without typing anything will result in \r or \n character being received and treated as a command, but there is no actual command to parse
            // In this case we can choose to ignore it or send an error message back to the terminal
            // For now just send an error message back to the terminal
            const char msg_diagnostics_error_corrupted_data[]="[DIAG][ERROR] Corrupted data received\r\n";
            usart1_dma_enq_data((const uint8_t*)msg_diagnostics_error_corrupted_data,strlen(msg_diagnostics_error_corrupted_data));
        }
    }
}
