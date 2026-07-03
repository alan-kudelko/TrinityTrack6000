/**
 * @addtogroup task_McuAnalog
 * @{
 */
#include <stm32g4xx_hal.h>

#include <task_McuAnalog.h>

#include <usart3_dma.h>

SystemMeasurement_t system_measurements SECTION(".sysDiag"); // Structure holding the latest system measurements

static uint32_t adc_dma_buffer[ADC_NUMBER_OF_CONVERSIONS]; // DMA buffer for ADC readings

extern ADC_HandleTypeDef hadc1; // ADC handle

void mcu_analog_timer_callback(void){
    HAL_ADC_Start_DMA(&hadc1,adc_dma_buffer,ADC_NUMBER_OF_CONVERSIONS);
}

void mcu_analog_update_measurements(void){
    // Update the system measurements from the ADC DMA buffer
    system_measurements.vrefint=__HAL_ADC_CALC_VREFANALOG_VOLTAGE(adc_dma_buffer[0],ADC_RESOLUTION_12B)*VDDA_CALIBRATION_FACTOR;
    system_measurements.mcu_temp=__HAL_ADC_CALC_TEMPERATURE(system_measurements.vrefint,adc_dma_buffer[1],ADC_RESOLUTION_12B);
    system_measurements.mq6_sensor=(float)adc_dma_buffer[2]*system_measurements.vrefint/4095.0f;
    system_measurements.mq7_sensor=(float)adc_dma_buffer[3]*system_measurements.vrefint/4095.0f;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*hadc){
    if(hadc->Instance==hadc1.Instance){
        mcu_analog_update_measurements();
    }
}
/**@} */