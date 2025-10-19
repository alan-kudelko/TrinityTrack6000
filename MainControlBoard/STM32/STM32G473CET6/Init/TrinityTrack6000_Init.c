/**
 * @file TrinityTrack6000_Init.c
 * @brief Implementation of TrinityTrack6000_Init.h
 * Initialization of HAL, Clock, GPIO, UART and Memory Diagnostics.
 * 
 * @note Some of the initialization functions are named respectively to
 * default STM32CubeMX generated functions to maintain consistency along
 * with their generated comments.
 *  
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <string.h>

#include <TrinityTrack6000_Init.h>
#include <TrinityTrack6000_Errors.h>
#include <TrinityTrack6000_Pinout.h>

extern void ramInfoInit(void);

const char msg_init_mcu_initialized_info[]         ="[SYS][0x00][  OK  ] System Init: Core + Peripherals Ready\r\n";
const char msg_init_GPIO_initialized_info[]        ="[SYS][0x01][  OK  ] GPIO initialized with default configuration\r\n";
const char msg_init_memory_initialized_info[]      ="[SYS][0x02][  OK  ] Memory ready\r\n";
const char msg_init_NRF_initialized_format_string[]="[SYS][0x03][%6s] Communication with NRF24L01\r\n";
const char msg_init_MCP_initialized_format_string[]="[SYS][0x03][%6s] Communication with MCP23S17\r\n";

const char msg_init_status_ok[]="OK";
const char msg_init_status_nok[]="FAILED";

//const char msg_init_

void SystemClock_Config(void){
  	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  	/** Configure the main internal regulator output voltage
  	*/
  	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  	/** Initializes the RCC Oscillators according to the specified parameters
  	* in the RCC_OscInitTypeDef structure.
  	*/
  	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  	RCC_OscInitStruct.PLL.PLLN = 85;
  	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Initializes the CPU, AHB and APB buses clocks
  	*/
  	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_4)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_ADC1_Init(void){
  	ADC_MultiModeTypeDef multimode = {0};
  	ADC_ChannelConfTypeDef sConfig = {0};

  	hadc1.Instance = ADC1;
  	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  	hadc1.Init.GainCompensation = 0;
  	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  	hadc1.Init.LowPowerAutoWait = DISABLE;
  	hadc1.Init.ContinuousConvMode = DISABLE;
  	hadc1.Init.NbrOfConversion = 1;
  	hadc1.Init.DiscontinuousConvMode = DISABLE;
  	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  	hadc1.Init.DMAContinuousRequests = DISABLE;
  	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  	hadc1.Init.OversamplingMode = DISABLE;

  	if(HAL_ADC_Init(&hadc1)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Configure the ADC multi-mode
  	*/
  	multimode.Mode = ADC_MODE_INDEPENDENT;
  	if(HAL_ADCEx_MultiModeConfigChannel(&hadc1,&multimode)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Configure Regular Channel
  	*/
  	sConfig.Channel = ADC_CHANNEL_1;
  	sConfig.Rank = ADC_REGULAR_RANK_1;
  	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  	sConfig.SingleDiff = ADC_SINGLE_ENDED;
  	sConfig.OffsetNumber = ADC_OFFSET_NONE;
  	sConfig.Offset = 0;
  	if(HAL_ADC_ConfigChannel(&hadc1,&sConfig)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_CORDIC_Init(void){
  	hcordic.Instance = CORDIC;
  	if(HAL_CORDIC_Init(&hcordic)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_I2C2_Init(void){
  	hi2c2.Instance = I2C2;
  	hi2c2.Init.Timing = 0x40621236;
  	hi2c2.Init.OwnAddress1 = 0;
  	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  	hi2c2.Init.OwnAddress2 = 0;
  	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  	if(HAL_I2C_Init(&hi2c2)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Configure Analogue filter
  	*/
  	if(HAL_I2CEx_ConfigAnalogFilter(&hi2c2,I2C_ANALOGFILTER_ENABLE)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Configure Digital filter
  	*/
  	if(HAL_I2CEx_ConfigDigitalFilter(&hi2c2,0)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_SPI1_Init(void){
  /* SPI1 parameter configuration*/
  	hspi1.Instance = SPI1;
  	hspi1.Init.Mode = SPI_MODE_MASTER;
  	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  	hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  	hspi1.Init.NSS = SPI_NSS_SOFT;
  	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  	hspi1.Init.CRCPolynomial = 7;
  	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  	if(HAL_SPI_Init(&hspi1)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_SPI2_Init(void){
  	/* SPI2 parameter configuration*/
  	hspi2.Instance = SPI2;
  	hspi2.Init.Mode = SPI_MODE_MASTER;
  	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  	hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  	hspi2.Init.NSS = SPI_NSS_SOFT;
  	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  	hspi2.Init.CRCPolynomial = 7;
  	hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  	hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  	if(HAL_SPI_Init(&hspi2)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_TIM8_Init(void){
  	TIM_MasterConfigTypeDef sMasterConfig = {0};
  	TIM_OC_InitTypeDef sConfigOC = {0};
  	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  	htim8.Instance = TIM8;
  	htim8.Init.Prescaler = 0;
  	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  	htim8.Init.Period = 65535;
  	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  	htim8.Init.RepetitionCounter = 0;
  	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  	if(HAL_TIM_PWM_Init(&htim8)!=HAL_OK){
    	Error_Handler();
  	}
  	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  	if(HAL_TIMEx_MasterConfigSynchronization(&htim8,&sMasterConfig)!=HAL_OK){
    	Error_Handler();
  	}
  	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  	sConfigOC.Pulse = 0;
  	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  	if(HAL_TIM_PWM_ConfigChannel(&htim8,&sConfigOC,TIM_CHANNEL_4)!=HAL_OK){
    	Error_Handler();
  	}
  	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  	sBreakDeadTimeConfig.DeadTime = 0;
  	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  	sBreakDeadTimeConfig.BreakFilter = 0;
  	sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  	sBreakDeadTimeConfig.Break2Filter = 0;
  	sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  	if(HAL_TIMEx_ConfigBreakDeadTime(&htim8,&sBreakDeadTimeConfig)!=HAL_OK){
    	Error_Handler();
  	}

  	HAL_TIM_MspPostInit(&htim8);
}

void MX_USART1_UART_Init(void){
  	huart1.Instance = USART1;
  	huart1.Init.BaudRate = USART1_BAUD_RATE;
  	huart1.Init.WordLength = UART_WORDLENGTH_8B;
  	huart1.Init.StopBits = UART_STOPBITS_1;
  	huart1.Init.Parity = UART_PARITY_NONE;
  	huart1.Init.Mode = UART_MODE_TX_RX;
  	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  	if(HAL_UART_Init(&huart1)!=HAL_OK){
    	Error_Handler();
  	}
  	if(HAL_UARTEx_SetTxFifoThreshold(&huart1,UART_TXFIFO_THRESHOLD_1_8)!=HAL_OK){
    	Error_Handler();
  	}
  	if(HAL_UARTEx_SetRxFifoThreshold(&huart1,UART_RXFIFO_THRESHOLD_1_8)!=HAL_OK){
    	Error_Handler();
  	}
  	if(HAL_UARTEx_DisableFifoMode(&huart1)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_USART2_UART_Init(void){
  	huart2.Instance = USART2;
  	huart2.Init.BaudRate = USART2_BAUD_RATE;
  	huart2.Init.WordLength = UART_WORDLENGTH_8B;
  	huart2.Init.StopBits = UART_STOPBITS_1;
  	huart2.Init.Parity = UART_PARITY_NONE;
  	huart2.Init.Mode = UART_MODE_TX_RX;
  	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  	if(HAL_UART_Init(&huart2)!=HAL_OK){
    	Error_Handler();
  	}
  	if(HAL_UARTEx_SetTxFifoThreshold(&huart2,UART_TXFIFO_THRESHOLD_1_8)!=HAL_OK){
    	Error_Handler();
  	}
  	if (HAL_UARTEx_SetRxFifoThreshold(&huart2,UART_RXFIFO_THRESHOLD_1_8)!=HAL_OK){
    	Error_Handler();
  	}
  	if (HAL_UARTEx_DisableFifoMode(&huart2)!=HAL_OK){
    	Error_Handler();
  	}
}

void MX_GPIO_Init(void){
  	GPIO_InitTypeDef GPIO_InitStruct = {0};
  	/* GPIO Ports Clock Enable */
  	__HAL_RCC_GPIOC_CLK_ENABLE();
  	__HAL_RCC_GPIOF_CLK_ENABLE();
  	__HAL_RCC_GPIOA_CLK_ENABLE();
  	__HAL_RCC_GPIOB_CLK_ENABLE();

  	/*Configure GPIO pin Output Level */
  	HAL_GPIO_WritePin(GPIOF, GPIO___ARM_BB_Gun_Pin|GPIO___Fire_BB_Gun_Pin, GPIO_PIN_RESET);

  	/*Configure GPIO pin Output Level */
  	HAL_GPIO_WritePin(GPIOA, GPIO___MQ_HEATERS_Pin|NXP_CS_Pin|FPGA_CS_Pin|NXP_RST_Pin
                          |RENESANS_RST_Pin, GPIO_PIN_RESET);

  	/*Configure GPIO pin Output Level */
  	HAL_GPIO_WritePin(GPIOB, NRF24L01_CS_Pin|FRAM_CS_Pin|MCP_CS_Pin|INFINEON_RST_Pin
                          |RENESANS_CS_Pin|INFINEON_CS_Pin|FPGA_RST_Pin, GPIO_PIN_RESET);

  	/*Configure GPIO pins : GPIO___ARM_BB_Gun_Pin GPIO___Fire_BB_Gun_Pin */
  	GPIO_InitStruct.Pin = GPIO___ARM_BB_Gun_Pin|GPIO___Fire_BB_Gun_Pin;
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  	/*Configure GPIO pins : GPIO___MQ_HEATERS_Pin NXP_CS_Pin FPGA_CS_Pin NXP_RST_Pin
                           RENESANS_RST_Pin */
  	GPIO_InitStruct.Pin = GPIO___MQ_HEATERS_Pin|NXP_CS_Pin|FPGA_CS_Pin|NXP_RST_Pin
                          |RENESANS_RST_Pin;
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : NRF24L01_CS_Pin FRAM_CS_Pin MCP_CS_Pin INFINEON_RST_Pin
                           RENESANS_CS_Pin INFINEON_CS_Pin FPGA_RST_Pin */
  	GPIO_InitStruct.Pin = NRF24L01_CS_Pin|FRAM_CS_Pin|MCP_CS_Pin|INFINEON_RST_Pin
                          |RENESANS_CS_Pin|INFINEON_CS_Pin|FPGA_RST_Pin;
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void initializeMemory(void){
	ramInfoInit();
// TheadX memory allocation etc.
}

void initializeSystem(){
// STM32CubeIDE generated initialization sequence
	HAL_Init();
	SystemClock_Config();
	MX_CORDIC_Init();
	MX_TIM8_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	HAL_UART_Transmit(&huart1,(uint8_t*)msg_init_mcu_initialized_info,strlen(msg_init_mcu_initialized_info),DEBUG_UART_TIMEOUT);
	
	MX_GPIO_Init(); // Set default POR state
	HAL_UART_Transmit(&huart1,(uint8_t*)msg_init_GPIO_initialized_info,strlen(msg_init_GPIO_initialized_info),DEBUG_UART_TIMEOUT);
// Custom initialization sequence
	initializeMemory();
	HAL_UART_Transmit(&huart1,(uint8_t*)msg_init_memory_initialized_info,strlen(msg_init_memory_initialized_info),DEBUG_UART_TIMEOUT);

	//char buffer[INIT_LINE_BUFFER_SIZE]={0};
	//buffer[0]='1';

	//initializeMCP();
	//snprintf(buffer,);
	//HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
}
