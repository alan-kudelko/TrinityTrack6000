/**
 * @defgroup task_CLI CLI Task
 * @brief Diagnostics task for system monitoring and testing with CLI interface.
 * 
 * @date 2026.01.03
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 * 
 * @{
 */

#ifndef TASK_CLI_H_
    #define TASK_CLI_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdbool.h>

#include <tx_api.h>

#define TASK_CLI_STACK_SIZE 1024  //!< Stack size for CLI task
#define TASKS_CLI_PRIORITY    2  //!< Priority for CLI task

#define TASK_CLI_RETRY_DELAY_MS 30 //!< Delay in milliseconds before retrying to enqueue data to UART1 DMA if it fails

#define TOKENS_MAX_COUNT 10 //!< Maximum number of tokens in a command
#define COMMANDS_MAX_COUNT 4 //!< Maximum number of supported commands, can be adjusted as needed

#define TASK_CLI_WAKEUP_QUEUE_STORAGE_LENGTH 4 //!< Length of the CLI wakeup queue storage in words (uint32_t), can be adjusted as needed

#define TASK_CLI_RX_BUFFER_SIZE 10 //!< Size of the rx buffer for receiving raw data from devices, can be adjusted as needed
#define TASK_CLI_TX_BUFFER_SIZE 10 //!< Size of the tx buffer for transmitting raw data to devices, can be adjusted as needed

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

extern TX_THREAD task_CLI_handle; /**< Thread handlefor CLI task */
extern ULONG task_CLI_stack[TASK_CLI_STACK_SIZE]; //!< Stack for CLI task

#define TASK_CLI_WAKEUP_USART_DATA 1 //!< Wakeup reason indicating that CLI task was woken up by USART data reception
#define TASK_CLI_WAKEUP_COMMAND_EXECUTED 2 //!< Wakeup reason indicating that CLI task was woken up after executing a command, can be used to trigger menu header update or other actions after command execution

typedef struct ALIGNED(4) TASK_CLI_WAKEUP_REASON{
    uint32_t wakeupReason;
}TASK_CLI_WAKEUP_REASON;

/**
 * @ingroup task_CLI
 * @{
 * @name CLI Commands
 * @brief CLI commands recognized by the parser
 */

extern const char command_help[]; //!< Command string for "help" command 
extern const char command_switch_mode[]; //!< Command string for switching CLI mode
extern const char command_switch_mode_diag[]; //!< Child command string for switching to diagnostics mode
extern const char command_switch_mode_test[]; //!< Child command string for switching to test mode
extern const char*command_switch_mode_children[]; //!< Array of child command strings for the "mode" command

extern const char command_show[]; //!< Command string for "show" command, can be used for showing various system information
extern const char command_show_mem[]; //!< Child command string for showing memory information
extern const char*command_show_children[]; //!< Array of child command strings for the "show" command

extern const char command_show_mem_ram1[]; //!< Child command string for showing RAM1 information
extern const char command_show_mem_ram2[]; //!< Child command string for showing RAM2 information
extern const char command_show_mem_ccsram[]; //!< Child command string for showing CCSRAM information
extern const char*command_show_mem_children[]; //!< Array of child command strings for the "show memory" command

extern const char command_write[]; //!< Command string for write operation
/**@} */

/**
 * @brief CLI Task command strings and messages
 * These are constant strings used for command recognition and user feedback in the CLI task.
 * @{
 */

extern const char*command_array[COMMANDS_MAX_COUNT]; /**< Array of command strings for command recognition, indexed by command type */

extern const void (*parse_functions[COMMANDS_MAX_COUNT])(uint8_t,char*[]); /**< Array of function pointers for parsing commands, indexed by command type */

/** @} */

/**
 * @ingroup task_CLI
 * @{
 * @name CLI Messages
 * @brief Messages printed by the CLI task
 */

extern const char msg_task_CLI_help[]; //!< Help message for all the CLI task commands
extern const char msg_task_CLI_help_show[]; //!< Help message for "show" command and its child commands
extern const char msg_task_CLI_help_write[]; //!< Help message for "write" command and its child commands

