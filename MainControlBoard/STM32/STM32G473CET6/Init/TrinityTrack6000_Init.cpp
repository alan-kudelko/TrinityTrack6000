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
#include <USART1_Dma.h>

#include <tasks.h>

extern "C" void ramInfoInit(void);

const char msg_init_mcu_initialized_info[]         ="[SYS][0x00][  OK  ] System Init: Core + Peripherals Ready\r\n";
const char msg_init_GPIO_initialized_info[]        ="[SYS][0x01][  OK  ] GPIO initialized with default configuration\r\n";
const char msg_init_memory_initialized_info[]      ="[SYS][0x02][  OK  ] Memory ready\r\n";
const char msg_init_NRF_initialized_format_string[]="[SYS][0x03][%6s] Communication with NRF24L01\r\n";
const char msg_init_MCP_initialized_format_string[]="[SYS][0x03][%6s] Communication with MCP23S17\r\n";

const char msg_init_status_ok[]="OK";
const char msg_init_status_nok[]="FAILED";

//const char msg_init_

extern "C" void usart1_dma_init(void);

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
  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */
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
  	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  	hspi1.Init.NSS = SPI_NSS_SOFT;
  	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
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
  	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
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

void MX_TIM3_Init(void){
  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 169;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  __HAL_TIM_ENABLE_OCxPRELOAD(&htim3, TIM_CHANNEL_2); // Modified
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);
}

void MX_TIM2_Init(void){
  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 169;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);
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

void MX_DMA_Init(void){
  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
  /* DMA2_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
  /* DMA2_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
  /* DMA2_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
  /* DMA1_Channel8_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel8_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel8_IRQn);
}

void MX_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, KILL_SWITCH_Pin|NRF24L01_CE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, ARM_GUN_Pin|FIRE_GUN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, NRF24L01_CS_Pin|FRAM_CS_Pin|MCP_CS_Pin|INFINEON_RST_Pin
                          |RENESANS_CS_Pin|INFINEON_CS_Pin|FPGA_RST_Pin|WATCHDOG_FEED_Pin
                          |NC_Pin|GPS_RST_Pin|MCP_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, NXP_CS_Pin|FPGA_CS_Pin|NXP_RST_Pin|RENESANS_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : KILL_SWITCH_Pin NRF24L01_CE_Pin */
  GPIO_InitStruct.Pin = KILL_SWITCH_Pin|NRF24L01_CE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ARM_GUN_Pin FIRE_GUN_Pin */
  GPIO_InitStruct.Pin = ARM_GUN_Pin|FIRE_GUN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : NRF24L01_CS_Pin FRAM_CS_Pin MCP_CS_Pin INFINEON_RST_Pin
                           RENESANS_CS_Pin INFINEON_CS_Pin FPGA_RST_Pin WATCHDOG_FEED_Pin
                           NC_Pin GPS_RST_Pin MCP_RST_Pin */
  GPIO_InitStruct.Pin = NRF24L01_CS_Pin|FRAM_CS_Pin|MCP_CS_Pin|INFINEON_RST_Pin
                          |RENESANS_CS_Pin|INFINEON_CS_Pin|FPGA_RST_Pin|WATCHDOG_FEED_Pin
                          |NC_Pin|GPS_RST_Pin|MCP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : NXP_CS_Pin FPGA_CS_Pin NXP_RST_Pin RENESANS_RST_Pin */
  GPIO_InitStruct.Pin = NXP_CS_Pin|FPGA_CS_Pin|NXP_RST_Pin|RENESANS_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void initializeThreadXMemory(void){
	// Allocate memory for ThreadX objects from statically allocated pool
	// to avoid fragmentation and issues with dynamic memory allocation
	// in embedded systems.
	
	// Clear all stacks
	task_CLI_init();
	
}

void initializeMemory(void){
// Initialize RAM info variables
	ramInfoInit();
// DMA memory zeroing and testing could be added here
	usart1_dma_init();
	//usart2_dma_init();
// TheadX memory allocation etc.
	initializeThreadXMemory();
}

void initializeSystem(){
// STM32CubeIDE generated initialization sequence
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init(); // Set default POR state
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_CORDIC_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	__HAL_DMA_DISABLE_IT(&hdma_adc1,DMA_IT_HT);
	__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_HT3);

	while(usart1_dma_enq_data((uint8_t*)msg_init_mcu_initialized_info,strlen(msg_init_mcu_initialized_info))!=true){
		HAL_Delay(10);
	}
	while(usart1_dma_enq_data((uint8_t*)msg_init_GPIO_initialized_info,strlen(msg_init_GPIO_initialized_info))!=true){
		HAL_Delay(10);
	}
// Custom initialization sequence
	initializeMemory();
	while(usart1_dma_enq_data((uint8_t*)msg_init_memory_initialized_info,strlen(msg_init_memory_initialized_info))!=true){
		HAL_Delay(10);
	}
	// Initialize USART1 DMA for diagnostics interface
	usart1_dma_rx_init();
	//char buffer[INIT_LINE_BUFFER_SIZE]={0};
	//buffer[0]='1';

	//initializeMCP();
	//snprintf(buffer,);
	//HAL_UART_Transmit(&huart1,(uint8_t*)buffer,strlen(buffer),DEBUG_UART_TIMEOUT);
}