/**
 * @file EXTI_Callbacks.c
 * @brief Implementation of EXTI callbacks for handling external interrupts.
 * This file contains the implementation of the HAL_GPIO_EXTI_Callback functions, which
 * are called by the HAL library when an external interrupt is triggered on a GPIO pin configured for EXTI.
 * The callback functions are responsible for handling the specific actions that need to be taken when an interrupt occurs on a particular pin.
 * @note The actual actions taken in the callback functions will depend on the specific requirements of the application and the devices connected to the GPIO pins.
 * 
 * @date 2026.13.04
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2026 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <stm32g4xx_hal.h>
#include <TrinityTrack6000_Pinout.h>

extern void radioDataReceived_callback(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin==NRF24L01_IRQ_Pin){
        radioDataReceived_callback();
    }
}