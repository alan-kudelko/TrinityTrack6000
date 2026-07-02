/**
 * 
 */
#include <stm32g4xx_hal.h>

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}
