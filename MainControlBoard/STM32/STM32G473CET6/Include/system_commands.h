/**
 * @file system_commands.h
 * @brief Header file for system commands used in the CLI task and system dispatcher.
 * This header defines the data structures and constants related to system commands that can be sent from the CLI task to the system dispatcher for execution.
 * It includes definitions for command types, payloads, and any necessary status or callback information.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef SYSTEM_COMMANDS_H_
    #define SYSTEM_COMMANDS_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdint.h>

typedef enum DEVICE_ID{
    DEVICE_MCP1,
    DEVICE_MCP2,
    DEVICE_NRF24L01,
    DEVICE_ADXL345,
    DEVICE_GPS,
    DEVICE_MCP23017,
    DEVICE_FRAM,
    DEVICE_INFINEON,
    DEVICE_NXP,
    DEVICE_RENESANS,
    DEVICE_FPGA,
}DEVICE_ID;

#define DEVICE_COUNT 11 //!< Total number of supported devices, can be adjusted as needed

typedef enum HARDWARE_ID{
    HARDWARE_MOTOR1,
    HARDWARE_MOTOR2,
    HARDWARE_SENSOR1,
    HARDWARE_SENSOR2,
    // Add more hardware IDs as needed
}HARDWARE_ID;

#define HARDWARE_COUNT 4 //!< Total number of supported hardware components, can be adjusted as needed

typedef enum{
    CLI_CMD_SET_VALUE,
    CLI_CMD_BUS_RAW_DATA,
}CLI_CMD_TYPE;

typedef struct ALIGNED(4) TASK_CLI_COMMAND{
    CLI_CMD_TYPE commandType;
    union{
        struct{
            uint32_t hardwareId;
            uint32_t value;
        }set;
        struct{
            uint8_t*txBuffer;
            uint8_t*rxBuffer;
            uint16_t txLength;
            uint16_t rxLength;
            DEVICE_ID deviceId;
        }rawData;
    }payload;
    uint32_t*commandStatus; // Probably not needed
    void(*callbackFn)(void);
}TASK_CLI_COMMAND;

_Static_assert(sizeof(TASK_CLI_COMMAND)%4==0,"TASK_CLI_COMMAND size must be a multiple of 4 bytes for proper alignment in the queue");

#endif // SYSTEM_COMMANDS_H_