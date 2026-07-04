/**
 * @addtogroup mcu_analog
 * @{
 */
#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Analog.h>

#include <usart3_dma.h>

SystemMeasurement_t system_measurements SECTION(".sysDiag"); // Structure holding the latest system measurements

static uint32_t adc_dma_buffer[ADC_NUMBER_OF_CONVERSIONS]; // DMA buffer for ADC readings

extern ADC_HandleTypeDef hadc1; // ADC handle
extern TIM_HandleTypeDef htim7; // Timer handle for triggering ADC conversions
extern DMA_HandleTypeDef hdma_adc1; // DMA handle for ADC

void mcu_analog_init(void){
    // Disable ADC DMA half-transfer interrupt to avoid unnecessary callbacks
    __HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_HT);
    // Clear any pending half-transfer flags to ensure a clean start
	__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_HT3);
    // Start TIM7 in interrupt mode to trigger ADC conversions via TRGO
    HAL_TIM_Base_Start_IT(&htim7);
    // Start the ADC in DMA mode
    HAL_ADC_Start_DMA(&hadc1,adc_dma_buffer,ADC_NUMBER_OF_CONVERSIONS);
}

void mcu_analog_update_measurements(void){
    // Update the system measurements from the ADC DMA buffer
    system_measurements.vrefint=__HAL_ADC_CALC_VREFANALOG_VOLTAGE(adc_dma_buffer[0],ADC_RESOLUTION_12B);
    system_measurements.mcu_temp=__HAL_ADC_CALC_TEMPERATURE(system_measurements.vrefint,adc_dma_buffer[1],ADC_RESOLUTION_12B);
    system_measurements.vrefint=system_measurements.vrefint*VDDA_CALIBRATION_FACTOR; // Apply calibration factor to Vrefint
    system_measurements.mq6_sensor=(float)adc_dma_buffer[2]*system_measurements.vrefint/4095.0f;
    system_measurements.mq7_sensor=(float)adc_dma_buffer[3]*system_measurements.vrefint/4095.0f;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*hadc){
    if(hadc->Instance==hadc1.Instance){
        mcu_analog_update_measurements();
    }
}
/**@} */