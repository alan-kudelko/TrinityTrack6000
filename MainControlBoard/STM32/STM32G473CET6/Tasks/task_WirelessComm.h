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

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <tx_api.h>

#include <stdint.h>

#include <NRF24L01.h>
#include <TrinityTrack6000_RadioConfig.h>

#define OPERATION_BUFFER_SIZE 33

#define TASK_WIRELESS_COMM_STACK_SIZE 512 /**< Stack size for wireless comm task */
#define TASK_WIRELESS_COMM_PRIORITY 2     /**< Priority for wireless comm task */

extern const char task_wireless_comm_name[]; /**< Name of the wireless comm task */

extern const struct NRF24L01_REGS nrf24l01_default_regs;

extern TX_THREAD task_wireless_comm_handle;
extern ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE];

extern uint32_t test_data[10];
extern uint16_t test_data_length;

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

extern void radioDataReceived_callback(void);

extern void nrf24l01_init(void);

extern void task_wireless_comm_init(void);

extern void task_wireless_comm(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_WIRELESS_COMM_H_