/**
 * @file TrinityTrack6000_Init.c
 * @brief Implementation of TrinityTrack6000_Init.h
 * Initialization of HAL, Clock, GPIO, UART and Memory Diagnostics.
 * 
 * @note Some of the initialization functions are named respectively to
 * default STM32CubeMX generated functions to maintain consistency along
 * with their generated comments.
 *  
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <string.h>

#include <TrinityTrack6000_Init.h>
#include <TrinityTrack6000_Pinout.h>
#include <USART3_Dma.h>

extern "C" void ramInfoInit(void);
extern "C" void ramInfoRefresh(void);

const char msg_init_mcu_initialized_info[]         ="[SYS][0x00][  OK  ] System Init: Core + Peripherals Ready\r\n";
const char msg_init_memory_initialized_info[]      ="[SYS][0x01][  OK  ] Memory ready\r\n";
const char msg_init_threadx_startup_info[]         ="[SYS][0xA0][  OK  ] ThreadX starting up...\r\n";

void initializeMemory(void){
// Initialize RAM info variables
	ramInfoInit();
  ramInfoRefresh();
// DMA memory zeroing and testing could be added here
	//usart2_dma_init();
}
