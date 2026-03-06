/**
 * @file NRF24L01.h
 * @brief Dedicated class for NRF24L01 module
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef NRF24L01_H_
    #define NRF24L01_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__ 

#include <Device_Def.h>

class NRF24L01{
    DEVICE_IO cePin;
    DEVICE_IO csnPin;
    DEVICE_IO irqPin;


public:
    NRF24L01();
    ~NRF24L01();
};

// Test functions

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void nrf24l01_test(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

// Dobra to co jest istotne:
// Port i pin CE CSN IRQ?
// Jakiś callback gdy będzie IRQ
// Na pewno wewnętrzne lub zewnętrzne bufory do obsługi DMA i interfejs do udostępniania wskaźników na te bufory
// Struktura przechowująca piny i porty. Też tak myślę, że ta struktura powinna być uniwersalna dla różnych urządzeń
// Podobnie będzie np. z klasą FRAM'u oraz potencjalnie innymi urzązeniami nie tylko SPI ale też UART czy I2C
// No oczywiscie rejestry i opis bitow
// Funkcje do konfiguracji i obslugi z abstrakcja rejestrow i bitow
// Bedzie to wykorzystywac na pewno moj interfejs do SPI1 DMA
// wskaźnik na funkcję timeout kolejkowania dla mojego SPI
// sygnatura zgodna z tx_thread_sleep
// 


#endif // NRF24L01_H_