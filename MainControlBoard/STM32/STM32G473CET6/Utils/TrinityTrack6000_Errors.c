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

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Errors.h>

void Error_Handler(void){
	// Called only when error is CRITICAL AND NOT RECOVERABLE
	// Non critical errors should be handled inside main or during
	// ThreadX kernel operation
	
	// MCU should reset after this kind of fault either by external watchdog timer
	// or by software
	__disable_irq();

	while(1){
		// Debug Loop
		// If GPIO is initialized, we activate buzzer
	}
}