/**
 * @file TrinityTrack6000_Init.h
 * @brief System initialization functions for TrinityTrack6000 project.
 *
 * This header provides prototypes for initialization routines including
 * system clock, HAL, GPIO, UART, and the overall system setup.  
 * It also defines error handling and assert reporting mechanisms.
 *
 * @date 2025.09.08
 * @author Alan Kudełko
 */

#ifndef _TRINITY_TRACK6000_INIT_H_
	#define _TRINITY_TRACK6000_INIT_H_

#include <TrinityTrack6000_Config.h>

/** @name Bootup sequence diagnostics strings
 *  @{
 */

extern const char msg_initializeHAL_info[]; /**< Info1 */
extern const char msg_initializeClock_info[]; /**< Info1 */
extern const char msg_initializeGPIO_info[]; /**< Info1 */
extern const char msg_initializeUART_info[]; /**< Info1 */
extern const char msg_initializeRAMDia_info[]; /**< Info1 */
/** @} */

#ifdef __cplusplus
	extern "C"{
#endif // __cplusplus

/**
  * @brief HAL Initialization Function
  * @param None
  * @retval None
  */
void initializeHAL(void);

/**
  * @brief System Clock Configuration
  * @retval None
  */
void initializeClock(void);

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void initializeGPIO(void);

/**
  * @brief USART Initialization Function
  * @param None
  * @retval None
  */
void initializeUART(void);

/**
  * @brief Memory Initialization Function
  * @param None
  * @retval None
  */
void initializeMemory(void);

/**
 * @brief System Initialization Function
 * @param None
 * @retval None
 */
void initializeSystem(void);

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void);

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line){
  	/* USER CODE BEGIN 6 */
  	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
	}
#endif // __cplusplus
	
#endif // _TRINITY_TRACK6000_INIT_H_
