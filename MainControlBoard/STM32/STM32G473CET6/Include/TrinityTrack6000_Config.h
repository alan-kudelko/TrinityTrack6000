/**
 * @file TrinityTrack6000_Config.h
 * @brief Configuration header for TrinityTrack6000 project.
 * 
 * This file contains configuration settings for the TrinityTrack6000
 * project, including UART parameters and other system-wide settings.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TRINITY_TRACK6000_CONFIG_H_
	#define TRINITY_TRACK6000_CONFIG_H_

#include <stm32g4xx_hal.h>

// ========================
// UART Configuration
// ========================
#define USART1_BAUD_RATE 115200;
#define USART2_BAUD_RATE 115200;

extern ADC_HandleTypeDef hadc1;

extern CORDIC_HandleTypeDef hcordic;

extern I2C_HandleTypeDef hi2c2;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim8;
// Uart Handler used for debugging and logging
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


	
#endif // TRINITY_TRACK6000_CONFIG_H_