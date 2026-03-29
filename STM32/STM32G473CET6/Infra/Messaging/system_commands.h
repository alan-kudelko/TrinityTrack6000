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

#include <Radio_Types.h>
#include <system_types.h>

typedef enum DEVICE_ID{
    DEVICE_MCP1, // For testing purposes
    DEVICE_MCP2, // For testing purposes
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
    REQUEST_SET_VALUE,
    REQUEST_BUS_RAW_DATA,
    REQUEST_SWITCH_MODE,
    REQUEST_GET_RADIO_STATS,
    REQUEST_GET_RADIO_RUNTIME_STATS,
    REQUEST_GET_RADIO_SETTINGS
}RequestType;

#define SYSTEM_REQUEST_STATUS_ERROR 0
#define SYSTEM_REQUEST_STATUS_OK 1

typedef struct ALIGNED(4) SystemRequest{
    RequestType commandType;
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
        struct{
            SYSTEM_MODE mode;
        }mode;
        RADIO_STATS*radioStats;
        RADIO_RUNTIME_STATS*radioRuntimeStats;
        NRF_SETTINGS*radioSettings;
    }payload;
    uint8_t*commandStatus; // Status of the command (ok or not ok) set by dispatcher
    uint8_t callbackEvent; // Set by request type in the CLI task
    void(*callbackFn)(uint8_t event);
}SystemRequest;

_Static_assert(sizeof(SystemRequest)%4==0,"SystemRequest size must be a multiple of 4 bytes for proper alignment in the queue");

#endif // SYSTEM_COMMANDS_H_