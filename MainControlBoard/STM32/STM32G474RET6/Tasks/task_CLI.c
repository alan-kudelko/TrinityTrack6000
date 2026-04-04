/**
 * @addtogroup task_CLI
 * @{
 */

// In the near future create internal helper function for enqueuing data to UART1 DMA with retry
// This function should divide longer messages into smaller chunks if necessary and handle retries

// Also I should add history of typed commands after implementing helper function
// Fix: Differentiate between write and read operation when it comes to error messages and hints in `build_bus_command`

// Add change mode command to the system commands and implement it in the system dispatcher
// Note: struct TASL_CLI_COMMAND is modified to accomodate the mode switch command

#include <stm32g4xx_hal.h>
#include <string.h>
#include <stdio.h>

#include <task_CLI.h>

#include <TrinityTrack6000_Pinout.h>

#include <USART3_Dma.h>
#include <TrinityTrack6000_MemInfo.h>

#include <system_commands.h>

const char task_CLI_name[]="CLI Task";

TX_THREAD task_CLI_handle SECTION(".task_handles");

ULONG task_CLI_stack[TASK_CLI_STACK_SIZE];

static TX_QUEUE task_cli_wakeup_queue;
static ULONG task_cli_wakeup_queue_storage[TASK_CLI_WAKEUP_QUEUE_STORAGE_LENGTH*sizeof(TASK_CLI_WAKEUP_REASON)/sizeof(uint32_t)];

extern enum SYSTEM_MODE system_mode;

extern TX_QUEUE task_cli_request_queue;

extern bool safe_atoi(const char*str,uint8_t length,uint8_t*result);

/**
 * @addtogroup task_CLI
 * @{
 */

const char command_help[]="help";

const char command_switch_mode[]="mode";
const char command_switch_mode_diag[]="diag";
const char command_switch_mode_test[]="test";
const char*command_switch_mode_children[]={command_switch_mode_diag,command_switch_mode_test};

const char command_show[]="show";
const char command_show_mem[]="memory";
const char command_show_radio[]="radio";
const char*command_show_children[]={command_show_mem,command_show_radio};

const char command_show_mem_ram1[]="ram1";
const char command_show_mem_ram2[]="ram2";
const char command_show_mem_ccsram[]="ccsram";
const char*command_show_mem_children[]={command_show_mem_ram1,command_show_mem_ram2,command_show_mem_ccsram};

const char command_show_radio_stats[]="stats";
const char command_show_radio_runtime[]="runtime";
const char command_show_radio_settings[]="settings";
const char*command_show_radio_children[]={command_show_radio_stats,command_show_radio_runtime,command_show_radio_settings};

const char command_write[]="write";
const char command_read[]="read";

/**@} */

const char*command_array[COMMANDS_MAX_COUNT]={
    command_help,
    command_switch_mode,
    command_show,
    command_write,
    command_read};

const void (*parse_functions[COMMANDS_MAX_COUNT])(uint8_t,char*[])={
    parse_command_help,
    parse_command_switch_mode,
    parse_command_show,
    parse_command_write,
    parse_command_read};

/*
    For now let's assume that commands are divided into 2 categories
    diagnostics commands and test commands
*/

/**
 * @ingroup task_CLI
 * @{
 */
const char msg_task_CLI_help[]=
"[CLI] Available commands:\r\n\
      help - Show this help message\r\n\
      history - Show command history\r\n\
      mode <diag/test> - Switch CLI mode between diagnostics and test menu\r\n\
      show - Show various system information. Use help show for more details\r\n\
      write <device_id> <op-code> <register> <value> - Write device's raw data. Use help write for more details\r\n\
      write <hardware_id> <value> - Write value to a hardware component. Use help write for more details\r\n"
      ;

const char msg_task_CLI_help_show[]=
"[CLI] Available show commands:\r\n\
      show memory - <ram1/ram2/ccsram> Show memory information\r\n\
      show radio - <stats/runtime/settings> Show radio information\r\n"
      ;

const char msg_task_CLI_help_write[]=
"[CLI] Avaible write devices:\r\n\
      write <device_id> <op-code> <register> <value>\r\n\
      1  DEVICE_MCP1\r\n\
      2  DEVICE_MCP2\r\n\
      3  DEVICE_NRF24L01\r\n\
      4  DEVICE_ADXL345\r\n\
      5  DEVICE_GPS\r\n\
      6  DEVICE_MCP23017\r\n\
      7  DEVICE_FRAM\r\n\
      8  DEVICE_INFINEON\r\n\
      9  DEVICE_NXP\r\n\
      10 DEVICE_RENESANS\r\n\
      11 DEVICE_FPGA\r\n"
