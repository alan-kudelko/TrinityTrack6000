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
#include <string.h>

#include <task_CLI.h>

#include <TrinityTrack6000_Pinout.h>

#include <USART1_Dma.h>

CLI_Task_State cli_task_state=CLI_STATE_DIAG_MENU; // Default state is diagnostics menu, can be changed to test menu by command

const char task_CLI_name[]="CLI Task";
TX_THREAD task_CLI_handle;
ULONG task_CLI_stack[TASK_CLI_STACK_SIZE];
TX_SEMAPHORE sem_task_CLI_command_ready;

const char command_help[]="help";

const char command_switch_mode[]="mode";
const char command_switch_mode_diag[]="diag";
const char command_switch_mode_test[]="test";
const char*command_switch_mode_children[]={command_switch_mode_diag,command_switch_mode_test};

const char*command_array[COMMANDS_MAX_COUNT]={command_help,command_switch_mode};

const void (*parse_functions[COMMANDS_MAX_COUNT])(uint8_t,char*[])={parse_command_help,parse_command_switch_mode};

/*
    For now let's assume that commands are divided into 2 categories
    diagnostics commands and test commands
*/
const char msg_task_CLI_help[]=
"[CLI] Available commands:\r\n\
      help - Show this help message\r\n\
      mode <diag/test> - Switch CLI mode between diagnostics and test menu\r\n";

const char msg_task_CLI_diag_menu_header[]="[CLI] DIAG> ";
const char msg_task_CLI_test_menu_header[]="[CLI] TEST> ";
const char msg_task_CLI_unknown_command[]="[CLI] Unknown command. Type help for available commands\r\n";

const char msg_task_CLI_mode_switched_to_diag[]="[CLI] Switched to diagnostics mode\r\n";
const char msg_task_CLI_mode_switched_to_test[]="[CLI] Switched to test mode\r\n";
const char msg_task_CLI_mode_switch_failed[]="[CLI] Failed to switch mode\r\n[CLI] Correct usage: mode <diag/test>\r\n";


void task_CLI_init(void){
    // Initialize semaphore
    tx_semaphore_create(&sem_task_CLI_command_ready,
                        (char*)"CLI Ready",
                        0); // Initial count 0
    // Initialize FSM state to default diagnostics menu
    cli_task_state=CLI_STATE_DIAG_MENU;
}

void parse_command(char*command,uint16_t length){
    // Parse received command and execute corresponding actions
    // For now just a placeholder function

    // Parsing idea is following:
    // 1. Tokenize the command string using space as a delimiter to separate command and its arguments into tokens
    // 2. Compare the first token with known command strings to identify the command
    // 3. Based on the command, parse the rest of the tokens as arguments and execute corresponding actions
    // For instance if the command is "show mem", then the first token is "show" which indicates that we want to
    // show some information, and the second token is "mem" which indicates that we want to show memory information,
    // so we can call a function that retrieves memory information and sends it back to the terminal
    // If the command is not found or arguments are invalid, we can send an error message back to the terminal
    // Or we can send a hint message back to the terminal indicating available commands and their usage

    // 1. Tokenization
    // We can use strtok function from string.h to tokenize the command string, but we need to be careful when using it in a multi-threaded
    // environment like FreeRTOS or ThreadX because strtok is not thread-safe, so we need to ensure that only one thread is using strtok at a time
    // We can also track the offset of the current token in the command string to avoid modifying the original command string
    // For simplicity, let's assume that the maximum number of tokens in a command is 10, we can adjust this number as needed
    char*tokens[TOKENS_MAX_COUNT]={NULL}; // Max 10 tokens
    uint8_t token_count=0; // Number of tokens parsed from the command
    char*saveptr; // Pointer for strtok_r to maintain context between calls
    // If there are more than 10 tokens, command is certainly invalid, so we can just return an error message
    tokens[token_count]=strtok_r(command," \r\n",&saveptr); // Get first token
    while(tokens[token_count]!=NULL){
        token_count++;
        if(token_count>TOKENS_MAX_COUNT){
            // Too many tokens, command is invalid
            while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
            }
            return;
        }
        tokens[token_count]=strtok_r(NULL," \r\n",&saveptr); // Get next token
    }
    // 2. Command recognition
    // Idea there is to use an iterative approach where we compare the first token with known command strings in a loop
    // If a match is found, we can call a corresponding function to handle that command and pass the rest of
    // the tokens as arguments to that function
    // The easiest way is to create a command table which is an array of structures where each structure contains a command string and
    // a pointer to a function that handles that command, then we can just loop through the command table and compare
    // the first token with the command string in the command table, if a match is found, we can call the corresponding function
    // If no match is found after looping through the entire command table, we can send an error message back to the terminal 

    // For now just a test implementation for the "mode" command to switch between diagnostics and test menu

    uint8_t command_index=COMMANDS_MAX_COUNT; // Index of the recognized command in the command table
    for(uint8_t i=0;i<COMMANDS_MAX_COUNT;i++){
        if(strncmp(tokens[0],command_array[i],strlen(command_array[i])-1)==0){
            command_index=i;
            break;
        }
    }
    // 3. Command execution
    // If command is recognized, call corresponding function to handle that command, otherwise send error message back to the terminal
    // Note: we can also implement a more advanced command recognition algorithm that can handle partial command matches and provide suggestions
    // For instance if the user types "mo" instead of "mode", we can recognize that as a partial match for the "mode" command and send 
    // a suggestion message back to the terminal indicating that the user might have meant "mode" command 
    if(command_index<COMMANDS_MAX_COUNT){
        // Command recognized, call corresponding function to handle that command
        parse_functions[command_index](token_count,tokens);
    }
    else{
        // Command not recognized, send error message back to the terminal
        while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
        }
    }
}
void parse_command_help(uint8_t argc,char*argv[]){
    // This function is called when the "help" command is received in the CLI interface
    // It provides information about available commands and their usage to the user

    // For now just a placeholder implementation that sends a static help message back to the terminal
    // In the future we can implement a more dynamic help system that generates help messages based on the available commands and their usage information
    while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_help,strlen(msg_task_CLI_help))!=true){
        tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
    }
}

