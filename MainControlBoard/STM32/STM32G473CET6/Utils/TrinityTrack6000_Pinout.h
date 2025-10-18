/**
 * @file TrinityTrack6000_Pinout.h
 * @brief Pinout definitions for TrinityTrack6000 project.
 * This header file defines the pin mappings and configurations
 * for the TrinityTrack6000 hardware platform.
 * 
 * @author Alan Kudełko
 */

#ifndef TRINITY_TRACK6000_PINOUT_H_
    #define TRINITY_TRACK6000_PINOUT_H_

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

#endif // TRINITY_TRACK6000_PINOUT_H_