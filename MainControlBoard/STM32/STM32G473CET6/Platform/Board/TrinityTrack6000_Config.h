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

/**
 * @brief CPU Clock speed 170MHz
 * @note Chaning this define will not result in changed MCU's frequency.
 * It this just an information
 */
#define F_CPU 170000000UL

/**
 * @brief UART configuration for TrinityTrack6000.
 * @defgroup UART_Config UART Configuration
 * @{
 */

/** Baud rate for USART1 */
#define USART1_BAUD_RATE 19200

/** Baud rate for USART2 */
#define USART2_BAUD_RATE 115200

/** Timeout for debug UART operations in milliseconds */
#define DEBUG_UART_TIMEOUT 1000

/** @} */

/**
 * @brief SPI configuration for TrinityTrack6000.
 * @defgroup SPI_Config SPI Configuration
 * @{
 */
#define SPI1_RESPONSE_TIMEOUT 1000
#define SPI2_RESPONSE_TIMEOUT 2000

#define SPI1_TIMEOUT_RETRY_COUNT 3
#define SPI2_TIMEOUT_RETRY_COUNT 3

/** @} */

/**
 * @brief I2C configuration for TrinityTrack6000.
 * @defgroup I2C_Config I2C Configuration
 * @{
 */
#define I2C2_RESPONSE_TIMEOUT 1000

#define I2C2_TIMEOUT_RETRY_COUNT 3

/** @} */

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern CORDIC_HandleTypeDef hcordic;

extern I2C_HandleTypeDef hi2c2;
extern DMA_HandleTypeDef hdma_i2c2_tx;
extern DMA_HandleTypeDef hdma_i2c2_rx;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
// Uart Handler used for debugging and logging
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

// ========================
// Global control variables
// ========================
extern SystemErrorStatus_t g_SystemErrors __attribute((section(".sysDiag")));

typedef enum SYSTEM_MODE{DEBUG,TEST,FAILSAFE_FAULT}SYSTEM_MODE;

extern SYSTEM_MODE system_mode __attribute((section(".sysDiag")));

#endif // TRINITY_TRACK6000_CONFIG_H_