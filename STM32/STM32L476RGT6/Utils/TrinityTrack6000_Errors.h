//Doxy
#ifndef _TRINITY_TRACK6000_ERRORS_H_
    #define _TRINITY_TRACK6000_ERRORS_H_

#define ERROR_HAL_PWREx_ControlVoltageScaling 0x100
#define ERROR_HAL_RCC_OscConfig               0x101
#define ERROR_HAL_RCC_ClockConfig             0x102
#define ERROR_HAL_UART_Init                   0x103

extern uint32_t global_error_code __attribute((section(".sysDiag")));

#endif // _TRINITY_TRACK6000_ERRORS_H_