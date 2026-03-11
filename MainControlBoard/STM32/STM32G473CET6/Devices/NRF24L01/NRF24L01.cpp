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

#include <SPI1_Dma.h>

#include <stm32g4xx_hal.h> // Delete later
void NRF24L01::initialize_hspi_data(){
    _transaction_data.callbackFn=nullptr;
    _transaction_data.flags=HSPI_FLAG_FULL_DUPLEX; // Only option since it's full duplex communication
    _transaction_data.gpio_pin=_csnPin.gpio_pin; // Chip select pin and port for SPI transport layer
    _transaction_data.gpio_port=_csnPin.gpio_port;
    _transaction_data.rxBuffer=nullptr;
    _transaction_data.rxLength=0;
    _transaction_data.txBuffer=nullptr;
    _transaction_data.txLength=0;
}

NRF24L01::NRF24L01(DEVICE_IO ce,DEVICE_IO csn, DEVICE_IO irq){
    _cePin=ce;
    _csnPin=csn;
    _irqPin=irq;

    this->initialize_hspi_data();
}

NRF24L01::NRF24L01(GPIO_TypeDef*cePort,uint16_t cePin,GPIO_TypeDef*csnPort,uint16_t csnPin,GPIO_TypeDef*irqPort,uint16_t irqPin){
    _cePin.gpio_port=cePort;
    _cePin.gpio_pin=cePin;
    _csnPin.gpio_port=csnPort;
    _csnPin.gpio_pin=csnPin;
    _irqPin.gpio_port=irqPort;
    _irqPin.gpio_pin=irqPin;

    this->initialize_hspi_data();
}

NRF24L01::~NRF24L01(){
    // Nothing to do
}

void NRF24L01::attach_tx_buffer(uint8_t*txBuffer,uint8_t txBufferLength){
    _transaction_data.txBuffer=txBuffer;
    _transaction_data.txLength=txBufferLength;
}

void NRF24L01::attach_rx_buffer(uint8_t*rxBuffer,uint8_t rxBufferLength){
    _transaction_data.rxBuffer=rxBuffer;
    _transaction_data.rxLength=rxBufferLength;
}

void NRF24L01::attach_callback_function(void(*callbackFn)(void)){
    // Probably should be inline since it's only 1 line
    _transaction_data.callbackFn=callbackFn;
}

