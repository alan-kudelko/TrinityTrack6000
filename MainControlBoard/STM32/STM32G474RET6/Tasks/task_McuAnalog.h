/**
 * @defgroup mcu_analog MCU Analog Measurements
 * @brief This module handles the analog measurements of the MCU, including Vrefint, MCU temperature, and gas sensor readings.
 * 
 * @date 2026.07.02
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2026 Alan Kudełko.  
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

#define ADC_NUMBER_OF_CONVERSIONS 4 /**< Number of ADC conversions to be performed */
#define VDDA_CALIBRATION_FACTOR 0.964f /**< Calibration factor for VDDA (all measurements are around 4% higher than actual) */

typedef struct{
    float vrefint; /**< Vrefint voltage in mV */
    float mcu_temp; /**< MCU temperature in degrees Celsius */
    float mq6_sensor; /**< MQ-6 gas sensor reading in mV */
    float mq7_sensor; /**< MQ-7 gas sensor reading in mV */
}SystemMeasurement_t;

extern SystemMeasurement_t system_measurements; /**< Structure holding the latest system measurements */

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Callback function for the MCU Analog timer
 * This function is called from the timer interrupt handler
 * It should trigger ADC conversions and read the results
 * @param None
 * @return None
 */
void mcu_analog_timer_callback(void);

/**
 * @brief Update the system measurements
 * This function reads the latest ADC values and updates the system_measurements structure
 * @param None
 * @return None
 */
void mcu_analog_update_measurements(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*hadc);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_MCU_ANALOG_H_

/**@} */