/**
 * @defgroup mcu_analog MCU Analog Measurements
 * @brief This module handles the analog measurements of the MCU, including Vrefint, MCU temperature, and gas sensor readings.
 * Additionally it handles MQ-6 and MQ-7 heating and cooling cycles via TIM20
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

#ifndef MCU_ANALOG_H
    #define MCU_ANALOG_H_

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
 * @brief Initialize the MCU Analog module
 * This function initializes the ADC and starts the DMA for analog measurements.
 * @note This function is called only once during initializion. After that the measurements are updated via TRGO from TIM7
 * @param None
 * @return None
 */
void mcu_analog_init(void);

/**
 * @brief Deinitialize the MCU Analog module
 * This function stops the ADC and DMA for analog measurements.
 * @param None
 * @return None
 */
void mcu_analog_deinit(void);

/**
 * @brief Update the system measurements
 * This function reads the latest ADC values and updates the system_measurements structure
 * @param None
 * @return None
 */
void mcu_analog_update_measurements(void);

/**
 * @brief ADC conversion complete callback
 * This function is called when the ADC conversion is complete.
 * @param hadc: ADC handle
 * @return None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*hadc);

/**
 * @brief Initialize TIM20 for MQ-6 and MQ-7 heating and cooling cycles
 * This function initializes TIM20 to generate PWM signal for controlling the heating and cooling cycles of the MQ-6 and MQ-7 sensors.
 * @param None
 * @return None
 */
void mcu_analog_mq_init(void);

/**
 * @brief Deinitialize TIM20 for MQ-6 and MQ-7 heating and cooling cycles
 * This function deinitializes TIM20 and stops the PWM signal for controlling the heating and cooling cycles of the MQ-6 and MQ-7 sensors.
 * @param None
 * @return None
 */
void mcu_analog_mq_deinit(void);

/**
 * @brief TIM6 callback for triggering heating and cooling cycles
 * This function is called periodically by TIM6 interrupt to manage the heating and cooling cycles of the MQ-6 and MQ-7 sensors.
 * @param None
 * @return None
 */
void mcu_analog_tim6_callback(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // MCU_ANALOG_H_

/**@} */