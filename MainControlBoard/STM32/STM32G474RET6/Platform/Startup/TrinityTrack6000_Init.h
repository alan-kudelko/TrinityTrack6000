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
 * @copyright
 * Copyright (c) 2026 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef TRINITY_TRACK6000_INIT_H_
    #define TRINITY_TRACK6000_INIT_H_

#include <TrinityTrack6000_Config.h>

#define INIT_LINE_BUFFER_SIZE 90

/** @name Bootup sequence diagnostics strings
 *  @{
 */
extern const char msg_init_mcuInitialized_info[]; /**< Info1 */
extern const char msg_init_memoryInitialized_info[]; /**< Info2 */
extern const char msg_init_threadx_startup_info[];
/** @} */

#ifdef __cplusplus
  extern "C"{
#endif

/**
  * @brief Memory Initialization Function
  * @param None
  * @retval None
  */
void initializeMemory(void);

#ifdef __cplusplus
  }
#endif

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

	
#endif // TRINITY_TRACK6000_INIT_H_
