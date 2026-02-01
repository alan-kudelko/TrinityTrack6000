/**
 * @file task_diagnostics.c
 * @brief Implementation of a diagnostics task for system monitoring with CLI interface.
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

#include <task_diagnostics.h>

#include <TrinityTrack6000_Pinout.h>

const char task_diagnostics_name[]="Diagnostics Task";
TX_THREAD task_diagnostics_handle;
ULONG task_diagnostics_stack[TASK_DIAGNOSTICS_STACK_SIZE];

void task_diagnostics(ULONG arg){
    UNUSED(arg);
    while(1){
        HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
        // For now just blink a diode to indicate task is running
        //HAL_GPIO_TogglePin(DIAG_LED_GPIO_Port,DIAG_LED_Pin);
        tx_thread_sleep(1000);
    }
}