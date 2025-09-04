/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <main.h>
#include <string.h>
#include <stdint.h>

#include <TrinityTrack6000_Config.h>

extern void ram_dump();
extern void flash_dump();
extern void initializeSystem();

void send_string(UART_HandleTypeDef*uart,char*s){
    HAL_UART_Transmit(uart,(uint8_t*)s,strlen(s),1000);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){
    initializeSystem();

    //ram_dump();

    GPIOA->MODER &= ~(0b11 << (5 * 2)); // wyczyść bity MODER5
    GPIOA->MODER |=  (0b01 << (5 * 2)); // ustaw jako output
    GPIOA->OTYPER &= ~(1 << 5);
    GPIOA->PUPDR &= ~(0b11 << (5 * 2));

    int i=0;
    while (i++<100){
        send_string(&uart,"Hello World!\r\n");
        GPIOA->ODR ^= (1 << 5);
	      HAL_Delay(100);
    }
    GPIOA->ODR|=(1<<5);
}
