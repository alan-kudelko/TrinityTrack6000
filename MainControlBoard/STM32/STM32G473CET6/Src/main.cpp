/**
 ******************************************************************************
 * @file           main.c
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

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void){
    initializeSystem();

    // Handle non critical errors which in worst case result
    // in system's few functionalities unavailiable

    ramInfoGeneral();
    ramInfoRAM1();
    ramInfoRAM2();
    ramInfoCCSRAM();

    while(1){
    HAL_GPIO_TogglePin(ARM_GUN_GPIO_Port,ARM_GUN_Pin);
    HAL_Delay(1000);
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
