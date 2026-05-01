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

#include <TrinityTrack6000_Errors.h>

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern CORDIC_HandleTypeDef hcordic;

extern I2C_HandleTypeDef hi2c2;
extern DMA_HandleTypeDef hdma_i2c2_tx;
extern DMA_HandleTypeDef hdma_i2c2_rx;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim20;

extern UART_HandleTypeDef huart2;
// Uart Handler used for debugging and logging
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

// ========================
// Global control variables
// ========================
extern SystemErrorStatus_t g_SystemErrors __attribute((section(".sysDiag")));

#endif // TRINITY_TRACK6000_CONFIG_H_