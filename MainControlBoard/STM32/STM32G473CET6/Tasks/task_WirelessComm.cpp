/**
 * @file task_WirelessComm.cpp
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

#include <stm32g4xx_hal.h>

#include <task_WirelessComm.h>
#include <NRF24L01.h>

const char task_wireless_comm_name[]="Wireless Task";
TX_THREAD task_wireless_comm_handle SECTION(".task_handles");
ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE] SECTION(".task_stacks_ccsram");

uint32_t test_data[10]{0};
uint16_t test_data_length=10;

static NRF24L01 nrf24l01;

extern "C" void task_wireless_comm_init(void){
    // Placeholder for future initialization
}

extern "C" void task_wireless_comm(ULONG arg){
    UNUSED(arg);

// For now just for architecture testing
// In the future body of this task will be dedicated to handle wireless rx/tx communication

    while(true){

        tx_thread_sleep(1000);
    }
}