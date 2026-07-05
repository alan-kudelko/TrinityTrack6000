/**
 * @addtogroup buzzer
 * @{
 */

#include <stm32g4xx_hal.h>
#include <buzzer.h>

extern TIM_HandleTypeDef htim1; // Timer handle for TIM1 (used for buzzer output)
extern TIM_HandleTypeDef htim17; // Timer handle for TIM17 (used for buzzer sequencing)

const BuzzerNote_t buzzer_test_sequence[BUZZER_SOUND_SEQUENCE_MAX_LENGTH]={
    {1000,10},
    {1100,10},
    {1200,10},
    {1300,10},
    {1400,10},
    {1500,10},
    {1600,10},
    {1700,10},
    {1800,10},
    {1900,10},
    {2000,10},
    {2100,10},
    {2200,10},
    {2300,10},
    {2400,10}
};

static int16_t buzzer_sequence_index=-1; // Index for the current note in the buzzer sequence
static BuzzerNote_t*buzzer_current_sequence=NULL; // Pointer to the current buzzer sequence being played

void buzzer_init(void){
    // Initialize the buzzer hardware (e.g., configure GPIO pin for TIM1_CH1 output)
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void buzzer_deinit(void){
    // Deinitialize the buzzer hardware (e.g., reset GPIO pin configuration)
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
    HAL_TIM_PWM_Stop(&htim17,TIM_CHANNEL_1); // Stop TIM17 if it was running
}

void buzzer_play(uint32_t sound){
    HAL_TIM_Base_Stop_IT(&htim17); // Stop TIM17 to reset any ongoing buzzer sequence
    // Select the appropriate buzzer sequence based on the requested sound
    switch(sound){
        case BUZZER_SOUND_RADIO_LINK_LOST:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_test_sequence; // Set the current sequence to the test sequence
            break;
        case BUZZER_SOUND_RADIO_LINK_ESTABLISHED:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_test_sequence; // Set the current sequence to the test sequence
            break;
        case BUZZER_SOUND_FAULT_DETECTED:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_test_sequence; // Set the current sequence to the test sequence
            break;
        case BUZZER_SOUND_MOTOR_TEMP_HIGH:
            buzzer_current_sequence=(BuzzerNote_t*)buzzer_test_sequence; // Set the current sequence to the test sequence
            break;
        default:
            buzzer_current_sequence=NULL; // No valid sound selected
            break;
    }

    if(buzzer_current_sequence!=NULL){
        buzzer_sequence_index=0; // Start from the first note in the sequence
        // Set the PWM frequency and duration for the first note
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
    if(buzzer_sequence_index>=BUZZER_SOUND_SEQUENCE_MAX_LENGTH-1){
        // If the end of the sequence is reached, stop the buzzer
        HAL_TIM_Base_Stop_IT(&htim17);
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0); // Stop the buzzer output
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