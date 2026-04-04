/**
 * 
 */

#ifndef DEVICE_DEF_H_
    #define DEVICE_DEF_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <stm32g4xx_hal.h>

typedef struct DEVICE_IO{
    GPIO_TypeDef*gpio_port;
    uint16_t gpio_pin;
}DEVICE_IO;

#endif // DEVICE_DEF_H_