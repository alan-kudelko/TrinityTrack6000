/**
 * @file TrinityTrack6000_Diagnostics.c
 * @brief Implementation of TrinityTrack6000_Diagnostics.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#include <string.h>
#include <stdio.h>

#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Diagnostics.h>
#include <TrinityTrack6000_Config.h>

const char msg_Diagnostics_header1[]                     ="+------------------------[ SYSTEM STATUS ]---------------------------+\r\n";
const char msg_Diagnostics_header2[]                     ="| Component        | Status                                          |\r\n";
const char msg_Diagnostics_header3[]                     ="+------------------+-------------------------------------------------+\r\n";
const char msg_Diagnostics_NRF_status_formatString[]     ="| NRF24L01         | %30s |\r\n";
const char msg_Diagnostics_FRAM_status_formatString[]    ="| FM25L16B-GTR     | %30s |\r\n";
const char msg_Diagnostics_MCP_status_formatString[]     ="| MCP23S17         | %30s |\r\n";
const char msg_Diagnostics_ADXL_status_formatString[]    ="| ADXL345          | %30s |\r\n";
const char msg_Diagnostics_L76K_status_formatString[]    ="| L76K GPS         | %30s |\r\n";
const char msg_Diagnostics_Infineon_status_formatString[]="| XMC4200          | %30s |\r\n";
const char msg_Diagnostics_NXP_status_formatString[]     ="| NXP              | %30s |\r\n";
const char msg_Diagnostics_RENESANS_status_formatString[]="| RENESANS         | %30s |\r\n";
const char msg_Diagnostics_FPGA_status_formatString[]    ="| FPGA             | %30s |\r\n";
                                                        // +------------------+-------------------------------------------------+
const char msg_Diagnostics_footer1[]                     ="| Commands: r(refresh) q(quit)                                       |\r\n";
const char msg_Diagnostics_footer2[]                     ="+--------------------------------------------------------------------+\r\n";

const char*msg_Diagnostics_device_status[ERROR_DEVICE_STATUS_COUNT]={
    "Online",
    "Offline",
    "Response timeout",
    "Unkown"
};

void systemDiagnostics_devices(void){
    systemDiagnostics_NRF();
    systemDiagnostics_FRAM();
    systemDiagnostics_MCP();
    systemDiagnostics_ADXL();
    systemDiagnostics_L76K();  
    systemDiagnostics_Infineon();
    systemDiagnostics_NXP();
    systemDiagnostics_RENESANS();
    systemDiagnostics_FPGA();
}

void systemDiagnostics_NRF(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code

}

void systemDiagnostics_FRAM(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_MCP(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_ADXL(void){
    // Basic read/write test over I2C
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_L76K(void){
    // Basic read/write test over UART
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_Infineon(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_NXP(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_RENESANS(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_FPGA(void){
    // Basic read/write test over SPI
    // with given timeouts
    // after which we set global_error_code
}

void systemDiagnostics_print_devices_Status(void){
    char buffer[DIAGNOSTICS_LINE_BUFFER_SIZE]={0};
    //Check is uart works
    // If so
    // Proceed to write data to serial port
    // On the other hand if uart doesn't work then I won't be able to be called due to lack of
    // serial data to process

// Send System Diagnostics headers 1-3
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_header1,strlen(msg_Diagnostics_header1),DEBUG_UART_TIMEOUT);
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_header2,strlen(msg_Diagnostics_header2),DEBUG_UART_TIMEOUT);
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_header3,strlen(msg_Diagnostics_header3),DEBUG_UART_TIMEOUT);
// Send System Diagnostics NRF24L01 status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_NRF_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.nrf_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics FM25L16B-GTR status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_FRAM_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.fram_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics MCP23S17 status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_MCP_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.mcp_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics ADXL345 status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_ADXL_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.adxl_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics L76K status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_L76K_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.l76k_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics Infineon status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_Infineon_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.infineon_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics NXP status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_NXP_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.nxp_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics RENESANS status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_RENESANS_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.renesans_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics FPGA status
    snprintf(buffer,DIAGNOSTICS_LINE_BUFFER_SIZE,msg_Diagnostics_FPGA_status_formatString,
        msg_Diagnostics_device_status[g_SystemErrors.fpga_error]
    );
    HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
// Send System Diagnostics footers
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_header3,strlen(msg_Diagnostics_header3),DEBUG_UART_TIMEOUT);
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_footer1,strlen(msg_Diagnostics_footer1),DEBUG_UART_TIMEOUT);
    HAL_UART_Transmit(&huart1,(uint8_t*)msg_Diagnostics_footer2,strlen(msg_Diagnostics_footer2),DEBUG_UART_TIMEOUT);
}