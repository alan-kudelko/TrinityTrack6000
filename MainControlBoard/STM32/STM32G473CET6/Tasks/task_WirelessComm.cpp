/**
 * @file task_WirelessComm.cpp
 * @brief 
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <stm32g4xx_hal.h>

#include <task_WirelessComm.h>
#include <TrinityTrack6000_Pinout.h>

const char task_wireless_comm_name[]="Wireless Task";

const NRF24L01_REGS nrf24l01_default_regs{
    NRF_BIT_TX_DS|NRF_BIT_MAX_RT|NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP|NRF_BIT_PRIM_RX,
    NRF_BIT_ENAA_P0,
    NRF_BIT_ERX_P0,
    NRF_BIT_AW1|NRF_BIT_AW0,
    0,
    NRF_BIT_RF_CH6,
    NRF_BIT_RF_DR|NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1|NRF_BIT_RF_LNA_HCURR,
    NRF_BIT_RX_DR|NRF_BIT_TX_DS|NRF_BIT_MAX_RT,
    0,
    0,
    RADIO_DEFAULT_ADDRESS,
    0,
    0,
    0,
    0,
    0,
    RADIO_DEFAULT_ADDRESS,
    RADIO_DEFAULT_PAYLOAD_SIZE_32,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    NRF_BIT_EN_ACK_PAY
};

TX_THREAD task_wireless_comm_handle SECTION(".task_handles");
ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE] SECTION(".task_stacks_ccsram");

uint32_t test_data[10]{0};
uint16_t test_data_length=10;

static volatile uint8_t operationDone=0;

static NRF24L01 nrf24l01(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,NRF24L01_CS_GPIO_Port,NRF24L01_CS_Pin,NRF24L01_IRQ_GPIO_Port,NRF24L01_IRQ_Pin);

static void operationDone_callback(void){
    operationDone++;
}

extern "C" void radioDataReceived_callback(void){
    // For now just a placeholder
}

extern "C" void nrf24l01_init(void){
    uint8_t txBuffer[OPERATION_BUFFER_SIZE];
    uint8_t rxBuffer[OPERATION_BUFFER_SIZE];

    nrf24l01.attach_rx_buffer(rxBuffer,OPERATION_BUFFER_SIZE);
    nrf24l01.attach_tx_buffer(txBuffer,OPERATION_BUFFER_SIZE);
    nrf24l01.attach_callback_function(operationDone_callback);

    while(nrf24l01.write_reg_config(nrf24l01_default_regs.config)){
        asm("nop");
    }


}

extern "C" void task_wireless_comm_init(void){
    // Placeholder for future initialization
}

extern "C" void task_wireless_comm(ULONG arg){
    UNUSED(arg);

// For now just for architecture testing
// In the future body of this task will be dedicated to handle wireless rx/tx communication

    while(true){

        tx_thread_sleep(1000);
    }
}
// No to tak, to jest specjalista od sprzetu i requesty pochodzace z taska CLI powinny po przejsciu
// Przez dispatchera trafic tutaj do zakolejkowania i asynchronicznego wyslania
// Ma to zalete, ze dowolny zewnetrzny rozkaz do urzadzenia moze zostac w ten sposob zakolejkowany
// Co wiecej maszyna stanow urzadzenia nie ulegnie zadnej komplikacji
// Po prostu czekamy 0 tickow na to czy dane sa w kolejce, jak nie to wykonujemy normalnie
// Kolejna zaleta jest to ze ten task to jedyny wlasciciel sprzetu oraz unikamy zaleznosci
// Co wiecej sterowanie pochodzace z tego taska bedzie ignorowane przez dispatcher w trybie debug
// A sama inicjalizacja oraz definicje domyslnych wartosci rejestrow beda mogly zostac zdefiniowane
// w tym tasku. Architektura bedzie przejrzysta i niezbyt skomplikowane
// Co wiecej jest to popularny wzorzec w RTOS'ach
// A i pomyslec nad struktura kolejki z danymi do tego taska

// Z kolei sam task musi sledzic rejestry, wykonowac skakanie po czestotliwosciach w zaleznosci od CD
// oraz przesylac globalna indeksowana telemetrie jako ACK do aparatury
// Jezeli cos w tym tasku pojdzie nie tak, ma zostac powiadomiony health monitor, ktory bedzie dalej
// myslec

// Mozna tez utworzyc strukture typu radio_link_status z danymi o linku zeby mogl ja odczytac
// task terminalowy
