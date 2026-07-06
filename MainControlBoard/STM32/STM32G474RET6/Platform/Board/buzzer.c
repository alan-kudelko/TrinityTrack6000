/**
 * @addtogroup buzzer
 * @{
 */

#include <stm32g4xx_hal.h>
#include <buzzer.h>

extern TIM_HandleTypeDef htim1; // Timer handle for TIM1 (used for buzzer output)
extern TIM_HandleTypeDef htim17; // Timer handle for TIM17 (used for buzzer sequencing)

const BuzzerNote_t buzzer_radio_link_lost_sequence[BUZZER_SOUND_SEQUENCE_MAX_LENGTH]={
    {1000,100},
    {1200,100},
    {1400,100},
    {1600,100},
    {1800,100},
    {0,100},
    {1000,100},
    {0,100},
    {1000,200}
}; // No need for further experimentation

const BuzzerNote_t buzzer_radio_link_established_sequence[BUZZER_SOUND_SEQUENCE_MAX_LENGTH]={
    {1800,100},
    {1600,100},
    {1400,100},
    {1200,100},
    {1000,100}
}; // No need for further experimentation

const BuzzerNote_t buzzer_fault_detected_sequence[BUZZER_SOUND_SEQUENCE_MAX_LENGTH]={
    {1000,5000}
}; // No need for further experimentation

const BuzzerNote_t buzzer_motor_temp_high_sequence[BUZZER_SOUND_SEQUENCE_MAX_LENGTH]={
    {1000,100},
    {1000,100},
    {2000,100},
    {2000,100},
    {1000,100},
    {1000,100},
    {2000,100},
    {2000,100},
    {1000,100},
    {1000,100},
    {2000,100},
    {2000,100},
    {1000,100},
    {1000,100},
}; // No need for further experimentation

static volatile int16_t buzzer_sequence_index=0; // Index for the current note in the buzzer sequence
static BuzzerNote_t*buzzer_current_sequence=NULL; // Pointer to the current buzzer sequence being played

void buzzer_init(void){
    // Initialize the buzzer hardware (e.g., configure GPIO pin for TIM1_CH1 output)
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void buzzer_deinit(void){
    // Deinitialize the buzzer hardware (e.g., reset GPIO pin configuration)
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
    HAL_TIM_Base_Stop_IT(&htim17); // Stop TIM17 if it was running
}

void buzzer_play(uint32_t sound){
    HAL_TIM_Base_Stop_IT(&htim17); // Stop TIM17 to reset any ongoing buzzer sequence
    __HAL_TIM_SET_COUNTER(&htim17,0);
    // Select the appropriate buzzer sequence based on the requested sound
    switch(sound){
        case BUZZER_SOUND_RADIO_LINK_LOST:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_radio_link_lost_sequence; // Set the current sequence to the radio link lost sequence
            break;
        case BUZZER_SOUND_RADIO_LINK_ESTABLISHED:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_radio_link_established_sequence; // Set the current sequence to the radio link established sequence
            break;
        case BUZZER_SOUND_FAULT_DETECTED:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_fault_detected_sequence; // Set the current sequence to the fault detected sequence
            break;
        case BUZZER_SOUND_MOTOR_TEMP_HIGH:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_motor_temp_high_sequence; // Set the current sequence to the motor temp high sequence
            break;
        default:
            buzzer_current_sequence=NULL; // No valid sound selected
    }

    if(buzzer_current_sequence!=NULL){
        buzzer_sequence_index=0; // Start from the first note in the sequence
        __HAL_TIM_SET_AUTORELOAD(&htim1,buzzer_current_sequence[buzzer_sequence_index].frequency_hz-1);
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,buzzer_current_sequence[buzzer_sequence_index].frequency_hz/2); // Set the PWM duty cycle to 50% for audible output

        __HAL_TIM_SET_COUNTER(&htim17,0); // Reset the counter for TIM17
        __HAL_TIM_SET_AUTORELOAD(&htim17,buzzer_current_sequence[buzzer_sequence_index].duration_ms*10-1);
        HAL_TIM_Base_Start_IT(&htim17); // Start TIM17 in interrupt mode to handle buzzer sequencing
    }
}

void buzzer_update_sequence_callback(void){
    // Iterate to the next note in the buzzer sequence
    buzzer_sequence_index++;
    // Check if the end of the sequence is reached
    if(buzzer_sequence_index>=BUZZER_SOUND_SEQUENCE_MAX_LENGTH){
        // If the end of the sequence is reached, stop the buzzer
        HAL_TIM_Base_Stop_IT(&htim17);
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0); // Stop the buzzer output
        __HAL_TIM_SET_AUTORELOAD(&htim1,1000); // Reset the PWM frequency
        buzzer_sequence_index=0; // Reset the sequence index
        buzzer_current_sequence=NULL; // Clear the current sequence pointer
    }
    else{
        // Set the PWM frequency and duration for the next note in the sequence
        __HAL_TIM_SET_AUTORELOAD(&htim1,buzzer_current_sequence[buzzer_sequence_index].frequency_hz-1);
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,buzzer_current_sequence[buzzer_sequence_index].frequency_hz/2); // Set the PWM duty cycle to 50% for audible output
        __HAL_TIM_SET_COUNTER(&htim17,0); // Reset the counter for TIM17
        __HAL_TIM_SET_AUTORELOAD(&htim17,buzzer_current_sequence[buzzer_sequence_index].duration_ms*10-1);
    }
}

/**@} */