;

const char msg_task_CLI_run_menu_header[]="[CLI] DIAG> ";
const char msg_task_CLI_test_menu_header[]="[CLI] TEST> ";
const char msg_task_CLI_failsafe_menu_header[]="[CLI] FAILSAFE> ";
const char msg_task_CLI_fault_menu_header[]="[CLI] FAULT> ";
const char msg_task_CLI_unknown_command[]="[CLI] Unknown command. Type help for available commands\r\n";

const char msg_task_CLI_mode_switched_to_diag[]="Switched to diagnostics mode\r\n";
const char msg_task_CLI_mode_switched_to_test[]="Switched to test mode\r\n";
const char msg_task_CLI_mode_switch_failed[]="Failed to switch mode\r\n";

const char msg_task_CLI_write_executed[]="Write command executed\r\n";
const char msg_task_CLI_read_executed[]="Read command executed\r\n";
const char msg_task_CLI_read_executed_format_string[]="    Register 0x%02X: 0x%02X\r\n";

const char msg_task_CLI_show_radio_stats_header[]="Radio stats\r\n";
const char msg_task_CLI_show_radio_stats_total_packages_format_string[]="Total packages %zu\r\n";
const char msg_task_CLI_show_radio_stats_received_packages_format_string[]="Received packages %zu\r\n";
const char msg_task_CLI_show_radio_stats_dropped_packages_format_string[]="Dropped packages %zu\r\n";
const char msg_task_CLI_show_radio_stats_duplicate_packages_format_string[]="Duplicated packages %zu\r\n";
const char msg_task_CLI_show_radio_stats_out_of_order_packages_format_string[]="Out of order packages %zu\r\n";
/**@} */

/**
 * @ingroup task_CLI
 * @{
 * @name CLI Task variables
 * @brief Internal variables used by the CLI task for write/read operations executed in the test menu
 * These variables are used to store the parameters of the write/read operations
 * received from the terminal and to track the status of those operations
 * They are not used in the diagnostics menu, but they can be used in the test menu
 * to perform write/read operations to devices and to provide feedback about the status of those operations back to the terminal
 * For instance, when a write command is received in the test menu, the CLI task can
 * store the parameters of that command (e.g., device ID, register address, value to write) in these variables, then it can execute the write operation and update the commandStatus variable based on the result of that operation
 * After that, it can send a message back to the terminal indicating whether the write operation was successful or if it failed with an error, and it can also provide details about the error if it failed
 * Similarly, when a read command is received in the test menu, the CLI task can execute the read operation and store the read data in the rxBuffer variable, then it can send that data back to the terminal along with a message indicating whether the read operation was successful or if it failed with an error
 * These variables can also be used to implement more complex test scenarios in the test menu where multiple write/read operations are performed in sequence and the status of each operation is tracked and reported back to the terminal
 * For instance, we can implement a test scenario where we write a value to a device, then read it back to verify that the write operation was successful, and we can report the status of each step back to the terminal
 * Overall, these variables are essential for enabling interactive testing and diagnostics through the CLI interface in the
 * test menu, allowing users to perform write/read operations and receive feedback about the status of those operations in real-time
 * Note: Actual communication is delegated to `task_SystemDispatcher` and dedicated tasks handling communication with specific devices
 */

static uint8_t txBuffer[TASK_CLI_TX_BUFFER_SIZE];
static uint8_t rxBuffer[TASK_CLI_RX_BUFFER_SIZE];
static uint8_t commandStatus;

static SystemRequest task_cli_SystemRequest;

static char tempBuffer[TASK_CLI_TEMPORARY_BUFFER_SIZE]; // Temporary buffer for snprintf operations

static RADIO_STATS radio_stats;

static RADIO_RUNTIME_STATS radio_runtime_stats;

//static NRF_SETTINGS nrf_settings;

/**@} */

