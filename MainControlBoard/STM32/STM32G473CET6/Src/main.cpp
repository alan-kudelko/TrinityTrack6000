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

#include <tx_api.h>

extern "C"{

void tx_application_define(void* first_unused_memory){
    // Create threads, queues, semaphores, mutexes here
}

void _tx_initialize_low_level(void){
    // Low level initialization before ThreadX starts
}

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
    ramInfoGeneral();
    ramInfoRAM1();
    ramInfoRAM2();
    ramInfoCCSRAM();

    uint8_t testData[]="Hello world this is DMA test\r\n";
    uint16_t testDataLength=strlen((const char*)testData);

    uint8_t receivedData[128]={0};
    uint16_t receivedDataLength=0;

    UNUSED(receivedData);
    UNUSED(receivedDataLength);
    UNUSED(testDataLength);

    usart1_dma_rx_init();

    tx_kernel_enter();
    while(1){
        //while(!usart1_dma_read_data(receivedData,&receivedDataLength,128)){
            // Wait for data
       // }
       while(1){
        // Debug loop
       }
        receivedData[receivedDataLength-1]='\r';
        receivedData[receivedDataLength]='\n';
        receivedDataLength+=1;
        usart1_dma_enq_data(receivedData,receivedDataLength);
        // Simple echo test

        //HAL_UART_Receive(&huart1,receivedData,128,DEBUG_UART_TIMEOUT);
        //receivedDataLength=strlen((const char*)receivedData);
        //HAL_UART_Transmit(&huart1,receivedData,receivedDataLength,DEBUG_UART_TIMEOUT);
        //HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
        //HAL_Delay(500);
    }

    return 0;
}

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
