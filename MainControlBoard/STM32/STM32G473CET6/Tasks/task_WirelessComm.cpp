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

#include <task_WirelessComm.h>
#include <TrinityTrack6000_Pinout.h>

const static NRF_SETTINGS nrf_default_settings{
    NRF_BIT_TX_DS|NRF_BIT_MAX_RT|NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP|NRF_BIT_PRIM_RX, // config
    NRF_BIT_ENAA_P0, // en_aa
    NRF_BIT_ERX_P0,  // en_rxaddr
    NRF_BIT_AW1|NRF_BIT_AW0, // setup_aw
    0x00, // setup_retr
    NRF_BIT_RF_CH6, // rf_ch
    NRF_BIT_RF_DR|NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1|NRF_BIT_RF_LNA_HCURR, // rf_setup
    RADIO_DEFAULT_ADDRESS, // rx_addr_p0
    {0x00}, // rx_addr_p1
    0x00, // rx_addr_p2
    0x00, // rx_addr_p3
    0x00, // rx_addr_p4
    0x00, // rx_addr_p5
    RADIO_DEFAULT_ADDRESS, // tx_addr
    RADIO_DEFAULT_PAYLOAD_SIZE_32, // rx_pw_p0
    0x00, // rx_pw_p1
    0x00, // rx_pw_p2
    0x00, // rx_pw_p3
    0x00, // rx_pw_p4
    0x00, // rx_pw_p5
    0x00, // dynpd
    NRF_BIT_EN_ACK_PAY // feature
};

const char task_wireless_comm_name[]="Wireless Task";
TX_THREAD task_wireless_comm_handle SECTION(".task_handles");
ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE] SECTION(".task_stacks_ccsram");

/** 
 * @name task_WirelessComm Initialization Internal state
 * @brief Internal initialization 
*/

static TX_SEMAPHORE task_wireless_comm_wakeup_sem;

static volatile uint8_t initializationStepDone=0;

static NRF24L01 nrf24l01(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,NRF24L01_CS_GPIO_Port,NRF24L01_CS_Pin,NRF24L01_IRQ_GPIO_Port,NRF24L01_IRQ_Pin);

static void initializationStepDone_callback(void){
    initializationStepDone++;
}

/**@} */

extern "C" void radioDataReceived_callback(void){
    // For now just a placeholder
    tx_semaphore_put(&task_wireless_comm_wakeup_sem);
}

extern "C" void nrf24l01_display_all_registers();

extern "C" void nrf24l01_init(void){
    uint8_t txBuffer[OPERATION_BUFFER_SIZE];
    uint8_t rxBuffer[OPERATION_BUFFER_SIZE];

    nrf24l01.attach_rx_buffer(rxBuffer);
    nrf24l01.attach_tx_buffer(txBuffer);
    nrf24l01.attach_callback_function(initializationStepDone_callback);

    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,GPIO_PIN_RESET);