void task_CLI_init(void){
    // Initialize queue for waking up the CLI task when a command is received from the terminal
    tx_queue_create(&task_cli_wakeup_queue,
        "CLI Wakeup",
        sizeof(TASK_CLI_WAKEUP_REASON)/sizeof(uint32_t),
        task_cli_wakeup_queue_storage,
        TASK_CLI_WAKEUP_QUEUE_STORAGE_LENGTH*sizeof(TASK_CLI_WAKEUP_REASON));

    // Initialize command structure with default values for testing purposes
    task_cli_SystemRequest.commandType=REQUEST_SET_VALUE;
    task_cli_SystemRequest.payload.set.hardwareId=HARDWARE_MOTOR1;
    task_cli_SystemRequest.payload.set.value=0;
    task_cli_SystemRequest.commandStatus=&commandStatus;
    task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;

    UNUSED(task_cli_SystemRequest);
    UNUSED(commandStatus);
    UNUSED(rxBuffer);
    UNUSED(txBuffer);

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
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
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
        if(strncmp(tokens[0],command_array[i],strlen(command_array[i]))==0){
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
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
    }
}

void parse_command_help(uint8_t argc,char*argv[]){
    // This function is called when the "help" command is received in the CLI interface
    // It provides information about available commands and their usage to the user
    if(argc>1){
        // If there are more than 1 arguments, we can check if the second argument is a valid command to provide more specific help information about that command
        if(strncmp(argv[1],command_show,strlen(command_show))==0){
            // User requested help for the "show" command, send help information for the "show" command
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_show,strlen(msg_task_CLI_help_show))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            return;
        }
        else if(strncmp(argv[1],command_write,strlen(command_write))==0){
            // User requested help for the "write" command, send help information for the "write" command
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            return;
        }
        else{
            // Invalid command provided as an argument, send error message back to the terminal
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            return;
        }
    }

    while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help,strlen(msg_task_CLI_help))!=true){
        tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
    }
}

void parse_command_switch_mode(uint8_t argc,char*argv[]){
    // This function is called when the "mode" command is received
    // It parses the child command to determine which mode to switch to (diagnostics or test)

    if((argc<2)||(argc>2)){
        // Invalid number of arguments, send hint back to terminal indicating correct usage of the command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_mode_switch_failed,strlen(msg_task_CLI_mode_switch_failed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
        return;
    }
    // Number of arguments is valid, now check the value of the child command
    if(strncmp(argv[1],command_switch_mode_diag,strlen(command_switch_mode_diag))==0){
        // Send request to change mode to run
        task_cli_SystemRequest.commandType=REQUEST_SWITCH_MODE;
        task_cli_SystemRequest.payload.mode.mode=SYSTEM_MODE_RUN;
        task_cli_SystemRequest.callbackEvent=TASK_CLI_WAKEUP_MODE_CHANGED;
        task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;
        tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,TX_WAIT_FOREVER);
    }
    else if(strncmp(argv[1],command_switch_mode_test,strlen(command_switch_mode_test))==0){
        // Send request to change mode to test
        task_cli_SystemRequest.commandType=REQUEST_SWITCH_MODE;
        task_cli_SystemRequest.payload.mode.mode=SYSTEM_MODE_TEST;
        task_cli_SystemRequest.callbackEvent=TASK_CLI_WAKEUP_MODE_CHANGED;
        task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;
        tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,TX_WAIT_FOREVER);
    }
    else{
        // Invalid child command, send hint back to terminal indicating correct usage of the command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_mode_switch_failed,strlen(msg_task_CLI_mode_switch_failed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
    }
    // Remember that when mode is set to test mode, this task should inform
    // execuite task to disable control
}

