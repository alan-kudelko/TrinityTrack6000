//Doxy

#ifndef _TRINITY_TRACK6000_CONFIG_H_
	#define _TRINITY_TRACK6000_CONFIG_H_

//#includes
#include "stm32l4xx_hal.h"

// ========================
// UART Configuration
// ========================

// Uart instance
#define UART_INSTANCE 2

// Uart baud rate
#define UART_BAUD_RATE 115200;

// Rest of uart settings are defalt
// wordLength 8B
// parity none
// stop_bits 1
// no flow control
// oversampling 16
// mode tx rx

// Uart Handler for use in HAL
extern UART_HandleTypeDef uart;


	
#endif // _TRINITY_TRACK6000_CONFIG_H_