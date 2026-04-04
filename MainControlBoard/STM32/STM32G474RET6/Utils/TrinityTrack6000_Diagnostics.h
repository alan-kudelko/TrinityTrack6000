/**
 * @file    TrinityTrack6000_Diagnostics.h
 * @brief   Diagnostics header for TrinityTrack6000 project.
 * This header provides prototypes for system diagnostics functions,
 * including memory diagnostics and overall system health checks
 * after initialization and before entering the main application loop
 * in main.c.
 * 
 * @note All functions set global_error_code in case of failure,
 * which can be used by Error_Handler() for debugging or startup without
 * faulty functionalities.
 * 
 * @note All functions can be called within systemDiagnostics() or separately
 * if needed.
 *  
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TRINITY_TRACK6000_DIAGNOSTICS_H_
    #define TRINITY_TRACK6000_DIAGNOSTICS_H_

#include <TrinityTrack6000_Errors.h>

#define DIAGNOSTICS_LINE_BUFFER_SIZE 90
#define DIAGNOSTICS_BAR_BUFFER_SIZE 11

extern const char msg_Diagnostics_header1[];
extern const char msg_Diagnostics_header2[];
extern const char msg_Diagnostics_header3[];
extern const char msg_Diagnostics_footer1[];
extern const char msg_Diagnostics_footer2[];

extern const char msg_Diagnostics_NRF_status_formatString[];
extern const char msg_Diagnostics_FRAM_status_formatString[];
extern const char msg_Diagnostics_MCP_status_formatString[];
extern const char msg_Diagnostics_ADXL_status_formatString[];
extern const char msg_Diagnostics_GPS_status_formatString[];
extern const char msg_Diagnostics_Infineon_status_formatString[];
extern const char msg_Diagnostics_NXP_status_formatString[];
extern const char msg_Diagnostics_RENESANS_status_formatString[];
extern const char msg_Diagnostics_FPGA_status_formatString[];

extern const char*msg_Diagnostics_status[ERROR_DEVICE_STATUS_COUNT];

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**
 * @brief Perform system diagnostics.
 * This function runs a series of diagnostics 
 * to verify the integrity and functionality of
 * the system components after initialization.
 * @note This function should be called
 * after initializeSystem() in main.c
 * and before entering the main application loop.
 * @return void
 * @retval None
 */
void systemDiagnostics_devices(void);


/**
 * @brief Perform NRF24L01+ specific diagnostics.
 * This function checks the functionality of the NRF24L01+ module,
 * including SPI communication and basic operation tests.
 * @return void
 * @retval None
 */
void systemDiagnostics_NRF(void);

/**
 * @brief Perform FM25L16B-GTR FRAM specific diagnostics.
 * This function checks the functionality of the FRAM module,
 * including SPI communication and basic read/write tests.
 * @return void
 * @retval None
 */
void systemDiagnostics_FRAM(void);

/**
 * @brief Perform MCP23S17 specific diagnostics.
 * This function checks the functionality of the MCP23S17 module,
 * including SPI communication and basic operation tests.
 * @return void
 * @retval None
 */

void systemDiagnostics_MCP(void);

/**
 * @brief Perform ADXL345 specific diagnostics.
 * This function checks the functionality of the ADXL345 module,
 * including SPI communication and basic operation tests.
 * @return void
 * @retval None
 */

void systemDiagnostics_ADXL(void);

/**
 * @brief Perform L76K GPS module specific diagnostics.
 * This function checks the functionality of the L76K GPS module,
 * including UART communication and basic operation tests.
 * @return void
 * @retval None
 */
void systemDiagnostics_GPS(void);

/**
 * @brief Perform Infineon XMC4200 specific diagnostics. 
 * This function checks the SPI communication with the Infineon MCU.
 * @return void
 * @retval None
 */
void systemDiagnostics_Infineon(void);

/**
 * @brief Perform NXP TBD specific diagnostics.
 * This function checks the SPI communication with the NXP MCU.
 * @return void
 * @retval None
 */
void systemDiagnostics_NXP(void);

/**
 * @brief Perform RENESAS TBD specific diagnostics.
 * This function checks the SPI communication with the RENESAS MCU.
 * @return void
 * @retval None
 */
void systemDiagnostics_RENESANS(void);

/**
 * @brief Perform FPGA specific diagnostics.
 * This function checks the SPI communication with the FPGA.
 * @return void
 * @retval None
 */
void systemDiagnostics_FPGA(void);

/**
 * @brief Prints system's status on serial port
 * 
 * @return void
 * @retval None
 */

void systemDiagnostics_print_devices_Status(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TRINITY_TRACK6000_DIAGNOSTICS_H_