bool NRF24L01::write_reg_config(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_CONFIG;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_config(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_CONFIG;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_en_aa(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_EN_AA;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_en_aa(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_EN_AA;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_en_rxAddr(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_EN_RXADDR;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_en_rxAddr(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_EN_RXADDR;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_setup_aw(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_SETUP_AW;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_setup_aw(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_SETUP_AW;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_setup_retr(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_SETUP_RETR;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_setup_retr(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_SETUP_RETR;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rf_ch(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RF_CH;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rf_ch(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RF_CH;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rf_setup(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RF_SETUP;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rf_setup(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RF_SETUP;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_status(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_STATUS;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_status(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_STATUS;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_observe_tx(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_OBSERVE_TX;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_cd(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_CD;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p0(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P0;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p0(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P0;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p1(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P1;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p1(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P1;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p2(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P2;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p2(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P2;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p3(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P3;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p3(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P3;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p4(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P4;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p4(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P4;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_addr_p5(const uint8_t*addr,uint8_t length){
    // SETUP_AW is important here since it defines address length
    // Add error checking in the future //Fix
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_ADDR_P5;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_addr_p5(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_ADDR_P5;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_tx_addr(const uint8_t*addr,uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_TX_ADDR;
    memcpy(_transaction_data.txBuffer+1,addr,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_tx_addr(uint8_t length){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_TX_ADDR;
    memset(_transaction_data.txBuffer+1,NRF_CMD_NOP,length);
    _transaction_data.txLength=length+1;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p0(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P0;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p0(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P0;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p1(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P1;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p1(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P1;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p2(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P2;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p2(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P2;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p3(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P3;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p3(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P3;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p4(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P4;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p4(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P4;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_rx_pw_p5(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_RX_PW_P5;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_rx_pw_p5(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_RX_PW_P5;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;
    
    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_fifo_status(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_FIFO_STATUS;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_fifo_status(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_FIFO_STATUS;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_dynpd(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_DYNPD;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_dynpd(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_DYNPD;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::write_reg_feature(uint8_t flags){
    _transaction_data.txBuffer[0]=NRF_CMD_W_REGISTER|NRF_REG_FEATURE;
    _transaction_data.txBuffer[1]=flags;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_reg_feature(){
    _transaction_data.txBuffer[0]=NRF_CMD_R_REGISTER|NRF_REG_FEATURE;
    _transaction_data.txBuffer[1]=NRF_CMD_NOP;
    _transaction_data.txLength=2;

    return spi1_dma_enq_data(&_transaction_data);
}

bool NRF24L01::read_rx_payload(uint8_t length){
    return true;
}

bool NRF24L01::write_tx_payload(uint8_t length){
    return true;
}


// Test functions

extern SPI_HandleTypeDef hspi1;

extern "C" void nrf24l01_init(void){
    // Dobra na razie pierwsza proba bedzie taka, ze to stm32 bedzie nadawac
    // Na arduino tymczasem zrobie polling albo event driven wypisywanie danych na uart w postaci surowej paczki
    // Detale sa nieistotne, istotne jest potwierdzenie konfiguracji i poprawnosci odbieranych przez Arduino danych
    // W STM'ie zrobimy sobie petle nadawcza co np. 1 sekunde i na arduino zobaczymy czy zadziała
    // Jeżeli zadziała to ustawimy do wysłania np. 1000 pakietów i określimy procent utraconych pakietów w zależności
    // Od odległości między nadajnikiem a odbiornikiem.
    // Pozwoli to na sprawdzenie czy jest dobre spasowanie anten na własnym PCB.
    // Ustawienia radia: (idą po kolei rejestrami)
    // STM/AVR CONFIG        NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP
    //     AVR EN_AA         NRF_BIT_ENAA_P0
    // STM/AVR SETUP_AW      NRF_BIT_AW1|NRF_BIT_AW0
    // STM/AVR SETUP_RETR    NRF_BIT_ARD5|NRF_BIT_ARC1|NRF_BIT_ARC0
    // STM/AVR RF_CH         NRF_BIT_RF_CH5
    // STM/AVR RF_SETUP      NRF_BIT_RF_DR|NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1
    //     AVR RX_ADDR_P0    0xFE FE FE FE FE
    // STM/AVR TX_ADDR       0xFE FE FE FE FE
    //     AVR RX_PW_P0      1<<5 (32 bajty)
    //
    // A tak teraz do zrobienia zostalo napisanie programu na Arduino Mega i sprobowac to skomunikowac
    // Jakkolwiek skomunikowac, bez przerwan bez czegokolwiek, jedynie sprawdzić % utraconych pakietów
    // w zależności od odległości, parametrem będzie prędkość transmisji 1 Mbps lub 2 Mbps
    // A na Arduino dodać LCD'ka żeby wyświetlić ten procent na koniec

    uint8_t operationTx[33]={0};
    uint8_t operationRx[33]={0};
    const uint8_t address[5]={0xFE,0xFE,0xFE,0xFE,0xFE};

    typedef struct NRF_SETTINGS{
        uint8_t config;
        uint8_t setup_aw;
        uint8_t setup_retr;
        uint8_t rf_ch;
        uint8_t rf_setup;
        uint8_t tx_addr[5];
    }NRF_SETTINGS;

    NRF24L01 nrf24l01(nullptr,0,GPIOB,GPIO_PIN_0,nullptr,0);

    nrf24l01.attach_rx_buffer(operationRx,33);
    nrf24l01.attach_tx_buffer(operationTx,33);

    NRF_SETTINGS nrf_wrote_settings{
        NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP,
        NRF_BIT_AW1|NRF_BIT_AW0,
        NRF_BIT_ARD5|NRF_BIT_ARC1|NRF_BIT_ARC0,
        NRF_BIT_RF_CH5,
        NRF_BIT_RF_DR|NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1,
        {address[0],address[1],address[2],address[3],address[4]}
    };

    NRF_SETTINGS nrf_read_settings{0};
    UNUSED(nrf_read_settings);
    UNUSED(nrf_wrote_settings);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
    HAL_Delay(100);
// Note that there should be retry logic when adding SPI data to SPI Queue, but for now omitted
    //nrf24l01.write_reg_config(NRF_BIT_PWR_UP);
        HAL_Delay(100);
    nrf24l01.write_reg_config(NRF_BIT_PWR_UP);
    HAL_Delay(100);
    nrf24l01.write_reg_config(NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP);
    HAL_Delay(100);
    nrf24l01.write_reg_setup_aw(NRF_BIT_AW1|NRF_BIT_AW0);
        HAL_Delay(100);
    nrf24l01.write_reg_setup_retr(NRF_BIT_ARD5|NRF_BIT_ARC1|NRF_BIT_ARC0);
        HAL_Delay(100);
    nrf24l01.write_reg_rf_ch(NRF_BIT_RF_CH5);
        HAL_Delay(100);
    nrf24l01.write_reg_rf_setup(NRF_BIT_RF_DR|NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1);
        HAL_Delay(100);
    nrf24l01.write_reg_tx_addr(address,5);
        HAL_Delay(100);
    nrf24l01.write_reg_rx_addr_p0(address,5);
        HAL_Delay(100);
// Verify settings
    nrf24l01.read_reg_config();
    HAL_Delay(100);
    nrf_read_settings.config=operationRx[1];
    nrf24l01.read_reg_setup_aw();
        HAL_Delay(100);
    nrf_read_settings.setup_aw=operationRx[1];
    nrf24l01.read_reg_setup_retr();
        HAL_Delay(100);
    nrf_read_settings.setup_retr=operationRx[1];
    nrf24l01.read_reg_rf_ch();
        HAL_Delay(100);
    nrf_read_settings.rf_ch=operationRx[1];
    nrf24l01.read_reg_rf_setup();
        HAL_Delay(100);
    nrf_read_settings.rf_setup=operationRx[1];
    nrf24l01.read_reg_tx_addr(5);
        HAL_Delay(100);
    memcpy(nrf_read_settings.tx_addr,operationRx+1,5);
// Flush TX
    operationTx[0]=NRF_CMD_FLUSH_TX;
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi1,operationTx,operationRx,1,HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);    

    operationTx[0]=NRF_CMD_W_TX_PAYLOAD;
    memset(operationTx+1,0xAA,32);
    for(int i=0;i<1000;i++){
        operationTx[0]=NRF_CMD_W_TX_PAYLOAD;
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi1,operationTx,operationRx,33,HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
        //HAL_Delay(20);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
        //HAL_Delay(20);
        //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);        
        HAL_Delay(60);

        operationTx[0]=NRF_CMD_W_REGISTER|NRF_REG_STATUS;
        operationTx[1]=NRF_BIT_TX_DS|NRF_BIT_MAX_RT;
        
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi1,operationTx,operationRx,2,HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
        operationTx[0]=NRF_CMD_W_REGISTER|NRF_REG_STATUS;
        operationTx[1]=0x30;

        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi1,operationTx,operationRx,2,HAL_MAX_DELAY);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
    }
}

extern "C" void callback(void){
    
}

extern "C" void nrf24l01_test(void){
    // Temporary test function
    uint8_t dataRx[32];
    uint8_t dataTx[32];

    memset(dataTx,0,sizeof(dataTx));
    memset(dataRx,0,sizeof(dataRx));

    dataTx[0]=0xff; // NOP
    dataTx[1]=0xff; // NOP
    
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
    memset(dataRx,0,sizeof(dataRx));
    hspi_data test_data;
    test_data.callbackFn=callback;
    test_data.flags=HSPI_FLAG_FULL_DUPLEX;
    test_data.gpio_port=GPIOB;
    test_data.gpio_pin=GPIO_PIN_0;
    test_data.txBuffer=dataTx;
    test_data.rxBuffer=dataRx;
    test_data.txLength=2;
    test_data.rxLength=2;

    spi1_dma_enq_data(&test_data);

    HAL_Delay(10);
// Testy API klasy NRF24L01 - warto tutaj dodać, że bufory muszą zostać zapewnione przez funkcję wywołującą lub przez proces wywołujący
// Potrzebny jest roboczy bufor rx i tx o długości 33 bajtów
// Dodatkowo bufor nadawczy i odbiorczy również o długości 33 bajtów

// Registers for read and write operations (modified inside the NRF24L01 object)
    uint8_t operationTx[33]={0};
    uint8_t operationRx[33]={0};

    NRF24L01 nrf24l01(nullptr,0,GPIOB,GPIO_PIN_0,nullptr,0);

    nrf24l01.attach_rx_buffer(operationRx,33);
    nrf24l01.attach_tx_buffer(operationTx,33);

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);

    nrf24l01.read_reg_status();
    HAL_Delay(100);
    nrf24l01.write_reg_status(NRF_BIT_MASK_RX_DR|NRF_BIT_MASK_TX_DS);
    HAL_Delay(100);
    nrf24l01.read_reg_status();

    HAL_Delay(100);

}