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
#include <USART1_Dma.h>
#include <string.h>
#include <stdio.h>
#include <system_commands.h>

const char task_wireless_comm_name[]="Wireless Task";
TX_THREAD task_wireless_comm_handle SECTION(".task_handles");
ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE] SECTION(".task_stacks_ccsram");

TX_QUEUE task_wireless_comm_request_queue;
ULONG task_wireless_comm_request_queue_storage[TASK_WIRELESS_COMM_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest)/sizeof(uint32_t)];

/** 
 * @name task_WirelessComm Initialization Internal state
 * @brief Internal initialization
 * @{
*/

static SystemRequest request;

static TX_SEMAPHORE task_wireless_comm_wakeup_sem;
static TX_SEMAPHORE task_wireless_comm_operation_done_sem; // Semaphore indicating that SPI request has been processed

static NRF24L01 nrf24l01(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,NRF24L01_CS_GPIO_Port,NRF24L01_CS_Pin,NRF24L01_IRQ_GPIO_Port,NRF24L01_IRQ_Pin);

static NRF_SETTINGS nrf_default_settings{
    NRF_BIT_TX_DS|NRF_BIT_MAX_RT|NRF_BIT_EN_CRC|NRF_BIT_CRCO|NRF_BIT_PWR_UP|NRF_BIT_PRIM_RX, // config
    NRF_BIT_ENAA_P0, // en_aa
    NRF_BIT_ERX_P0,  // en_rxaddr
    NRF_BIT_AW1|NRF_BIT_AW0, // setup_aw
    0x00, // setup_retr
    NRF_BIT_RF_CH1|NRF_BIT_RF_CH5|NRF_BIT_RF_CH4, // rf_ch
    NRF_BIT_RF_PWR2|NRF_BIT_RF_PWR1|NRF_BIT_RF_LNA_HCURR, // rf_setup
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

static uint8_t txBuffer[NRF_SPI_REQUEST_MAX_LENGTH]{0};
static uint8_t rxBuffer[NRF_SPI_REQUEST_MAX_LENGTH]{0};
static uint8_t ackPayload[NRF_SPI_REQUEST_MAX_LENGTH]{0};
static uint8_t ackDummy[NRF_SPI_REQUEST_MAX_LENGTH]{0};

// Handler containing all the radio parameters

radio_t radio{
    &nrf24l01, // nrf handler
    txBuffer, 
    rxBuffer,
    ackPayload, // ack payload
    ackDummy,   // ack payload rx dummy buffer
    radioOperationDone_callback,
    0, // Fix magic number later
    NRF_PAYLOAD_MAX_LENGTH,
    NRF_ADDRESS_MAX_LENGTH,
};

/**@} */

//static NRF_RUNTIME_STATUS nrf_runtime_status{0};

static RADIO_STATS radio_stats{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1
};

extern "C" void radioDataReceived_callback(uint8_t event){
    // For now just a placeholder
    UNUSED(event);
    tx_semaphore_put(&task_wireless_comm_wakeup_sem);
}

extern "C" void radioOperationDone_callback(uint8_t event){
    UNUSED(event);
    tx_semaphore_put(&task_wireless_comm_operation_done_sem);
}

extern "C" void task_wireless_comm_init(void){
    tx_semaphore_create(&task_wireless_comm_wakeup_sem,
       (char*)"Wireless Wakeup",
       0);
    tx_semaphore_create(&task_wireless_comm_operation_done_sem,
        (char*)"Wireless op done",
        0);
    tx_queue_create(&task_wireless_comm_request_queue,
        (char*)"Wireless request queue",
        sizeof(SystemRequest)/sizeof(uint32_t),
        task_wireless_comm_request_queue_storage,
        TASK_WIRELESS_COMM_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest));
}

extern "C" void task_wireless_comm_write_settings(NRF_SETTINGS*settings){
    radio.dev->attach_tx_buffer(radio.txBuffer);
    radio.dev->attach_rx_buffer(radio.rxBuffer);
    radio.dev->attach_callback_function(radio.callbackFn,0);
    radio.callbackEvent=0;

    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,GPIO_PIN_RESET);

// 0. CONFIG PWR_UP if set for faster startup
    while(!radio.dev->write_reg_config(settings->config&NRF_BIT_PWR_UP)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 1. FEATURE register activation
    do{
        // 1.1 ACTIVATE Feature and DYNPD registers
        while(!radio.dev->activate()){
            tx_thread_sleep(1);
        }
        tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
        // 1.2 FEATURE write
        while(!radio.dev->write_reg_feature(settings->feature)){
            tx_thread_sleep(1);
        }
        tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
        // 1.4 Verify if register's value is correct
        while(!radio.dev->read_reg_feature()){
            tx_thread_sleep(1);
        }
        tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);    
        // 1.5 Retry if needed (activate command works by toggling)
    }while(radio.rxBuffer[1]!=settings->feature);
