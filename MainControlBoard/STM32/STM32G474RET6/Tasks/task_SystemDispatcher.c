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

#include <SPI1_Dma.h> // Temporary include for testing

const char task_SystemDispatcher_name[]="System Dispatcher";

TX_THREAD task_SystemDispatcher_handle SECTION(".task_handles");;
ULONG task_SystemDispatcher_stack[TASKS_SYSTEM_DISPATCHER_STACK_SIZE] SECTION(".task_stacks_ccsram");;

TX_QUEUE task_cli_request_queue;
ULONG task_cli_request_queue_storage[TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest)/sizeof(uint32_t)];

static SystemRequest task_cli_command;

static hspi_data spi_transaction_data;

SYSTEM_MODE system_mode SECTION(".sysDiag"); // Przeniesc do taska trybu systemu

extern TX_QUEUE task_wireless_comm_request_queue; // Request queue to task wireless_comm

void failsafe_stop(void){

}

void fault_stop(void){

}

void parse_cli_queue(SystemRequest*command){
    // For sure we have to check which device to process
    // We have to check first if operation can be done safely
    switch(command->commandType){
        case REQUEST_SET_VALUE:
            parse_cli_queue_request_set_value(command);
            // Placeholder for now
        break;
        case REQUEST_BUS_RAW_DATA:
            parse_cli_queue_request_bus_raw_data(command);
        break;
        case REQUEST_SWITCH_MODE:
            parse_cli_queue_request_switch_mode(command);
        break;
        case REQUEST_GET_RADIO_STATS:
            parse_cli_queue_request_radio_stats(command);
        break;
        case REQUEST_GET_RADIO_RUNTIME_STATS:

        break;
        case REQUEST_GET_RADIO_SETTINGS:

        break;
        default:
        // Should not happen
    }
}

void parse_cli_queue_request_set_value(SystemRequest*command){
    // Placeholder
}

void parse_cli_queue_request_bus_raw_data(SystemRequest*command){
    switch(command->payload.rawData.deviceId){
        // case for all the devices
        // send request to given device specialist
        // For instance task_WirelessComm
        default:

    }
}

void parse_cli_queue_request_switch_mode(SystemRequest*command){
    if((system_mode==SYSTEM_MODE_FAULT)||(system_mode==SYSTEM_MODE_FAILSAFE)){
        *command->commandStatus=SYSTEM_REQUEST_STATUS_ERROR;
        command->callbackFn(command->callbackEvent);
        return;   
    }

    system_mode=command->payload.mode.mode;
    *command->commandStatus=SYSTEM_REQUEST_STATUS_OK;
    command->callbackFn(command->callbackEvent);
}

void parse_cli_queue_request_radio_stats(SystemRequest*command){
    // Enqueue data in the radio command queue
    tx_queue_send(&task_wireless_comm_request_queue,command,0);
}

void task_SystemDispatcher_create(void){
    tx_queue_create(&task_cli_request_queue,
        "CLI Commands",
        sizeof(SystemRequest)/sizeof(uint32_t),
        task_cli_request_queue_storage,
        TASK_CLI_COMMAND_QUEUE_STORAGE_LENGTH*sizeof(SystemRequest));

        system_mode=SYSTEM_MODE_FAILSAFE; // Temporary    
    tx_thread_create(&task_SystemDispatcher_handle,
                    (char*)task_SystemDispatcher_name,
                    task_SystemDispatcher,
                    0,
                    &task_SystemDispatcher_stack,
                    sizeof(task_SystemDispatcher_stack),
                    TASKS_SYSTEM_DISPATCHER_PRIORITY,
                    TASKS_SYSTEM_DISPATCHER_PRIORITY,
                    TX_NO_TIME_SLICE,
                    TX_AUTO_START);
}

void task_SystemDispatcher(ULONG arg){
    UNUSED(arg);

    while(1){
        //queue_wireless_comm_status=tx_queue_receive();
        tx_queue_receive(&task_cli_request_queue,&task_cli_command,TX_WAIT_FOREVER); // For now
        // Later there will be second queue for reading data from task_wireless_comm
        // This data will be send to other mcu's to perform their duties
        parse_cli_queue(&task_cli_command);

        continue;

        switch(system_mode){
            case SYSTEM_MODE_RUN:
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
                // Send data to other mcus
            break;
            case SYSTEM_MODE_TEST:
                // Ignore data from radio task - debug mode
            case SYSTEM_MODE_FAILSAFE:
                // System halt when there is significant package loss
            case SYSTEM_MODE_FAULT:
                // System halt when one of the mcus or hardware is not responding
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