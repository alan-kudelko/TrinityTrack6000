/**
 * @file TrinityTrack6000_Config.c
 * @brief Implementation of TrinityTrack6000_Config.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2026 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <TrinityTrack6000_Config.h>

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CORDIC_HandleTypeDef hcordic;

I2C_HandleTypeDef hi2c2;
DMA_HandleTypeDef hdma_i2c2_tx;
DMA_HandleTypeDef hdma_i2c2_rx;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim20;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

SystemErrorStatus_t g_SystemErrors={0};