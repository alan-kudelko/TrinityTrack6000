/**
 * @addtogroup mcu_analog
 * @{
 */
#include <stm32g4xx_hal.h>

#include <TrinityTrack6000_Analog.h>

#include <usart3_dma.h>

SystemMeasurement_t system_measurements SECTION(".sysDiag"); // Structure holding the latest system measurements

static uint32_t adc_dma_buffer[ADC_NUMBER_OF_CONVERSIONS]; // DMA buffer for ADC readings

static const uint32_t heater_pwm_fills[2]={950,300}; // PWM fill values for heating and cooling cycles

static const uint32_t heater_cycle_duration_s[2]={5,1}; // Duration of heating and cooling cycles in seconds

extern ADC_HandleTypeDef hadc1; // ADC handle

extern TIM_HandleTypeDef htim6; // Timer handle for triggering heating and cooling cycles
extern TIM_HandleTypeDef htim7; // Timer handle for triggering ADC conversions
extern TIM_HandleTypeDef htim20; // Timer handle for MQ-6 and MQ-7 heating and cooling cycles

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

void mcu_analog_deinit(void){
    // Stop the ADC DMA to halt ongoing conversions
    HAL_ADC_Stop_DMA(&hadc1);
    // Stop TIM7 to cease triggering ADC conversions
    HAL_TIM_Base_Stop_IT(&htim7);
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

void mcu_analog_mq_init(void){
    HAL_TIM_Base_Start_IT(&htim6); // Start TIM6 in interrupt mode for system tick
    HAL_TIM_PWM_Start(&htim20,TIM_CHANNEL_2); // Start PWM on TIM20 channel
    mcu_analog_start_heating_cycle(); // Start the heating cycle for MQ-6 and MQ-7 sensors
}

void mcu_analog_mq_deinit(void){
    HAL_TIM_PWM_Stop(&htim20,TIM_CHANNEL_2); // Stop PWM on TIM20 channel
    HAL_TIM_Base_Stop_IT(&htim6); // Stop TIM6 interrupt for heating and cooling cycles
}

void mcu_analog_start_heating_cycle(void){
    __HAL_TIM_SET_COMPARE(&htim20,TIM_CHANNEL_2,heater_pwm_fills[0]); // Set PWM fill for heating cycle
}

void mcu_analog_start_cooling_cycle(void){
    __HAL_TIM_SET_COMPARE(&htim20,TIM_CHANNEL_2,heater_pwm_fills[1]); // Set PWM fill for cooling cycle
}

void mcu_analog_tim6_callback(void){
    static uint8_t cycle_state=0; // 0: heating, 1: cooling
    // Set the PWM fill value based on the current cycle state
    __HAL_TIM_SET_COMPARE(&htim20,TIM_CHANNEL_2,heater_pwm_fills[cycle_state]);
    // Set the duration for the current cycle
    __HAL_TIM_SET_COMPARE(&htim6,TIM_CHANNEL_1,heater_cycle_duration_s[cycle_state]);
    // Clear the TIM6 CNT register to reset the timer count
    __HAL_TIM_SET_COUNTER(&htim6,0);
    // Toggle cycle state for the next callback
    cycle_state=(cycle_state+1)%2;
}

/**@} */