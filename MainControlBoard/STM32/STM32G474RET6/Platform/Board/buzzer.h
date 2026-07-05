/**
 * @defgroup buzzer System Buzzer
 * @brief This module handles the system buzzer functionality via TIM1_CH1 output.
 * Sequencing through the notes is done via TIM17 interrupts. The buzzer can play a sequence of notes defined
 * as an array of BuzzerNote_t structures. Each note has a frequency and duration.
 * 
 * 
 * @warning This module will be refactored in the future to get rid of CPU operations in the interrupt context. 
 * Target solution is DMA-based buzzer output with a timer to trigger the next note. This will allow for more complex sequences and reduce CPU load.
 * 
 * @date 2026.07.05
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

#ifndef BUZZER_H
    #define BUZZER_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stdint.h>

#define BUZZER_SOUND_SEQUENCE_MAX_LENGTH 30 /**< Max length of the buzzer sound sequence */

/**
 * @brief Structure representing a single note for the buzzer
 * Each note has a frequency in Hz and a duration in milliseconds.
 * @{
 */
typedef struct{
    uint16_t frequency_hz;
    uint16_t duration_ms;
}BuzzerNote_t;

enum BuzzerSound_t{
    BUZZER_SOUND_RADIO_LINK_LOST=0,
    BUZZER_SOUND_RADIO_LINK_ESTABLISHED,
    BUZZER_SOUND_FAULT_DETECTED,
    BUZZER_SOUND_MOTOR_TEMP_HIGH,
};

/**@} */

extern const BuzzerNote_t buzzer_test_sequence[];

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Initialize the system buzzer
 * This function initializes the buzzer hardware and starts the PWM output on TIM1_CH1.
 * @param None
 * @return None
 */
void buzzer_init(void);

/**
 * @brief Deinitialize the system buzzer
 * This function stops the PWM output and deinitializes the buzzer hardware.
 * @param None
 * @return None
 */
void buzzer_deinit(void);

/**
 * @brief Play a specific buzzer sound
 * @param sound The buzzer sound to play
 */
void buzzer_play(uint32_t sound);

/**
 * @brief Callback function to update the buzzer sequence
 * This function is called from the TIM17 interrupt handler to update the buzzer sequence.
 * @param None
 * @return None
 */
void buzzer_update_sequence_callback(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // BUZZER_H_

/**@} */