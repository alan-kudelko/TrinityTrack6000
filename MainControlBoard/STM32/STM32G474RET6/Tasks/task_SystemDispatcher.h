/**
 * @file task_SystemDispatcher.h
 * @brief 
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

// Doxy z opisem co to ma robic

#ifndef TASK_SYSTEM_DISPATCHER_H_
    #define TASK_SYSTEM_DISPATCHER_H_

#include <tx_api.h>

#include <system_commands.h>
#include <system_types.h>

#define TASKS_SYSTEM_DISPATCHER_STACK_SIZE 1024  //!< Stack size for System Dispatcher task
#define TASKS_SYSTEM_DISPATCHER_PRIORITY 1      //!< Priority for System Dispatcher task

#define TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH 10 /**< Length of queue in words (uint32_t) */

#define TASK_CLI_COMMAND_QUEUE_TIMEOUT 1000 /**< Timeout  */
#define TASK_WIRELESS_COMM_TIMEOUT 50 /**< Timeout */

extern const char task_SystemDispatcher_name[];  /**< Name of the System Dispatcher task */

extern TX_THREAD task_SystemDispatcher_handle;
extern ULONG task_SystemDispatcher_stack[TASKS_SYSTEM_DISPATCHER_STACK_SIZE];

extern TX_QUEUE task_cli_request_queue;
extern ULONG task_cli_request_queue_storage[TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest)/sizeof(uint32_t)];

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void failsafe_stop(void);

void fault_stop(void);

void parse_cli_queue(SystemRequest*command);

void parse_cli_queue_request_set_value(SystemRequest*command);

void parse_cli_queue_request_bus_raw_data(SystemRequest*command);

void parse_cli_queue_request_switch_mode(SystemRequest*command);

void parse_cli_queue_request_radio_stats(SystemRequest*command);

void task_SystemDispatcher_init(void);

void task_SystemDispatcher(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_SYSTEM_DISPATCHER_H_