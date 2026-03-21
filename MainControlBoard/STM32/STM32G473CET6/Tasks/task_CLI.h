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
#define TASKS_CLI_PRIORITY    5  //!< Priority for CLI task

#define TASK_CLI_RETRY_DELAY_MS 30 //!< Delay in milliseconds before retrying to enqueue data to UART1 DMA if it fails

#define TOKENS_MAX_COUNT 10 //!< Maximum number of tokens in a command
#define COMMANDS_MAX_COUNT 5 //!< Maximum number of supported commands, can be adjusted as needed

#define TASK_CLI_WAKEUP_QUEUE_STORAGE_LENGTH 4 //!< Length of the CLI wakeup queue storage in words (uint32_t), can be adjusted as needed

#define TASK_CLI_RX_BUFFER_SIZE 10 //!< Size of the rx buffer for receiving raw data from devices, can be adjusted as needed
#define TASK_CLI_TX_BUFFER_SIZE 10 //!< Size of the tx buffer for transmitting raw data to devices, can be adjusted as needed

#define TASK_CLI_TEMPORARY_BUFFER_SIZE 31 //!< Size of temporary buffers used in command parsing

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
#define TASK_CLI_WAKEUP_WRITE_EXECUTED 2 //!< Wakeup reason indicating that CLI task was woken up after executing a command, can be used to trigger menu header update or other actions after command execution
#define TASK_CLI_WAKEUP_READ_EXECUTED 3 //!< Wakeup reason indicating that CLI task was woken up after executing a read command, can be used to trigger sending read data back to terminal or other actions after read command execution
#define TASK_CLI_WAKEUP_MODE_CHANGED 4
#define TASK_CLI_WAKEUP_RADIO_STATS_READ 5
#define TASK_CLI_WAKEUP_RADIO_RUNTIME_READ 6
#define TASK_CLI_WAKEUP_RADIO_SETTINGS_READ 7

typedef struct ALIGNED(4) TASK_CLI_WAKEUP_REASON{
    uint8_t wakeupReason;
    uint8_t requestStatus;
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
extern const char command_show_radio[];
extern const char*command_show_children[]; //!< Array of child command strings for the "show" command

extern const char command_show_mem_ram1[]; //!< Child command string for showing RAM1 information
extern const char command_show_mem_ram2[]; //!< Child command string for showing RAM2 information
extern const char command_show_mem_ccsram[]; //!< Child command string for showing CCSRAM information
extern const char*command_show_mem_children[]; //!< Array of child command strings for the "show memory" command

extern const char command_show_radio_stats[];
extern const char command_show_radio_runtime[];
extern const char command_show_radio_settings[];
extern const char*command_show_radio_children[];

extern const char command_write[]; //!< Command string for write operation
extern const char command_read[]; //!< Command string for read operation
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

extern const char msg_task_CLI_run_menu_header[]; //!< Default menu header for CLI task
extern const char msg_task_CLI_test_menu_header[]; //!< Test menu header for CLI task
extern const char msg_task_CLI_failsafe_menu_header[];
extern const char msg_task_CLI_fault_menu_header[];
extern const char msg_task_CLI_unknown_command[]; //!< Message indicating unknown command received in CLI task
extern const char msg_task_CLI_mode_switched_to_diag[]; //!< Message indicating that CLI mode has been switched to diagnostics mode
extern const char msg_task_CLI_mode_switched_to_test[]; //!< Message indicating that CLI mode has been switched to test mode
extern const char msg_task_CLI_mode_switch_failed[]; //!< Message indicating that CLI mode switch failed with correct usage hint

extern const char msg_task_CLI_write_executed[]; //!< Message indicating that a write command has been executed
extern const char msg_task_CLI_read_executed[]; //!< Message indicating that a read command has been executed
extern const char msg_task_CLI_read_executed_format_string[]; //!< Format string for message indicating read register and its value
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
 * 
 */
void parse_command_read(uint8_t argc,char*argv[]);

/**
 * @brief Build a bus command based on the received CLI command arguments.
 * This function takes the parsed command arguments and constructs a bus command that can be sent to `SystemDispatcher` 
 * for execution. It validates the command arguments, constructs the appropriate command structure, and sends it to the
 * `SystemDispatcher` via a queue for further processing.
 * @param argc: Number of arguments in the command (including the main command and child command
 * and any additional parameters needed for the bus command).
 * @param argv: Array of strings representing the command and its arguments, where argv[0]
 * @return true if the bus command was successfully built and false otherwise
 * @note This function is meant to be called within `parse_command_write` or `parse_command_read`
*/
bool build_bus_command(uint8_t argc,char*argv[]);

/**
 * @brief Callback function for when CLI task receives data from USART.
 * This function is called in the USART1 DMA reception complete callback when a full command is received (indicated by \r or \n character).
 * It sends a wakeup reason to the CLI task's wakeup queue to indicate that new data has been received and is ready to be processed.
 * @param None.
 * @return None.
 */
void callback_cli_data_received(uint8_t event);

/**
 * @brief Callback function for when a CLI command has been executed.
 * This function is called after a command has been executed to notify the CLI task that it can perform any necessary actions after command execution, such as updating the menu header or showing command status.
 * It sends a wakeup reason to the CLI task's wakeup queue to indicate that a command has been executed.
 * @param None.
 * @return None.
 */
void callback_cli_write_executed(uint8_t event);

/**
 * @brief Callback function for when a CLI read command has been executed.
 * This function is called after a read command has been executed to notify the CLI task that it can perform any necessary actions after read command execution, such as sending read data back to terminal or showing command status.
 * It sends a wakeup reason to the CLI task's wakeup queue to indicate that a read command has been executed.
 * @param None.
 * @return None.
 */
void callback_cli_read_executed(uint8_t event);

/**
 * @brief Show command execution status.
 * This function is called after a command has been executed to show the status of the command execution to the user.
 * For now, it just sends a static message back to the terminal, but in the future, it can be enhanced to show more detailed information about the command execution status.
 * @param uint32_t wakeupReason: The reason for the wakeup event that triggered the status display..
 * @return None.
 */
void show_command_status(uint32_t wakeupReason);



void show_command_status_switch_mode(void);

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

#ifdef __cplusplus
    }
#endif // __cplusplus


#endif // TASK_CLI_H_

/**@} */