// 2. DYNPD
    while(!radio.dev->write_reg_dynpd(settings->dynpd)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 3. CONFIG
    while(!radio.dev->write_reg_config(settings->config)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 4. EN_AA
    while(!radio.dev->write_reg_en_aa(settings->en_aa)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 5. EN_RXADDR
    while(!radio.dev->write_reg_en_rxAddr(settings->en_rxaddr)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 6. SETUP_AW
    while(!radio.dev->write_reg_setup_aw(settings->setup_aw)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 7. SETUP_RETR
    while(!radio.dev->write_reg_setup_retr(settings->setup_retr)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 8. RF_CH
    while(!radio.dev->write_reg_rf_ch(settings->rf_ch)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 9. RF_SETUP
    while(!radio.dev->write_reg_rf_setup(settings->rf_setup)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 10. RX_ADDR_P0
    while(!radio.dev->write_reg_rx_addr_p0(settings->rx_addr_p0,radio.addressLength)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 11. RX_ADDR_P1
    while(!radio.dev->write_reg_rx_addr_p1(settings->rx_addr_p1,radio.addressLength)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 12. RX_ADDR_P2
    while(!radio.dev->write_reg_rx_addr_p2(settings->rx_addr_p2)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 13. RX_ADDR_P3
    while(!radio.dev->write_reg_rx_addr_p3(settings->rx_addr_p3)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 14. RX_ADDR_P4
    while(!radio.dev->write_reg_rx_addr_p4(settings->rx_addr_p4)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 15. RX_ADDR_P5
    while(!radio.dev->write_reg_rx_addr_p5(settings->rx_addr_p5)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 16. TX_ADDR
    while(!radio.dev->write_reg_tx_addr(settings->tx_addr,radio.addressLength)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 17. RX_PW_P0
    while(!radio.dev->write_reg_rx_pw_p0(settings->rx_pw_p0)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 18. RX_PW_P1
    while(!radio.dev->write_reg_rx_pw_p1(settings->rx_pw_p1)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 19. RX_PW_P2
    while(!radio.dev->write_reg_rx_pw_p2(settings->rx_pw_p2)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 20. RX_PW_P3
    while(!radio.dev->write_reg_rx_pw_p3(settings->rx_pw_p3)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 21. RX_PW_P4
    while(!radio.dev->write_reg_rx_pw_p4(settings->rx_pw_p4)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 22. RX_PW_P5
    while(!radio.dev->write_reg_rx_pw_p5(settings->rx_pw_p5)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 23. FLUSH TX
    while(!radio.dev->flush_tx()){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 24. FLUSH RX
    while(!radio.dev->flush_rx()){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);
// 25. STATUS Clear
    while(!radio.dev->write_reg_status(NRF_REG_STATUS_CLEAR_MASK)){
        tx_thread_sleep(1);
    }
    tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);

    tx_thread_sleep(3);

    __HAL_GPIO_EXTI_CLEAR_IT(NRF24L01_IRQ_Pin);
    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port,NRF24L01_CE_Pin,GPIO_PIN_SET);
}

extern "C" void task_wireless_comm_update_radio_stats(RADIO_STATS*radio_stats,uint8_t packet_id){
    static uint8_t packages_lost;

    radio_stats->total_packages++;

    if(radio_stats->first_packet){
        // Check if this is the first packet in the session
        radio_stats->expected_id=packet_id+1;
        radio_stats->last_id=packet_id;
        radio_stats->packages_received++;
        radio_stats->first_packet=0;
        return;
    }
    if(radio_stats->expected_id==packet_id){
        // No drop
        radio_stats->expected_id++;
        radio_stats->last_id=packet_id;
        radio_stats->packages_received++;
    }
    else if(radio_stats->expected_id<packet_id){
        // Packages dropped
        packages_lost=packet_id-radio_stats->expected_id;

        radio_stats->packages_dropped+=packages_lost;
        radio_stats->expected_id=packet_id+1;
        radio_stats->last_id=packet_id;
        radio_stats->packages_received++;
    }
    else{
        if(radio_stats->last_id==packet_id){
            // Duplicate
            radio_stats->packages_duplicate++;
        }
        else{
            radio_stats->packages_out_of_order++;
        }
    }
}

extern "C" void task_wireless_comm_process_request(void){
    switch(request.commandType){
        case REQUEST_BUS_RAW_DATA:

        break;
        case REQUEST_GET_RADIO_STATS:
            task_wireless_comm_process_request_radio_stats();
        break;

        default:
        
        break;
    }
}

extern "C" void task_wireless_comm_process_request_radio_stats(void){
    *request.payload.radioStats=radio_stats;
    *request.commandStatus=SYSTEM_REQUEST_STATUS_OK;
    request.callbackFn(request.callbackEvent);
}

extern "C" void task_wireless_comm(ULONG arg){
    UNUSED(arg);

    //nrf24l01.attach_callback_function(nullptr,0);
    //nrf24l01.write_ack_payload(32,NRF_RX_PIPE0);
    task_wireless_comm_write_settings(&nrf_default_settings);
// For now just for architecture testing
// In the future body of this task will be dedicated to handle wireless rx/tx communication
// Change semaphore to queue in order to process requests from other tasks


    while(true){
        if(tx_queue_receive(&task_wireless_comm_request_queue,&request,0)==TX_SUCCESS){
            task_wireless_comm_process_request();
        }

        if(tx_semaphore_get(&task_wireless_comm_wakeup_sem,12000)==TX_SUCCESS){ // For debug time
        // Read data
        radio.dev->attach_callback_function(radioOperationDone_callback,0);

        do{
            while(!radio.dev->read_rx_payload(radio.payloadLength)){
                tx_thread_sleep(1);
            }
            tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);

            // Update radio_stats
            task_wireless_comm_update_radio_stats(&radio_stats,radio.rxBuffer[FRAME_FIELD_ID]);

            while(!radio.dev->read_reg_fifo_status()){
                tx_thread_sleep(1);
            }
            tx_semaphore_get(&task_wireless_comm_operation_done_sem,TX_WAIT_FOREVER);

        }while(!(radio.rxBuffer[1]&NRF_BIT_FIFO_RX_EMPTY));

        // write out data on serial port
        // clear irq
            radio.dev->attach_callback_function(nullptr,0);
            while(!radio.dev->write_reg_status(NRF_REG_STATUS_CLEAR_MASK)){
                tx_thread_sleep(1);
            }
        }
        else{
            
            
        }
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
