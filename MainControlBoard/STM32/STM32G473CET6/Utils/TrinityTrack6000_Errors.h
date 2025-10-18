/**
 * @file TrinityTrack6000_Errors.h
 * @brief Error codes definitions for TrinityTrack6000 project.
 * This module defines error codes used throughout the TrinityTrack6000
 * project to standardize error handling and reporting.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TRINITY_TRACK6000_ERRORS_H_
    #define TRINITY_TRACK6000_ERRORS_H_

/**
 * @name Error Codes
 * @{
 */

#define ERROR_HAL_PWREx_ControlVoltageScaling 0x100 /**<  */
#define ERROR_HAL_RCC_OscConfig               0x101 /**<  */
#define ERROR_HAL_RCC_ClockConfig             0x102 /**<  */
#define ERROR_HAL_UART_Init                   0x103 /**<  */
#define ERROR_HAL_CORDIC_Init                 0x104

#define ERROR_MEMINFO_RAM1_USAGE_OVERFLOW       0x200 /**<  */
#define ERROR_MEMINFO_RAM2_USAGE_OVERFLOW       0x201 
#define ERROR_MEMINFO_CCSRAM_USAGE_OVERFLOW     0x202

/** @} */

/**
 * @brief Global error code variable.
 * This variable holds the current error code for the system.
 */
extern uint32_t global_error_code __attribute((section(".sysDiag")));

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void);

#endif // TRINITY_TRACK6000_ERRORS_H_