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

void tx_application_define(void* first_unused_memory){
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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

    uint8_t testData[]="Hello world this is DMA test\r\n";
    uint16_t testDataLength=strlen((const char*)testData);

    uint8_t receivedData[128]={0};
    uint16_t receivedDataLength=0;

    UNUSED(receivedData);
    UNUSED(receivedDataLength);
    UNUSED(testDataLength);
    //HAL_Delay(5000);

    tx_kernel_enter();
    while(1){
        while(!usart1_dma_read_data(receivedData,&receivedDataLength,128)){
            // Wait for data
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
