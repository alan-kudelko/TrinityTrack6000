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

#define TOKENS_MAX_COUNT 10 /**< Maximum number of tokens in a command */
#define COMMANDS_MAX_COUNT 2 /**< Maximum number of supported commands, can be adjusted as needed */

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

/**
 * @brief CLI Task command strings and messages
 * These are constant strings used for command recognition and user feedback in the CLI task.
 * @{
 */

extern const char command_help[]; /**< Command string for "help" command */

extern const char command_switch_mode[]; /**< Command string for switching CLI mode */
extern const char command_switch_mode_diag[]; /**< Child command string for switching to diagnostics mode */
extern const char command_switch_mode_test[]; /**< Child command string for switching to test mode */
extern const char*command_switch_mode_children[]; /**< Array of child command strings for the "mode" command */

extern const char*command_array[COMMANDS_MAX_COUNT]; /**< Array of command strings for command recognition, indexed by command type */

extern const void (*parse_functions[COMMANDS_MAX_COUNT])(uint8_t,char*[]); /**< Array of function pointers for parsing commands, indexed by command type */

/** @} */

extern const char msg_task_CLI_help[]; /**< Help message for all the CLI task commands*/

extern const char msg_task_CLI_diag_menu_header[]; /**< Default menu header for CLI task */
extern const char msg_task_CLI_test_menu_header[]; /**< Test menu header for CLI task */
extern const char msg_task_CLI_unknown_command[]; /**< Message indicating unknown command received in CLI task */
extern const char msg_task_CLI_mode_switched_to_diag[]; /**< Message indicating that CLI mode has been switched to diagnostics mode */
extern const char msg_task_CLI_mode_switched_to_test[]; /**< Message indicating that CLI mode has been switched to test mode */
extern const char msg_task_CLI_mode_switch_failed[]; /**< Message indicating that CLI mode switch failed with correct usage hint */ 

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
void parse_command(char*command,uint16_t length);

/**
 * @brief Parse "help" command to display CLI help information.
 * This function is called when the "help" command is received in the CLI interface.
 * It provides information about available commands and their usage to the user.
 * @param argc: Number of arguments in the command (including the main command and child command).
 * @param argv: Array of strings representing the command and its arguments, where argv[0] is the main command ("help").
 * @return None.
 */
void parse_command_help(uint8_t argc,char*argv[]);

/**
 * @brief Parse "mode" command to switch CLI mode.
 * This function is called when the "mode" command is received in the CLI interface.
 * It parses the child command to determine which mode to switch to (diagnostics or test).
 * Based on the child command, it updates the CLI task's FSM state and sends a confirmation message back to the terminal.
 * @param argc: Number of arguments in the command (including the main command and child command).
 * @param argv: Array of strings representing the command and its arguments, where argv[0]
 * is the main command ("mode") and argv[1] is the child command ("diag" or "test").
 * @return None.
 */
void parse_command_switch_mode(uint8_t argc,char*argv[]);

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