/**
 * @defgroup task_McuAnalog MCU Analog Task
 * @brief Task responsible for monitoring and managing the analog components of the system, such as ADC readings
 * backup domain voltage monitoring, MCU temperature monitoring, Vrefint monitoring, MQ-6 and MQ-7 gas sensors monitoring
 * 
 * @date 2026.02.03
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 * 
 * @{
 */

#ifndef TASK_MCU_ANALOG_H_
    #define TASK_MCU_ANALOG_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <tx_api.h>

#define TASK_MCU_ANALOG_STACK_SIZE 1024 //!< Stack size for MCU Analog task
#define TASK_MCU_ANALOG_PRIORITY 6 //!< Priority for MCU Analog task

extern const char task_McuAnalog_name[]; /**< Name of the MCU Analog task */

extern TX_THREAD task_McuAnalog_handle; /**< Thread handle for Mcu Analog task */
extern ULONG task_McuAnalog_stack[TASK_MCU_ANALOG_STACK_SIZE]; //!< Stack for Mcu Analog task

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void task_McuAnalog_init(void);

void task_McuAnalog(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_MCU_ANALOG_H_

/**@} */