void parse_command_show(uint8_t argc,char*argv[]){
    // This function is called when the "show" command is received
    // It parses the child command to determine what information to show
    // Possible the most complex command to implement due to ambiguity of the "show" command,
    // For instance "show mem" command can have multiple child commands to specify which memory
    // information to show, so we need to implement a more complex parsing logic to handle that
    // For now just a placeholder implementation that sends a static message back to the terminal

    if(strncmp(argv[1],command_show_mem,strlen(command_show_mem))==0){
        // Show memory information
        if(argc>2){
            // Check which memory information to show based on the child command
            if(strncmp(argv[2],command_show_mem_ram1,strlen(command_show_mem_ram1))==0){
                // Show RAM1 information
                ramInfoRAM1(tx_thread_sleep,TASK_CLI_RETRY_DELAY_MS);
            }
            else if(strncmp(argv[2],command_show_mem_ram2,strlen(command_show_mem_ram2))==0){
                // Show RAM2 information
                ramInfoRAM2(tx_thread_sleep,TASK_CLI_RETRY_DELAY_MS);
            }
            else if(strncmp(argv[2],command_show_mem_ccsram,strlen(command_show_mem_ccsram))==0){
                // Show CCSRAM information
                ramInfoCCSRAM(tx_thread_sleep,TASK_CLI_RETRY_DELAY_MS);
            }
            else{
                // Invalid child command, send hint back to terminal indicating correct usage of the command
                while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
                    tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
                }
            }
        }
        else{
            // No child command provided, show general memory information
            ramInfoGeneral(tx_thread_sleep,TASK_CLI_RETRY_DELAY_MS);
        }

    }
    else if(strncmp(argv[1],command_show_radio,strlen(command_show_radio))==0){
        // Show radio information
        if(argc>2){
            // Check which information to show based on the child command
            if(strncmp(argv[2],command_show_radio_stats,strlen(command_show_radio_stats))==0){
                // Radio stats
                task_cli_SystemRequest.commandType=REQUEST_GET_RADIO_STATS;
                task_cli_SystemRequest.payload.radioStats=&radio_stats;
                task_cli_SystemRequest.callbackEvent=TASK_CLI_WAKEUP_RADIO_STATS_READ;
                task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;
                tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,0);
            }
            else if(strncmp(argv[2],command_show_radio_runtime,strlen(command_show_radio_runtime))==0){
                // Radio runtime stats from device
                task_cli_SystemRequest.commandType=REQUEST_GET_RADIO_RUNTIME_STATS;
                task_cli_SystemRequest.payload.radioRuntimeStats=&radio_runtime_stats;
                task_cli_SystemRequest.callbackEvent=TASK_CLI_WAKEUP_RADIO_RUNTIME_READ;
                task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;
                tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,0);                
            }
            else if(strncmp(argv[2],command_show_radio_settings,strlen(command_show_radio_settings))==0){
                // Radio settings
            }
            else{
                // Invalid child command, send hint back to terminal indicating correct usage of the command
                while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command))!=true){
                    tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
                }                
            }
        }
    }
    else{
        // Too few arguments or invalid child command, send hint back to terminal indicating correct usage of the command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_show,strlen(msg_task_CLI_help_show))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
    }
}

void parse_command_write(uint8_t argc,char*argv[]){
    // This function is called when the "write" command is received in the CLI interface
    // It parses the child command to determine which device to write to and what data to write
    // Note: some devices are not using op-codes so it's possible to send just a register and value
    if(build_bus_command(argc,argv)!=true){
        // Failed to build command, no need to send error message since build_bus_command already
        // sends hint messages back to the terminal in case of failure
        return;
    }
        // For now just send data to first device to the implementation
        //txBuffer[0]=1<<6;
        //txBuffer[1]=0x12;
        //txBuffer[2]=0x00;

    // Write operation and its callback function

    task_cli_SystemRequest.payload.rawData.rxBuffer=NULL;
    task_cli_SystemRequest.payload.rawData.rxLength=0;
    task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;

    tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,TX_WAIT_FOREVER);
}

void parse_command_read(uint8_t argc,char*argv[]){
    if(build_bus_command(argc,argv)!=true){
        // Failed to build command, no need to send error message since build_bus_command already
        // sends hint messages back to the terminal in case of failure
        return;
    }
    // Read operation and its callback function

    task_cli_SystemRequest.payload.rawData.rxBuffer=rxBuffer;
    task_cli_SystemRequest.payload.rawData.rxLength=1; // At least for now we can assume that we are only reading 1 byte
    task_cli_SystemRequest.callbackFn=task_cli_wakeup_callback;

    tx_queue_send(&task_cli_request_queue,&task_cli_SystemRequest,TX_WAIT_FOREVER);
}

bool build_bus_command(uint8_t argc,char*argv[]){
    if((argc<4)||(argc>5)){
        // Invalid number of arguments, send help information for the "write" command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
        return false;
    }
    // Try to convert argv[1] to a valid device ID (integer value is required)
    if(safe_atoi(argv[1],strlen(argv[1]),&task_cli_SystemRequest.payload.rawData.deviceId)!=true){
        // Invalid device ID, send help information for the "write" command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
        return false;
    }
    // Try to convert argv[2] to a valid op-code (integer value is required, value can be in decimal or hexadecimal format)
    if(safe_atoi(argv[2],strlen(argv[2]),&txBuffer[0])!=true){
        // Invalid op-code, send help information for the "write" command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
        return false;
    }
    // Try to convert argv[3] to a register address (integer value is required, value can be in decimal or hexadecimal format)
    if(safe_atoi(argv[3],strlen(argv[3]),&txBuffer[1])!=true){
        // Invalid register address, send help information for the "write" command
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
        }
        return false;
    }
    // Try to convert argv[4] to a value to write (integer value is required, value can be in decimal or hexadecimal format)
    if(argc>4){
        if(safe_atoi(argv[4],strlen(argv[4]),&txBuffer[2])!=true){
            // Invalid value to write, send help information for the "write" command
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_help_write,strlen(msg_task_CLI_help_write))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            return false;
        }
    }
    
    task_cli_SystemRequest.commandType=REQUEST_BUS_RAW_DATA;
    task_cli_SystemRequest.payload.rawData.txBuffer=txBuffer;
    task_cli_SystemRequest.payload.rawData.txLength=argc-2;
    task_cli_SystemRequest.commandStatus=&commandStatus;

    return true;
}

