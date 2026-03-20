/**
 * @file task_CommandExecutor.c
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

// Doxy z opisem co to ma robic

#include <stm32g4xx_hal.h>

#include <task_SystemDispatcher.h>

#include <task_ModeManager.h>

#include <SPI1_Dma.h> // Temporary include for testing

const char task_SystemDispatcher_name[]="System Dispatcher";

TX_THREAD task_SystemDispatcher_handle SECTION(".task_handles");;
ULONG task_SystemDispatcher_stack[TASKS_SYSTEM_DISPATCHER_STACK_SIZE] SECTION(".task_stacks_ccsram");;

TX_QUEUE task_cli_request_queue;
ULONG task_cli_request_queue_storage[TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest)/sizeof(uint32_t)];

static SystemRequest task_cli_command;

static hspi_data spi_transaction_data;

extern TX_QUEUE task_cli_feedback_queue;

extern SYSTEM_MODE system_mode;


void failsafe_stop(void){

}

void fault_stop(void){

}

void parse_cli_queue(SystemRequest command){
    
}

void task_SystemDispatcher_init(void){
    // For now just a placeholder
    tx_queue_create(&task_cli_request_queue,
        "CLI Commands",
        sizeof(SystemRequest)/sizeof(uint32_t),
        task_cli_request_queue_storage,
        TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest)/sizeof(uint32_t));

        system_mode=RUN; // Temporary
}

void task_SystemDispatcher(ULONG arg){
    UNUSED(arg);
    uint32_t queue_cli_status;
    UNUSED(queue_cli_status);

    //uint32_t queue_wireless_comm_status;

    while(1){
        //queue_wireless_comm_status=tx_queue_receive();
        tx_queue_receive(&task_cli_request_queue,&task_cli_command,TX_WAIT_FOREVER);

        switch(system_mode){
            case RUN:
                spi_transaction_data.txBuffer=task_cli_command.payload.rawData.txBuffer;
                spi_transaction_data.txLength=task_cli_command.payload.rawData.txLength;
                spi_transaction_data.rxBuffer=task_cli_command.payload.rawData.rxBuffer;
                spi_transaction_data.rxLength=task_cli_command.payload.rawData.rxLength;
                spi_transaction_data.callbackFn=task_cli_command.callbackFn;
                spi_transaction_data.gpio_port=GPIOB;
                if(task_cli_command.payload.rawData.deviceId==DEVICE_MCP1){
                    spi_transaction_data.gpio_port=GPIOB;
                    spi_transaction_data.gpio_pin=GPIO_PIN_1;
                }
                else if(task_cli_command.payload.rawData.deviceId==DEVICE_MCP2){
                    spi_transaction_data.gpio_port=GPIOA;
                    spi_transaction_data.gpio_pin=GPIO_PIN_15;
                }
                spi1_dma_enq_data(&spi_transaction_data);
            break;
            case TEST:
                
            case FAILSAFE:

            case FAULT:

            default:
            // Should not happen

        }
    }
}

// Dobra co zrobic?
// 2. dorobic tryb sprawdzania trybu systemu w tasku
// 3. zaprojektowac strukture sterujaca przekazywana z taska CLI do CommandExecutora
// 4. Tymczasowo command executor niech obsluguje te zapytania potem zrobic oddelegowywanie sterowan do dedykowanych taskow
// 5. Przeniesc przetwarzanie do jakiegos zarzadcy sprzetu

// Ale to zanim to naprawic tworzenie kolejki i zwrocic uwage na smaczki threadx'a