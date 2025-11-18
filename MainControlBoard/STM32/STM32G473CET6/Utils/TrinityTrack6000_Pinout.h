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

#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOC
#define KILL_SWITCH_Pin GPIO_PIN_14
#define KILL_SWITCH_GPIO_Port GPIOC
#define NRF24L01_CE_Pin GPIO_PIN_15
#define NRF24L01_CE_GPIO_Port GPIOC
#define ARM_GUN_Pin GPIO_PIN_0
#define ARM_GUN_GPIO_Port GPIOF
#define FIRE_GUN_Pin GPIO_PIN_1
#define FIRE_GUN_GPIO_Port GPIOF
#define MQ6_IN_Pin GPIO_PIN_0
#define MQ6_IN_GPIO_Port GPIOA
#define MQ7_IN_Pin GPIO_PIN_1
#define MQ7_IN_GPIO_Port GPIOA
#define MQ_HEATERS_Pin GPIO_PIN_4
#define MQ_HEATERS_GPIO_Port GPIOA
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
#define WATCHDOG_FEED_Pin GPIO_PIN_4
#define WATCHDOG_FEED_GPIO_Port GPIOB
#define NC_Pin GPIO_PIN_5
#define NC_GPIO_Port GPIOB
#define GPS_RST_Pin GPIO_PIN_8
#define GPS_RST_GPIO_Port GPIOB
#define MCP_RST_Pin GPIO_PIN_9
#define MCP_RST_GPIO_Port GPIOB

#endif // TRINITY_TRACK6000_PINOUT_H_