void callback_cli_data_received(){
    TASK_CLI_WAKEUP_REASON wakeup;
    wakeup.wakeupReason=TASK_CLI_WAKEUP_USART_DATA;
    tx_queue_send(&task_cli_wakeup_queue,&wakeup,0);
}

void task_cli_wakeup_callback(uint8_t event){
    TASK_CLI_WAKEUP_REASON wakeup;
    wakeup.wakeupReason=event;
    tx_queue_send(&task_cli_wakeup_queue,&wakeup,0);
}

void show_command_status_write(void){
    if(*task_cli_SystemRequest.commandStatus==SYSTEM_REQUEST_STATUS_OK){
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_write_executed,strlen(msg_task_CLI_write_executed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
        }
    }
    else{
        // Failed to execute write
    }
}

void show_command_status_read(void){
    if(*task_cli_SystemRequest.commandStatus==SYSTEM_REQUEST_STATUS_OK){
        while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_read_executed,strlen(msg_task_CLI_read_executed))!=true){
            tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
        }
        // Display read data from rxBuffer variable
        if(task_cli_SystemRequest.payload.rawData.txLength==3){
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_read_executed_format_string,
                task_cli_SystemRequest.payload.rawData.txBuffer[1], // Register address is at index 1 in the txBuffer)
                task_cli_SystemRequest.payload.rawData.rxBuffer[0]);

            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }

            while(usart3_dma_enq_data((uint8_t*)"\r\n",strlen("\r\n"))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
        }
        else if(task_cli_SystemRequest.payload.rawData.txLength==2){
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_read_executed_format_string,
                task_cli_SystemRequest.payload.rawData.txBuffer[0], // Register address is at index 1 in the txBuffer)
                task_cli_SystemRequest.payload.rawData.rxBuffer[0]);

            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }

            while(usart3_dma_enq_data((uint8_t*)"\r\n",strlen("\r\n"))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
        }
    }
    else{
        // Failed to execute read
    }
}

void show_command_status_switch_mode(void){
    switch(*task_cli_SystemRequest.commandStatus){
        case SYSTEM_REQUEST_STATUS_OK:
            // Mode switched
            if(task_cli_SystemRequest.payload.mode.mode==SYSTEM_MODE_RUN){
                while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_mode_switched_to_diag,strlen(msg_task_CLI_mode_switched_to_diag))!=true){
                    tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
                }
            }
            else if(task_cli_SystemRequest.payload.mode.mode==SYSTEM_MODE_TEST){
                while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_mode_switched_to_test,strlen(msg_task_CLI_mode_switched_to_test))!=true){
                    tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
                }
            }
        break;
        case SYSTEM_REQUEST_STATUS_ERROR:
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_mode_switch_failed,strlen(msg_task_CLI_mode_switch_failed))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
        break;
        default:
            // Failed to switch mode
    }
}

void show_command_status_radio_stats(void){
    switch(*task_cli_SystemRequest.commandStatus){
        case SYSTEM_REQUEST_STATUS_OK:
            while(usart3_dma_enq_data((uint8_t*)msg_task_CLI_show_radio_stats_header,strlen(msg_task_CLI_show_radio_stats_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA                
            }
            // Display total packages
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_show_radio_stats_total_packages_format_string,
                (size_t)radio_stats.total_packages);
            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
            // Display received packages
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_show_radio_stats_received_packages_format_string,
                (size_t)radio_stats.packages_received);
            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
            // Display dropped packages
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_show_radio_stats_dropped_packages_format_string,
                (size_t)radio_stats.packages_dropped);
            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
            // Display duplicate packages
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_show_radio_stats_duplicate_packages_format_string,
                (size_t)radio_stats.packages_duplicate);
            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
            // Display out of order packages
            memset(tempBuffer,0,sizeof(tempBuffer));
            snprintf(tempBuffer,
                TASK_CLI_TEMPORARY_BUFFER_SIZE,
                msg_task_CLI_show_radio_stats_out_of_order_packages_format_string,
                (size_t)radio_stats.packages_out_of_order);
            while(usart3_dma_enq_data((uint8_t*)tempBuffer,strlen(tempBuffer))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS);
            }
        break;
        case SYSTEM_REQUEST_STATUS_ERROR:

        break;
    }
}

