/**
 ******************************************************************************
 * @file           main.cpp
 * @brief          Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include <main.h>
#include <string.h>

#include <TrinityTrack6000_Config.h>
#include <TrinityTrack6000_Init.h>
#include <TrinityTrack6000_Diagnostics.h>
#include <TrinityTrack6000_MemInfo.h>
#include <USART1_Dma.h>
#include <SPI1_Dma.h>

#include <NRF24L01.h>

#include <tx_api.h>

#include <tasks.h>

extern "C"{

// Test blink task

TX_THREAD task_blink_handle;
ULONG task_blink_stack[128];

void test_function(UINT(*fptr)(ULONG),ULONG retryTimeout);
UINT delay_function(ULONG timeout);

void task_blink(ULONG arg){
    UNUSED(arg);
    while(1){
        HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
        test_function(tx_thread_sleep,1000);
        
    }
}

void test_spi_callbackFn(void){
    usart1_dma_enq_data((uint8_t*)"Done!\r\n",strlen("Done!\r\n"));
}

void tx_application_define(void* first_unused_memory){
    // Note: Pre-kernel calls
    // Create threads, queues, semaphores, mutexes here
    UNUSED(first_unused_memory);
    tx_thread_create(&task_blink_handle,
                    (char*)"Blink Task",
                    task_blink,
                    0,
                    &task_blink_stack,
                    sizeof(task_blink_stack),
                    1,
                    1,
                    TX_NO_TIME_SLICE,
                    TX_AUTO_START);

    tx_thread_create(&task_CLI_handle,
                    (char*)task_CLI_name,
                    task_CLI,
                    0,
                    &task_CLI_stack,
                    sizeof(task_CLI_stack),
                    TASKS_CLI_PRIORITY,
                    TASKS_CLI_PRIORITY,
                    TX_NO_TIME_SLICE,
                    TX_AUTO_START);
    //tx_thread_create(&task_spi1_handle,(char*)"SPI1 Task",spi1_dma_test,0,&task_spi1_stack,sizeof(task_spi1_stack),1,1,TX_NO_TIME_SLICE,TX_AUTO_START);
    //tx_thread_create(&task_second_device_handle,(char*)"SPI1 Second",spi1_dma_second_device,0,&task_second_device_stack,sizeof(task_second_device_stack),1,1,TX_NO_TIME_SLICE,TX_AUTO_START);
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

void test_function(UINT(*fptr)(ULONG),ULONG retryTimeout){
    fptr(retryTimeout);
}

UINT delay_function(ULONG timeout){
    HAL_Delay(timeout);
    return 0;
}

}

UINT (*ptr)(ULONG);


void test_SPI_communication(){
    // Test will be conducted with two MCP23S17 IO expanders
    // Reset pin is PB2(18) for both devices
    // CS pin for green LED is PB0(17), for red LED is PB1(16)
    // Both devices are on SPI1, so we will use software CS control

    // During the test, there should be reset of both devices
    // Then, according to the datasheet, we need to disable auto-increment mode by writing 0x00 to IOCON register (address 0x0A)
    // After that, we will configure bank A to output
    // Finally, we will write different values to each device to see if they
    // respond correctly (green LED should blink, red LED should be off)

    // After this test, we can be sure that the SPI communication is working correctly and we can proceed with more complex tasks
    // And so that, we can work on SPI1 DMA driver

    uint8_t txData[5]={0};
    uint8_t rxData[5]={0};
    UNUSED(rxData);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET); // Reset both devices
    HAL_Delay(1); // Wait for devices to reset
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET); // Release reset
    HAL_Delay(1); // Wait for devices to be ready
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);

    while(true){
// Disable auto-increment mode for device 1 (green LED)
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET); // Select device 1
    // Write to IOCON register (0x0A) with value 0x00
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x0A; // Register address
    txData[2]|=(1<<5); // SEQOP bit set to 1 to disable auto-increment mode
    //HAL_Delay(10); // Short delay between transactions
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET); // Deselect device 1
    memset(txData,0,sizeof(txData)); // Clear txData for next transaction

    //HAL_Delay(10); // Short delay between transactions
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET); // Select device 1
    //HAL_Delay(10); // Short delay between transactions
    // Set all GPIO to output for device 1 (green LED)
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x00; // IODIRA register address
    txData[2]=0x00; // Set all pins of bank A to output
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET); // Deselect device 1

    memset(txData,0,sizeof(txData)); // Clear txData for next transaction

    //HAL_Delay(10); // Short delay between transactions
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET); // Select device 1
    //HAL_Delay(10); // Short delay between transactions
    // Set all GPIO to output for device 1 (green LED)
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x12; // IODIRA register address
    txData[2]=0xFF; // Set all pins of bank A to output
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete

    memset(txData,0,sizeof(txData)); // Clear txData for next transaction
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET); // Deselect device 1
// Disable auto-increment mode for device 2 (red LED)
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); // Select device 2
    // Write to IOCON register (0x0A) with value 0x00
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x0A; // Register address
    txData[2]|=(1<<5); // SEQOP bit set to 1 to disable auto-increment mode
    //HAL_Delay(10); // Short delay between transactions
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); // Deselect device 2
    memset(txData,0,sizeof(txData)); // Clear txData for next transaction

    //HAL_Delay(10); // Short delay between transactions
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); // Select device 1
    //HAL_Delay(10); // Short delay between transactions
    // Set all GPIO to output for device 1 (green LED)
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x00; // IODIRA register address
    txData[2]=0x00; // Set all pins of bank A to output
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); // Deselect device 1

    memset(txData,0,sizeof(txData)); // Clear txData for next transaction

    //HAL_Delay(10); // Short delay between transactions
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET); // Select device 1
    //HAL_Delay(10); // Short delay between transactions
    // Set all GPIO to output for device 1 (green LED)
    txData[0]|=(1<<6); // Device OP-Code
    txData[1]=0x12; // IODIRA register address
    txData[2]=0xFF; // Set all pins of bank A to output
    HAL_SPI_Transmit(&hspi1,txData,3,HAL_MAX_DELAY);
    //HAL_Delay(10); // Wait for transmission to complete

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); // Deselect device 1
    break;
    }
    // Check ioc, there is a pending code generation
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void){
    initializeSystem();
    // Handle non critical errors which in worst case result
    // in system's few functionalities unavailable
    ramInfoRefresh();
    ramInfoGeneral(delay_function,100);
    ramInfoRAM1(delay_function,100);
    ramInfoRAM2(delay_function,100);
    ramInfoCCSRAM(delay_function,100);

        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,500);

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,500);

        test_SPI_communication();

    HAL_Delay(100);

    uint32_t adc_buffer[4]{0};

    while(false){
        HAL_ADC_Start_DMA(&hadc1,adc_buffer,4);
        
        HAL_Delay(1000);
    }

    nrf24l01_test();

    tx_kernel_enter();

    return 0;
}

// Dobra punkt wejscia po ogarnieciu ADC i DMA
// No to tak klasa do NRF24L01 i testy komunikacyjne
// Na razie sprawdzic czy dla tego sterownika SPI1 bedzie to chodzic
// Interfejs do udostepniania wskaznikow na bufory wewnetrzne
// Bufory wewnetrzne statycznie alokowane w sekcji .dma
// Nad samym szkieletem sie pomysli oraz nad api
// Potem osobny task komunikacyjny do SPI
// Nastepnie rozbudowa na przetestowanie komunikacji z przelaczaniem stron
// Odbiorca po otrzymaniu paczki staje sie wysylajacym
// Potrzebny jest drugi uklad do testow np. jakis AVR
// ALe to juz raczej bez CMake i moze byc to zwykle arduino z obsluga SPI
// Dwie funkcje startowe
// Jedna bedzie sluzyc do wyznaczania procenta utraconych pakietow (do testow po zlutowaniu PCB)
// Druga do tymczasowego przesylania danych i telemetrii od urzadzenia do urzadzenia

// Dobra do rozwiazania jest na pewno problem z podwojnym przerwaniem konwersji (nie mam zielonego pojecia czemu)
// Bo najlepsze, ze przerwanie DMA_IT_HT jest zablokowane oraz jest wyczyszczona jego flaga
// Dodatkowo apropo ADC trzeba wprowadzic kompensacje napiecia zasilania podpietego do VREF+


// Co do samego DMA nalezy przeprowadzic balans
// Tzn. operacje najczesciej wykonywane i najciesze powinny trafic do dwoch roznych
// Kontrolerow DMA

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
