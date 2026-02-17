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

#include <task_CLI.h>

#include <TrinityTrack6000_Pinout.h>

#include <USART1_Dma.h>

CLI_Task_State cli_task_state=CLI_STATE_DIAG_MENU; // Default state is diagnostics menu, can be changed to test menu by command

const char task_CLI_name[]="CLI Task";
TX_THREAD task_CLI_handle;
ULONG task_CLI_stack[TASK_CLI_STACK_SIZE];
TX_SEMAPHORE sem_task_CLI_command_ready;

const char command_show[]="show";

const char msg_task_CLI_default_menu_header[]="[CLI] DIAG> ";
const char msg_task_CLI_test_menu_header[]="[CLI] TEST> ";
const char msg_task_CLI_unknown_command[]="[CLI][ERROR] Unknown command\r\n";

void task_CLI_init(void){
    // Initialize semaphore
    tx_semaphore_create(&sem_task_CLI_command_ready,
                        (char*)"CLI Ready",
                        0); // Initial count 0
    // Initialize FSM state to default diagnostics menu
    cli_task_state=CLI_STATE_DIAG_MENU;
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

void display_menu_header(void){
    // Display menu header based on current FSM state
    switch(cli_task_state){
        case CLI_STATE_DIAG_MENU:
            while(usart1_dma_enq_data((const uint8_t*)msg_task_CLI_default_menu_header,strlen(msg_task_CLI_default_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
            }
            break;
        case CLI_STATE_TEST_MENU:
            while(usart1_dma_enq_data((const uint8_t*)msg_task_CLI_test_menu_header,strlen(msg_task_CLI_test_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
            }
            break;
        default:
            // Should never reach here, but just in case, display default menu header
            while(usart1_dma_enq_data((const uint8_t*)msg_task_CLI_default_menu_header,strlen(msg_task_CLI_default_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
            }
            break;
    }
}

void task_CLI(ULONG arg){
    UNUSED(arg);
    uint8_t rx_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
    uint16_t rx_length=0;
    while(1){
        display_menu_header();
        // Wait for command ready semaphore
        // This semaphore is given in the USART1 DMA reception complete callback when a full command is received (indicated by \r or \n character)
        tx_semaphore_get(&sem_task_CLI_command_ready,TX_WAIT_FOREVER);
        // Read data from CLI interface and process commands
        // For now just write received command back to UART
        if(usart1_dma_read_data(rx_buffer,&rx_length,UART1_DMA_RX_RING_BUFFER_SIZE-1)){
            // Data is valid
            // Echo back received data
            // Add \r\n at the end of the command for better formatting in terminal
            rx_buffer[rx_length-1]='\r';
            rx_length++;
            rx_buffer[rx_length-1]='\n';
            while(usart1_dma_enq_data(rx_buffer,rx_length)!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
                // If enqueueing data to UART1 DMA fails, we can choose to either retry or skip sending the echoed command back to the terminal
                // For now let's just retry until it succeeds to ensure that the echoed command is always sent back to the terminal
            }
            // Parse command and execute corresponding actions
            parse_command(rx_buffer,rx_length);
        }
        else{
            // Data corrupted or error
            // Also possible that command is empty
            // For instance pressing enter without typing anything will result in \r or \n character being received and treated as a command, but there is no actual command to parse
            // In this case we can choose to ignore it or send an error message back to the terminal
            // For now just send an error message back to the terminal
            usart1_dma_enq_data((const uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command));
        }
    }
}

// For now let's just focus on diagnostic part of the task
// Next step is test mode where we can send commands to run specific tests and show results in terminal
// From architecture's point of view, there should be some kind of hardware ownership
// In test mode, this task should disable control for other tasks via some kind of mutex or semaphore to ensure
// that there are no conflicts between tasks when accessing hardware resources during tests

// For now plan is to implement a simple command parser that can parse commands