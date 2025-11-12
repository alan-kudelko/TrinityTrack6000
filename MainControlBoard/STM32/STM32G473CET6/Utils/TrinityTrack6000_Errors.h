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

#include <stdint.h>

/**
 * @name Program error codes
 * @{
 */
#define ERROR_MEMINFO_RAM1_USAGE_OVERFLOW       0x01 /**< Error in calculating memory use in RAM1 bank */
#define ERROR_MEMINFO_RAM2_USAGE_OVERFLOW       0x02 /**< Error in calculating memory use in RAM2 bank */
#define ERROR_MEMINFO_CCSRAM_USAGE_OVERFLOW     0x03 /**< Error in calculating memory use in CCSRAM bank */

/** @} */

#define ERROR_DEVICE_STATUS_COUNT 4

/**
 * @name Devices status codes
 * @{
 */
#define ERROR_DEVICE_STATUS_OK      0x00
#define ERROR_DEVICE_STATUS_OFFLINE 0x01
#define ERROR_DEVICE_STATUS_TIMEOUT 0x02
#define ERROR_DEVICE_STATUS_UNKNOWN 0x03
/** @} */

typedef struct{
    uint16_t system_error;
    uint8_t nrf_error;
    uint8_t fram_error;
    uint8_t mcp_error;
    uint8_t adxl_error;
    uint8_t gps_error;
    uint8_t infineon_error;
    uint8_t nxp_error;
    uint8_t renesans_error;
    uint8_t fpga_error;
}SystemErrorStatus_t;

/**
 * @brief  This function is executed in case of critical error occurrence from 
 * which system can no safely recover.
 * Each time critical error occurs this function should be called.
 * @retval None
 */

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void Error_Handler(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TRINITY_TRACK6000_ERRORS_H_