/**
 * Doxy
 */

#include <stm32g4xx_hal.h>
#include <TrinityTrack6000_Pinout.h>

extern void radioDataReceived_callback(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin==NRF24L01_IRQ_Pin){
        radioDataReceived_callback();
    }
}