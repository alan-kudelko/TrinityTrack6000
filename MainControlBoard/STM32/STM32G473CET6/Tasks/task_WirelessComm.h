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

#define TASK_WIRELESS_COMM_STACK_SIZE 1024 /**< Stack size for wireless comm task */
#define TASK_WIRELESS_COMM_PRIORITY 2     /**< Priority for wireless comm task */

extern const char task_wireless_comm_name[]; /**< Name of the wireless comm task */

extern TX_THREAD task_wireless_comm_handle;
extern ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE];

typedef struct NRF_SETTINGS{
    uint8_t config;
    uint8_t en_aa;
    uint8_t en_rxaddr;
    uint8_t setup_aw;
    uint8_t setup_retr;
    uint8_t rf_ch;
    uint8_t rf_setup;
    uint8_t rx_addr_p0[5];
    uint8_t rx_addr_p1[5];
    uint8_t rx_addr_p2;
    uint8_t rx_addr_p3;
    uint8_t rx_addr_p4;
    uint8_t rx_addr_p5;
    uint8_t tx_addr[5];
    uint8_t rx_pw_p0;
    uint8_t rx_pw_p1;
    uint8_t rx_pw_p2;
    uint8_t rx_pw_p3;
    uint8_t rx_pw_p4;
    uint8_t rx_pw_p5;
    uint8_t dynpd;
    uint8_t feature;
}NRF_SETTINGS;

typedef struct NRF_RUNTIME_STATUS{
    uint8_t status;
    uint8_t fifo_status;
    uint8_t observe_tx;
    uint8_t cd;
}NRF_RUNTIME_STATUS;

typedef struct RADIO_STATS{
    uint32_t packages_received;
    uint32_t packages_dropped;
    uint32_t packages_duplicate;
    uint32_t packages_out_of_order;
    uint32_t channel_busy_events;
}RADIO_STATS;

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