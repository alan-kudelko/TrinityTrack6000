/**
 * @file NRF24L01.cpp
 * @brief Implementation of NRF24L01.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

 #include <NRF24L01.h>

 #include <string.h>

 NRF24L01::NRF24L01(){

 }

 NRF24L01::~NRF24L01(){
    
 }

 extern SPI_HandleTypeDef hspi1;

 extern "C" void nrf24l01_test(void){
    // Temporary test function
    uint8_t dataRx[32];
    uint8_t dataTx[32];

    memset(dataTx,0,sizeof(dataTx));
    memset(dataRx,0,sizeof(dataRx));

    dataTx[0]=0xff; // NOP
    dataTx[1]=0xff; // NOP
    // Note op-code is 2 bytes long
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); // Select NRF24L01
    HAL_SPI_TransmitReceive(&hspi1,dataTx,dataRx,2,HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); // Deselect NRF24L01
    // Ostatnim razem przyszła wartość 14 czyli 0x0e jest to wartosc poprawna

    // Teraz odczyt konkretnego rejestru czyli config
    // 000A AAAA w miejsce A wstawiamy 5 bitowy adres rejestru

    // Rejestr config to 0x00
    dataTx[0]=0x00;
    dataTx[1]=0x00;

    memset(dataRx,0,sizeof(dataRx));
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET); // Select NRF24L01
    HAL_SPI_TransmitReceive(&hspi1,dataTx,dataRx,2,HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); // Deselect NRF24L01

    // Teraz testy z moim driverem SPI1_DMA
    // Trzeba zmienic jego implementacje zeby korzystala z flagi half-duplex lub full-duplex
    // Zmiana jest do wykonania w przetwarzaniu konca transmisji zeby nie generowac zegara po raz kolejny
    // W implementacji troche tego bedzie do zmiany
 }