// 0. Write PWR_UP in CONFIG register for faster startup
    while(!nrf24l01.write_reg_config(NRF_BIT_PWR_UP)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 1. Feature register activation
// Note that during software reset NRF is still powered so that it should remember thas it has been activated
// On the other hand, during first power up activation is required
// It is advised to write desired feature register bit mask and read it
// if value is different than desired (typically 0x00) this function must be activated

    // Try to write feature register
    while(!nrf24l01.write_reg_feature(nrf_default_settings.feature)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
    // Read feature register
    while(!nrf24l01.read_reg_feature()){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;

    if(rxBuffer[1]!=nrf_default_settings.feature){
        // Feature disabled, activate feature
        while(!nrf24l01.activate()){
            __NOP();
        }
        while(initializationStepDone==0){
            __NOP();
        }
        initializationStepDone=0;
        // Try again
        while(!nrf24l01.write_reg_feature(nrf_default_settings.feature)){
            __NOP();
        }
        while(initializationStepDone==0){
            __NOP();
        }
        initializationStepDone=0;
    }
// 2. DYNPD
    while(!nrf24l01.write_reg_dynpd(nrf_default_settings.dynpd)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 3. CONFIG
    while(!nrf24l01.write_reg_config(nrf_default_settings.config)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;    
// 4. EN_AA
    while(!nrf24l01.write_reg_en_aa(nrf_default_settings.en_aa)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 5. EN_RXADDR
    while(!nrf24l01.write_reg_en_rxAddr(nrf_default_settings.en_rxaddr)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 6. SETUP_AW
    while(!nrf24l01.write_reg_setup_aw(nrf_default_settings.setup_aw)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 7. SETUP_RETR
    while(!nrf24l01.write_reg_setup_retr(nrf_default_settings.setup_retr)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 8. RF_CH
    while(!nrf24l01.write_reg_rf_ch(nrf_default_settings.rf_ch)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 9. RF_SETUP
    while(!nrf24l01.write_reg_rf_setup(nrf_default_settings.rf_setup)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 10. RX_ADDR_P0
    while(!nrf24l01.write_reg_rx_addr_p0(nrf_default_settings.rx_addr_p0,RADIO_DEFAULT_ADDRESS_LENGTH)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 11. RX_ADDR_P1
    while(!nrf24l01.write_reg_rx_addr_p1(nrf_default_settings.rx_addr_p1,RADIO_DEFAULT_ADDRESS_LENGTH)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 12. RX_ADDR_P2
    // FIX API of the NRF24L01 class
    // while(!nrf24l01.write_reg_rx_addr_p2(nrf_default_settings.rx_addr_p2)){
    //     __NOP();
    // }
    // while(initializationStepDone==0){
    //     __NOP();
    // }
    // initializationStepDone=0;
// 13. RX_ADDR_P3
    // FIX API of the NRF24L01 class
    // while(!nrf24l01.write_reg_rx_addr_p3(nrf_default_settings.rx_addr_p3)){
    //     __NOP();
    // }
    // while(initializationStepDone==0){
    //     __NOP();
    // }
    // initializationStepDone=0;
// 14. RX_ADDR_P4
    // FIX API of the NRF24L01 class
    // while(!nrf24l01.write_reg_rx_addr_p4(nrf_default_settings.rx_addr_p4)){
    //     __NOP();
    // }
    // while(initializationStepDone==0){
    //     __NOP();
    // }
    // initializationStepDone=0;
// 15. RX_ADDR_P5
    // FIX API of the NRF24L01 class
    // while(!nrf24l01.write_reg_rx_addr_p5(nrf_default_settings.rx_addr_p5)){
    //     __NOP();
    // }
    // while(initializationStepDone==0){
    //     __NOP();
    // }
    // initializationStepDone=0;
// 16. TX_ADDR
    while(!nrf24l01.write_reg_tx_addr(nrf_default_settings.tx_addr,RADIO_DEFAULT_ADDRESS_LENGTH)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 17. RX_PW_P0
    while(!nrf24l01.write_reg_rx_pw_p0(nrf_default_settings.rx_pw_p0)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 18. RX_PW_P1
    while(!nrf24l01.write_reg_rx_pw_p1(nrf_default_settings.rx_pw_p1)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 19. RX_PW_P2
    while(!nrf24l01.write_reg_rx_pw_p2(nrf_default_settings.rx_pw_p2)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 20. RX_PW_P3
    while(!nrf24l01.write_reg_rx_pw_p3(nrf_default_settings.rx_pw_p3)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 21. RX_PW_P4
    while(!nrf24l01.write_reg_rx_pw_p4(nrf_default_settings.rx_pw_p4)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 22. RX_PW_P5
    while(!nrf24l01.write_reg_rx_pw_p5(nrf_default_settings.rx_pw_p5)){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 23. FLUSH RX
    while(!nrf24l01.flush_rx()){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 24. FLUSH TX
    while(!nrf24l01.flush_tx()){
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;
// 25. Clear STATUS flags
    while(!nrf24l01.write_reg_status(0x70)){ // FIX Magic number
        __NOP();
    }
    while(initializationStepDone==0){
        __NOP();
    }
    initializationStepDone=0;


    HAL_Delay(1);
    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,GPIO_PIN_SET);
    nrf24l01_display_all_registers();
}

extern "C" void task_wireless_comm_init(void){
    tx_semaphore_create(&task_wireless_comm_wakeup_sem,
       nullptr,
       0);
}

extern "C" void task_wireless_comm(ULONG arg){
    UNUSED(arg);

// For now just for architecture testing
// In the future body of this task will be dedicated to handle wireless rx/tx communication

    while(true){
        tx_semaphore_get(&task_wireless_comm_wakeup_sem,TX_WAIT_FOREVER); // For debug time


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
