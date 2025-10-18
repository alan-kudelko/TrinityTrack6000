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

uint32_t global_error_code=0;

void Error_Handler(void){
	__disable_irq();
	while(1){
		// Debug Loop
		// In the future restart of the system
	}
}