extern const char msg_task_CLI_diag_menu_header[]; //!< Default menu header for CLI task
extern const char msg_task_CLI_test_menu_header[]; //!< Test menu header for CLI task
extern const char msg_task_CLI_unknown_command[]; //!< Message indicating unknown command received in CLI task
extern const char msg_task_CLI_mode_switched_to_diag[]; //!< Message indicating that CLI mode has been switched to diagnostics mode
extern const char msg_task_CLI_mode_switched_to_test[]; //!< Message indicating that CLI mode has been switched to test mode
extern const char msg_task_CLI_mode_switch_failed[]; //!< Message indicating that CLI mode switch failed with correct usage hint

/**@} */

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
 * @brief Parse "show" command to display system information.
 * This function is called when the "show" command is received in the CLI interface.
 * It parses the child command to determine what information to show (e.g., memory information).
 * Based on the child command, it retrieves the requested information and sends it back to the terminal.
 * @param argc: Number of arguments in the command (including the main command and child command).
 * @param argv: Array of strings representing the command and its arguments, where argv[0]
 * is the main command ("show") and argv[1] is the child command (e.g., "memory").
 * @return None.
 */
void parse_command_show(uint8_t argc,char*argv[]);

/**
 * @brief Parse "write" command to perform write operations.
 * This function is called when the "write" command is received in the CLI interface.
 * It parses the child command to determine which device to write to (e.g., MCP1 or MCP2) and performs the corresponding write operation.
 * @param argc: Number of arguments in the command (including the main command and child command
 * and any additional parameters needed for the write operation).
 * @param argv: Array of strings representing the command and its arguments, where argv[0]
 * is the main command ("write") and argv[1] is the child command (e.g., "mcp1" or "mcp2") followed by any additional parameters needed for the write operation.
 * @return None.
 */
void parse_command_write(uint8_t argc,char*argv[]);

/**
 * @brief Callback function for when CLI task receives data from USART.
 * This function is called in the USART1 DMA reception complete callback when a full command is received (indicated by \r or \n character).
 * It sends a wakeup reason to the CLI task's wakeup queue to indicate that new data has been received and is ready to be processed.
 * @param None.
 * @return None.
 */
void callback_cli_data_received(void);

/**
 * @brief Callback function for when a CLI command has been executed.
 * This function is called after a command has been executed to notify the CLI task that it can perform any necessary actions after command execution, such as updating the menu header or showing command status.
 * It sends a wakeup reason to the CLI task's wakeup queue to indicate that a command has been executed.
 * @param None.
 * @return None.
 */
void callback_cli_write_executed(void);

/**
 * @brief Show command execution status.
 * This function is called after a command has been executed to show the status of the command execution to the user.
 * For now, it just sends a static message back to the terminal, but in the future, it can be enhanced to show more detailed information about the command execution status.
 * @param None.
 * @return None.
 */
void show_command_status(void);

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
 * @brief CLI task function.
 * This task handles system diagnostics and provides a CLI interface
 * for monitoring system status and performance.
 * @param arg: Argument passed to the task (not used).
 * @return None.
 */
void task_CLI(ULONG arg);

/**
 * @brief Safe string to integer conversion function.
 * This function attempts to convert a string to an integer value while performing error checking to ensure that
 * the conversion is successful and that the resulting value is within a valid range. It returns true if the conversion
 * is successful and the value is valid, and false otherwise.
 * @param str: Pointer to the input string to be converted.
 * @param length: Length of the input string in bytes.
 * @param result: Pointer to a uint8_t variable where the converted integer value will be stored if the conversion is successful.
 * @return true if the conversion is successful and the value is valid, false otherwise.
 * @note This function is designed to be safe and robust, preventing potential issues such as buffer overflows,
 * invalid input formats, and out-of-range values. It should be used whenever there is a need to convert user input from the CLI interface
 * to integer values for further processing, ensuring that the system can handle invalid input gracefully without crashing
 * or exhibiting undefined behavior. The implementation of this function should include checks for valid numeric characters, proper handling
 * of optional leading whitespace and signs, and validation of the resulting integer value against expected ranges for the specific use case.
 * @note It supports conversion of decimal, hexadecimal (with "0x" prefix) and binary (with "0b" prefix) string formats to integer values, providing
 * flexibility in how users can input numeric values in the CLI interface. The function should also handle edge cases such as empty strings,
 * strings with only whitespace, and strings with invalid characters, returning false in those cases to indicate that the conversion was
 * unsuccessful and the input was invalid.
 */

bool safe_atoi(const char*str,uint8_t length, uint8_t*result);

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // TASK_CLI_H_

/**@} */