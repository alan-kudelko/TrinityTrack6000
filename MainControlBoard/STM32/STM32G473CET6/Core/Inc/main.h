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
#define GPIO___BUZZER_Pin GPIO_PIN_13
#define GPIO___BUZZER_GPIO_Port GPIOC
#define QUARTZ_32kHz_Pin GPIO_PIN_14
#define QUARTZ_32kHz_GPIO_Port GPIOC
#define QUARTZ_32kHzC15_Pin GPIO_PIN_15
#define QUARTZ_32kHzC15_GPIO_Port GPIOC
#define GPIO___ARM_BB_Gun_Pin GPIO_PIN_0
#define GPIO___ARM_BB_Gun_GPIO_Port GPIOF
#define GPIO___Fire_BB_Gun_Pin GPIO_PIN_1
#define GPIO___Fire_BB_Gun_GPIO_Port GPIOF
#define GPIO___MQ_HEATERS_Pin GPIO_PIN_4
#define GPIO___MQ_HEATERS_GPIO_Port GPIOA
#define NRF24L01_CS_Pin GPIO_PIN_0
#define NRF24L01_CS_GPIO_Port GPIOB
#define FRAM_CS_Pin GPIO_PIN_1
#define FRAM_CS_GPIO_Port GPIOB
#define MCP_CS_Pin GPIO_PIN_2
#define MCP_CS_GPIO_Port GPIOB
#define INFINEON_RST_Pin GPIO_PIN_10
#define INFINEON_RST_GPIO_Port GPIOB
#define RENESANS_CS_Pin GPIO_PIN_11
#define RENESANS_CS_GPIO_Port GPIOB
#define INFINEON_CS_Pin GPIO_PIN_12
#define INFINEON_CS_GPIO_Port GPIOB
#define NXP_CS_Pin GPIO_PIN_10
#define NXP_CS_GPIO_Port GPIOA
#define FPGA_CS_Pin GPIO_PIN_11
#define FPGA_CS_GPIO_Port GPIOA
#define NXP_RST_Pin GPIO_PIN_12
#define NXP_RST_GPIO_Port GPIOA
#define RENESANS_RST_Pin GPIO_PIN_15
#define RENESANS_RST_GPIO_Port GPIOA
#define FPGA_RST_Pin GPIO_PIN_3
#define FPGA_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