void parse_command_switch_mode(uint8_t argc,char*argv[]){
    // This function is called when the "mode" command is received
    // It parses the child command to determine which mode to switch to (diagnostics or test)

    if((argc<2)||(argc>2)){
        // Invalid number of arguments, send hint back to terminal indicating correct usage of the command
        while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_mode_switch_failed,strlen(msg_task_CLI_mode_switch_failed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
        }
        return;
    }
    // Number of arguments is valid, now check the value of the child command
    if(strncmp(argv[1],command_switch_mode_diag,strlen(command_switch_mode_diag)-1)==0){
        // Switch to diagnostics mode
        cli_task_state=CLI_STATE_DIAG_MENU;
        while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_mode_switched_to_diag,strlen(msg_task_CLI_mode_switched_to_diag))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
        }
    }
    else if(strncmp(argv[1],command_switch_mode_test,strlen(command_switch_mode_test)-1)==0){
        // Switch to test mode
        cli_task_state=CLI_STATE_TEST_MENU;
        while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_mode_switched_to_test,strlen(msg_task_CLI_mode_switched_to_test))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
        }
    }
    else{
        // Invalid child command, send hint back to terminal indicating correct usage of the command
        while(usart1_dma_enq_data((uint8_t*)msg_task_CLI_mode_switch_failed,strlen(msg_task_CLI_mode_switch_failed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
        }
    }
    // Remember that when mode is set to test mode, this task should inform
    // execuite task to disable control
}

void display_menu_header(void){
    // Display menu header based on current FSM state
    switch(cli_task_state){
        case CLI_STATE_DIAG_MENU:
            while(usart1_dma_enq_data((const uint8_t*)msg_task_CLI_diag_menu_header,strlen(msg_task_CLI_diag_menu_header))!=true){
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
            while(usart1_dma_enq_data((const uint8_t*)msg_task_CLI_diag_menu_header,strlen(msg_task_CLI_diag_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART1 DMA
            }
            break;
    }
}

void task_CLI(ULONG arg){
    UNUSED(arg);
    char rx_buffer[UART1_DMA_RX_RING_BUFFER_SIZE]={0};
    uint16_t rx_length=0;
    while(1){
        // Display menu header based on current FSM state
        display_menu_header();
        // Wait for command ready semaphore
        // This semaphore is given in the USART1 DMA reception complete callback when a full command is received (indicated by \r or \n character)
        tx_semaphore_get(&sem_task_CLI_command_ready,TX_WAIT_FOREVER);
        // Read data from CLI interface and process commands
        // For now just write received command back to UART
        memset(rx_buffer,0,sizeof(rx_buffer)); // Clear rx buffer before reading new command
        if(usart1_dma_read_data((uint8_t*)rx_buffer,&rx_length,UART1_DMA_RX_RING_BUFFER_SIZE-1)){
            // Data is valid
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
