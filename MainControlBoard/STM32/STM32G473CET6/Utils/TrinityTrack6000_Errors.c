/**
 * @file TrinityTrack6000_Errors.c
 * @brief Implementation of TrinityTrack6000_Errors.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission. 
 */

#include <stdint.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Errors.h>

// Probabbly in the future I will expand error handling

SystemErrorStatus_t g_SystemErrors={0};

void Error_Handler(void){
	// Called only when error is critical and not recoverable
	// MCU should reset after this kind of fault either by external watchdog timer
	// or by software
	__disable_irq();

	while(1){
		// Debug Loop
		// If UART is initialized, we can send error code over UART
		// If GPIO is initialized, we activate buzzer
		// In the future restart of the system
	}
}