/**
 * @file task_diagnostics.h
 * @brief Diagnostics task for system monitoring with CLI interface.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TASK_CLI_H_
    #define TASK_CLI_H_

#include <tx_api.h>

#define TASK_CLI_STACK_SIZE 512  /**< Stack size for CLI task */
#define TASKS_CLI_PRIORITY    2  /**< Priority for CLI task */

#define TASK_CLI_RETRY_DELAY_MS 100 /**< Delay in milliseconds before retrying to enqueue data to UART1 DMA if it fails */

/**
 * @brief CLI Task FSM States
 * This enumeration defines the possible states of the CLI task's finite state machine (FSM).
 * The CLI task can be in one of the following states:
 * CLI_STATE_DIAG_MENU: The default state where the CLI task provides diagnostics commands to monitor system status and performance.
 * CLI_STATE_TEST_MENU: A test state where the CLI task provides test commands for testing various functionalities of the system.
 * @warning It is important to ensure that during testing, the CLI task should disable control of any other external hardware
 * managing tasks to prevent unintended consequences. For instance, if there is a command that allows testing of motor control, 
 * it should ensure that it does not interfere with the normal operation of the motor control task or any safety mechanisms in place.
 * This can be achieved by implementing a semaphore or mutex that other tasks check before performing any operations
 * that could be affected by the test commands, and the CLI task can take control of that semaphore/mutex when
 * entering the test menu state to ensure that other tasks do not perform any operations that could interfere with
 * the testing process. Additionally, it is crucial to implement proper error handling and safety checks in the test commands
 * to prevent any potential damage to the hardware or unsafe conditions during testing. Always ensure that the test commands 
 * are designed with safety in mind and that there are clear instructions and warnings for the user when using the test menu.
 */
typedef enum{
    CLI_STATE_DIAG_MENU, /**< Default diagnostics menu state */
    CLI_STATE_TEST_MENU  /**< Test menu state */
} CLI_Task_State;

extern const char task_CLI_name[];  /**< Name of the CLI task */

extern TX_THREAD task_CLI_handle __attribute((section(".task_handles.task_CLI"))); /**< Thread handlefor CLI task */
extern ULONG task_CLI_stack[TASK_CLI_STACK_SIZE] __attribute((section(".task_stacks.task_CLI"))); /**< Stack for CLI task */
extern TX_SEMAPHORE sem_task_CLI_command_ready __attribute((section(".task_semaphores.task_CLI"))); /**< Semaphore for CLI task indicating command ready to parse */

extern const char command_show[]; /**< Command string for showing diagnostics information */

extern const char msg_task_CLI_default_menu_header[]; /**< Default menu header for CLI task */
extern const char msg_task_CLI_test_menu_header[]; /**< Test menu header for CLI task */
extern const char msg_task_CLI_unknown_command[]; /**< Message indicating unknown command received in CLI task */

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Initialize CLI task.
 * This function sets up necessary resources for the CLI task,
 * such as semaphores.
 * @param None.
 * @return None.
 */
void task_CLI_init(void);

/**
 * @brief Display CLI menu header based on current FSM state.
 * This function checks the current state of the CLI task's FSM and
 * sends the appropriate menu header to the terminal via USART1 DMA.
 * It ensures that the correct menu header is displayed before the command prompt.
 * @param None.
 * @return None.
 */
void display_menu_header(void);

/**
 * @brief Parse received command from CLI interface.
 * This function takes a received command and its length, parses it,
 * and executes corresponding actions based on the command.
 * @param command: Pointer to the received command data.
 * @param length: Length of the received command data in bytes.
 * @return None.
 */
void parse_command(const uint8_t*command,uint16_t length);

/**
 * @brief CLI task function.
 * This task handles system diagnostics and provides a CLI interface
 * for monitoring system status and performance.
 * @param arg: Argument passed to the task (not used).
 * @return None.
 */

void task_CLI(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // TASK_CLI_H_