void display_menu_header(void){
    // Display menu header based on current FSM state
    switch(system_mode){
        case SYSTEM_MODE_RUN:
            while(usart3_dma_enq_data((const uint8_t*)msg_task_CLI_run_menu_header,strlen(msg_task_CLI_run_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            break;
        case SYSTEM_MODE_TEST:
            while(usart3_dma_enq_data((const uint8_t*)msg_task_CLI_test_menu_header,strlen(msg_task_CLI_test_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
            break;
        case SYSTEM_MODE_FAILSAFE:
            while(usart3_dma_enq_data((const uint8_t*)msg_task_CLI_failsafe_menu_header,strlen(msg_task_CLI_failsafe_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
        break;
        case SYSTEM_MODE_FAULT:
            while(usart3_dma_enq_data((const uint8_t*)msg_task_CLI_fault_menu_header,strlen(msg_task_CLI_fault_menu_header))!=true){
                tx_thread_sleep(TASK_CLI_RETRY_DELAY_MS); // Sleep for a while before retrying to enqueue data to UART3 DMA
            }
        break;
    }
}

void task_CLI(ULONG arg){
    UNUSED(arg);
    char usart_rx_buffer[UART3_DMA_RX_RING_BUFFER_SIZE]={0};
    uint16_t usart_rx_length=0;

    TASK_CLI_WAKEUP_REASON wakeup={0};

    uint8_t raw_tx_data;
    uint16_t raw_tx_data_length;

    uint8_t raw_rx_data;
    uint16_t raw_rx_data_length;

    UNUSED(raw_tx_data);
    UNUSED(raw_tx_data_length);
    UNUSED(raw_rx_data);
    UNUSED(raw_rx_data_length);

    while(1){
        // Display menu header based on current FSM state
        display_menu_header();
        // Wait for command ready semaphore // Change to queue
        // This semaphore is given in the USART3 DMA reception complete callback when a full command is received (indicated by \r or \n character)
        tx_queue_receive(&task_cli_wakeup_queue,&wakeup,TX_WAIT_FOREVER);

        switch(wakeup.wakeupReason){
            // Called only by callback_cli_data_received (status checking can be ommited)
            case TASK_CLI_WAKEUP_USART_DATA:
                memset(usart_rx_buffer,0,sizeof(usart_rx_buffer)); // Clear rx buffer before reading new command
                if(usart3_dma_read_data((uint8_t*)usart_rx_buffer,&usart_rx_length,UART3_DMA_RX_RING_BUFFER_SIZE-1)){
                    // Data is valid
                    // Parse command and execute corresponding actions
                    parse_command(usart_rx_buffer,usart_rx_length);
                    // This call should be FSM state depended, for instance commands used in diagnostics menu
                    // may not be available in test menu and vice versa, so we can check the current FSM state and parse commands accordingly
                }
                else{
                    // Data corrupted or error
                    // Also possible that command is empty
                    // For instance pressing enter without typing anything will result in \r or \n character being received and treated as a command, but there is no actual command to parse
                    // In this case we can choose to ignore it or send an error message back to the terminal
                    // For now just send an error message back to the terminal
                    //usart3_dma_enq_data((const uint8_t*)msg_task_CLI_unknown_command,strlen(msg_task_CLI_unknown_command));
                }
            break;
                // On the other hand these command can be finished with or without success
            case TASK_CLI_WAKEUP_WRITE_EXECUTED:
                show_command_status_write();
            break;
            case TASK_CLI_WAKEUP_READ_EXECUTED:
                show_command_status_read();
                //show_command_status(wakeup.wakeupReason);
            break;
            case TASK_CLI_WAKEUP_MODE_CHANGED:
                show_command_status_switch_mode();
            break;
            case TASK_CLI_WAKEUP_RADIO_STATS_READ:
                show_command_status_radio_stats();
            break;
            case TASK_CLI_WAKEUP_RADIO_RUNTIME_READ:

            break;
            case TASK_CLI_WAKEUP_RADIO_SETTINGS_READ:

            break;

            default:
            // Should not happen
        }
    }
}


/**@} */