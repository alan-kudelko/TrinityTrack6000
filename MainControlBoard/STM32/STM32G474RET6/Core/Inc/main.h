/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NC_5_Pin GPIO_PIN_13
#define NC_5_GPIO_Port GPIOC
#define KILL_SWITCH_Pin GPIO_PIN_14
#define KILL_SWITCH_GPIO_Port GPIOC
#define WATCHDOG_EN_Pin GPIO_PIN_15
#define WATCHDOG_EN_GPIO_Port GPIOC
#define ARM_GUN_Pin GPIO_PIN_0
#define ARM_GUN_GPIO_Port GPIOF
#define FIRE_GUN_Pin GPIO_PIN_1
#define FIRE_GUN_GPIO_Port GPIOF
#define GPS_STANDBY_Pin GPIO_PIN_0
#define GPS_STANDBY_GPIO_Port GPIOC
#define WATCHDOG_FEED_Pin GPIO_PIN_1
#define WATCHDOG_FEED_GPIO_Port GPIOC
#define MQ_HEATERS_Pin GPIO_PIN_2
#define MQ_HEATERS_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_3
#define BUZZER_GPIO_Port GPIOC
#define MQ6_IN_Pin GPIO_PIN_0
#define MQ6_IN_GPIO_Port GPIOA
#define MQ7_IN_Pin GPIO_PIN_1
#define MQ7_IN_GPIO_Port GPIOA
#define GPS_RST_Pin GPIO_PIN_4
#define GPS_RST_GPIO_Port GPIOA
#define NRF24L01_PWR_Pin GPIO_PIN_4
#define NRF24L01_PWR_GPIO_Port GPIOC
#define NRF24L01_CE_Pin GPIO_PIN_5
#define NRF24L01_CE_GPIO_Port GPIOC
#define NRF24L01_CS_Pin GPIO_PIN_0
#define NRF24L01_CS_GPIO_Port GPIOB
#define FRAM_CS_Pin GPIO_PIN_1
#define FRAM_CS_GPIO_Port GPIOB
#define NRF24L01_IRQ_Pin GPIO_PIN_2
#define NRF24L01_IRQ_GPIO_Port GPIOB
#define NRF24L01_IRQ_EXTI_IRQn EXTI2_IRQn
#define NC_4_Pin GPIO_PIN_10
#define NC_4_GPIO_Port GPIOB
#define INFINEON_RST_Pin GPIO_PIN_11
#define INFINEON_RST_GPIO_Port GPIOB
#define INFINEON_CS_Pin GPIO_PIN_12
#define INFINEON_CS_GPIO_Port GPIOB
#define INFINEON_RDY_Pin GPIO_PIN_6
#define INFINEON_RDY_GPIO_Port GPIOC
#define INFINEON_RDY_EXTI_IRQn EXTI9_5_IRQn
#define NC_3_Pin GPIO_PIN_7
#define NC_3_GPIO_Port GPIOC
#define NC_2_Pin GPIO_PIN_8
#define NC_2_GPIO_Port GPIOC
#define NC_1_Pin GPIO_PIN_9
#define NC_1_GPIO_Port GPIOC
#define RENESANS_CS_Pin GPIO_PIN_10
#define RENESANS_CS_GPIO_Port GPIOA
#define NXP_CS_Pin GPIO_PIN_11
#define NXP_CS_GPIO_Port GPIOA
#define FPGA_CS_Pin GPIO_PIN_12
#define FPGA_CS_GPIO_Port GPIOA
#define RENESANS_RDY_Pin GPIO_PIN_15
#define RENESANS_RDY_GPIO_Port GPIOA
#define RENESANS_RDY_EXTI_IRQn EXTI15_10_IRQn
#define RENESANS_RST_Pin GPIO_PIN_2
#define RENESANS_RST_GPIO_Port GPIOD
#define NXP_RST_Pin GPIO_PIN_3
#define NXP_RST_GPIO_Port GPIOB
#define FPGA_RST_Pin GPIO_PIN_4
#define FPGA_RST_GPIO_Port GPIOB
#define NXP_RDY_Pin GPIO_PIN_5
#define NXP_RDY_GPIO_Port GPIOB
#define NXP_RDY_EXTI_IRQn EXTI9_5_IRQn
#define GPS_PPS1_Pin GPIO_PIN_6
#define GPS_PPS1_GPIO_Port GPIOB
#define FPGA_RDY_Pin GPIO_PIN_7
#define FPGA_RDY_GPIO_Port GPIOB
#define FPGA_RDY_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
