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

 // After considering the requirements this task is obsolete and can be removed. System measurements can be handled
 // by timer triggered ADC conversions and DMA transfers
 // This way we can avoid the overhead of a separate task and reduce the overall complexity of the system.

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

typedef struct{
    float vrefint; /**< Vrefint voltage in volts */
    float vdda; /**< VDDA voltage in volts */
    float mcu_temp; /**< MCU temperature in degrees Celsius */
    float mq6_sensor; /**< MQ-6 gas sensor reading in ppm */
    float mq7_sensor; /**< MQ-7 gas sensor reading in ppm */
}SystemMeasurement_t;

extern SystemMeasurement_t system_measurements; /**< Structure holding the latest system measurements */

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void task_McuAnalog_init(void);

void task_McuAnalog_create(void);

void task_McuAnalog(ULONG arg);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_MCU_ANALOG_H_

/**@} */