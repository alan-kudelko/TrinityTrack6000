/**
 * @file task_WirelessComm.h
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

// For now task emulating communication with NRF24L01

#ifndef TASK_WIRELESS_COMM_H_
    #define TASK_WIRELESS_COMM_H_

#include <tx_api.h>

#include <stdint.h>

#define TASK_WIRELESS_COMM_STACK_SIZE 512 /**< Stack size for wireless comm task */
#define TASK_WIRELESS_COMM_PRIORITY 2     /**< Priority for wireless comm task */

extern const char task_wireless_comm_name[]; /**< Name of the wireless comm task */

extern TX_THREAD task_wireless_comm_handle __attribute((section(".task_handles.taskWirelessComm")));
extern ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE] __attribute((section(".task_stacks.taskWirelessComm")));

extern uint32_t test_data[10];
extern uint16_t test_data_length;

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

extern void task_wireless_comm_init(void);

extern void task_wireless_comm(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_WIRELESS_COMM_H_