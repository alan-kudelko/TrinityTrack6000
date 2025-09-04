#include <TrinityTrack6000_Init.h>


void initializeClock(void){
  	RCC_OscInitTypeDef RCC_OscInitStruct={0};
  	RCC_ClkInitTypeDef RCC_ClkInitStruct={0};

  	/** Configure the main internal regulator output voltage
  	*/
  	if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Initializes the RCC Oscillators according to the specified parameters
  	* in the RCC_OscInitTypeDef structure.
  	*/
  	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSI;
  	RCC_OscInitStruct.HSIState=RCC_HSI_ON;
  	RCC_OscInitStruct.HSICalibrationValue=RCC_HSICALIBRATION_DEFAULT;
  	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_ON;
  	RCC_OscInitStruct.PLL.PLLSource=RCC_PLLSOURCE_HSI;
  	RCC_OscInitStruct.PLL.PLLM=1;
  	RCC_OscInitStruct.PLL.PLLN=10;
  	RCC_OscInitStruct.PLL.PLLP=RCC_PLLP_DIV7;
  	RCC_OscInitStruct.PLL.PLLQ=RCC_PLLQ_DIV2;
  	RCC_OscInitStruct.PLL.PLLR=RCC_PLLR_DIV2;
  	if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!=HAL_OK){
    	Error_Handler();
  	}

  	/** Initializes the CPU, AHB and APB buses clocks
  	*/
  	RCC_ClkInitStruct.ClockType=RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  	RCC_ClkInitStruct.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
  	RCC_ClkInitStruct.AHBCLKDivider=RCC_SYSCLK_DIV1;
  	RCC_ClkInitStruct.APB1CLKDivider=RCC_HCLK_DIV1;
  	RCC_ClkInitStruct.APB2CLKDivider=RCC_HCLK_DIV1;

  	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_4)!=HAL_OK){
    	Error_Handler();
  	}	
}

void initializeHAL(void){
	HAL_Init();
}

void initializeGPIO(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
}

void initializeUART(void){
	uart.Instance=USART2;
	uart.Init.BaudRate=UART_BAUD_RATE;
	uart.Init.WordLength=UART_WORDLENGTH_8B;
	uart.Init.Parity=UART_PARITY_NONE;
	uart.Init.StopBits=UART_STOPBITS_1;
	uart.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	uart.Init.OverSampling=UART_OVERSAMPLING_16;
	uart.Init.Mode=UART_MODE_TX_RX;
	
	__HAL_RCC_USART2_CLK_ENABLE();

	if(HAL_UART_Init(&uart)!=HAL_OK){
		Error_Handler();
	}
}

void initializeSystem(void){
	initializeHAL();
	initializeClock();
	initializeGPIO();
	initializeUART();
}

void Error_Handler(void){
	__disable_irq();
	while(1){
		// Debug Loop
		// In the future restart of the system
	}
}