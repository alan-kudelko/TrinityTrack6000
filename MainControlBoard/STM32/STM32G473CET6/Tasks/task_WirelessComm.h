/**
 * @file task_WirelessComm.h
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

#ifndef TASK_WIRELESS_COMM_H_
    #define TASK_WIRELESS_COMM_H_

#ifdef __DOXYGEN__
    #define SECTION(x)
    #define ALIGNED(x)
#else
    #define SECTION(x) __attribute((section(x)))
    #define ALIGNED(x) __attribute((aligned(x)))
#endif // __DOXYGEN__

#include <tx_api.h>

#include <stdint.h>

#include <NRF24L01.h>
#include <Radio_Types.h>

#define OPERATION_BUFFER_SIZE 33

#define TASK_WIRELESS_COMM_STACK_SIZE 1512 /**< Stack size for wireless comm task */
#define TASK_WIRELESS_COMM_PRIORITY 3     /**< Priority for wireless comm task */

#define TASK_WIRELESS_COMM_COMMAND_QUEUE_STORAGE_LENGTH 3

typedef struct{
    NRF24L01*dev; // Pointer to NRF24L01 class object
    uint8_t*txBuffer; // Pointer to tx buffer for read/write operations
    uint8_t*rxBuffer; // Pointer to rx buffer for read/write operations
    uint8_t*txAckPayload; // Pointer to ack payload
    uint8_t*rxAckDummy;   // Pointer to dummy rx buffer used in write ack payload operation
    void(*callbackFn)(uint8_t event);
    uint8_t callbackEvent;
    uint8_t payloadLength;
    uint8_t addressLength;
}radio_t;

// Note: there are two sets of buffers since txBuffer and rxBuffer operations have to be synchronized
// Via semafor, but writing ack payload doesn't need synchronization.

extern const char task_wireless_comm_name[]; /**< Name of the wireless comm task */

extern TX_THREAD task_wireless_comm_handle;

extern ULONG task_wireless_comm_stack[TASK_WIRELESS_COMM_STACK_SIZE];

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

extern void radioDataReceived_callback(uint8_t event);

extern void radioOperationDone_callback(uint8_t event);

extern void task_wireless_comm_init(void);

extern void task_wireless_comm_write_settings(NRF_SETTINGS*settings);

extern void task_wireless_comm_read_settings(void);

extern bool task_wireless_comm_verify_settings(void);

extern void task_wireless_comm_update_radio_stats(RADIO_STATS*radio_stats,uint8_t packet_id);

extern void task_wireless_comm_process_request(void);

extern void task_wireless_comm_process_request_radio_stats(void);

extern void task_wireless_comm(ULONG arg);

// Klasa nrf24l01 nie musi byc już zmieniana
// Teraz tylko funkcje pomocnicze taska radiowego znające threadx'a
// Więc do zmiany jest widoczność semafora na pewno
// I można zamknąć część operacji w funkcje i trochę tu przejrzy
// funkcje takie jak kolejkowanie zapytań mogą być inline
// Ważna uwaga, bo może być potrzebny atrybut always inline ale to zobaczymy
// Te api nad którym wczoraj pracowałem jest do wyrzucenia xD
// Struktury mogą zostać
// Natomiast z racji dużego zapasu czasu CPU na obsługę pakietu nawet przy 500Hz
// Możemy zostawić oczekiwanie na semafor - nie rozwala to timingu

// Można się również pokusić o zmianę sterownika SPI z podtrzymaniem pinu CS między transferami
// Ale to nie jest aktualnie priorytet

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_WIRELESS_COMM_H_