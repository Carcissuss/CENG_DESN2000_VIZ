/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "coast.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
uint32_t seconds = 0;
uint32_t period_count = 0;
uint32_t decimal_second_count = 0;
uint32_t button_double_press_time[4];
uint32_t button_holding_time[4];

/* button press check */
bool button1 = false;
bool button2 = false;
bool button3 = false;
bool buttonB = false;

/* double press interval and holding time */
uint32_t double_press_interval = 10;
uint32_t holding_bound = 15;
/* press check */
bool is_first_press[4];
bool is_double_press[4];
bool is_holding[4];

/* sound setting */
bool enable_sound = true;
bool enable_vibration = true;

/* screen navigation */
typedef struct {
	bool screen_homepage;
	bool sceen_countdown;
	bool screen_stopwatch;
	bool screen_alarm;
	bool screen_setting_1;
	bool screen_setting_2;
}Navigation;

Navigation screen = {
    .screen_homepage = true,
    .sceen_countdown = false,
    .screen_stopwatch = false,
    .screen_alarm = false,
    .screen_setting_1 = false,
    .screen_setting_2 = false
};

/*  */

/* Private functions ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */
extern void coast_asm_delay(uint32_t milliseconds);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);

  for (int i =0 ; i < 4; i++) {
	  button_double_press_time[i] = 0;
	  button_holding_time[i] = 0;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 296;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 198;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 9999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 7199;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 71;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin SW2_Pin */
  GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SW3_Pin */
  GPIO_InitStruct.Pin = SW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
	coast_gpio_init();
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == B1_Pin) {
		/* B1 is pressed */
		if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1) {
			/* sound indication */
			if (sound_enable) {
				generate_sound(300,50,&htim1);
			}
			if (is_first_press[0] == true && is_double_press[0] == false &&
				(decimal_second_count - button_double_press_time[0]) <= double_press_interval) {
				is_double_press[0] = true;
				is_holding[0] = false;
				is_single_press[0] = false;
			} else {
				is_single_press[0] = true;
				is_holding[0] = false
				is_double_press[0] = false;
			}
			button_holding_time[0] = decimal_second_count;
		}
		/* B1 is released */
		else {
			stop_sound(&htim1);
			if ((decimal_second_count - button_holding_time[0]) >= holding_bound) {
				is_holding[0] = true;
				is_double_press[0] = false;
				is_single_press[0] = false;
			} else {
				button_double_press_time[0] = decimal_second_count;
			}
		}
	} else if (GPIO_Pin == SW1_Pin) {
		/* The sw1 pin is pressed */
		if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 1) {
			/* sound indication */
			if (sound_enable) {
				generate_sound(300,50,&htim1);
			}
			if (is_first_press[1] == true && is_double_press[1] == false &&
				(decimal_second_count - button_double_press_time[1]) <= double_press_interval) {
				is_double_press[1] = true;
				is_single_press[1] = false;
			} else {
				is_single_press[1] = true;
				is_double_press[1] = false;
			}
			button_holding_time[1] = decimal_second_count;

		}
		/* The sw1 pin is released */
		else {
			stop_sound(&htim1);
			if ((decimal_second_count - button_holding_time[1]) >= holding_bound) {
				is_holding[1] = true;
				button_double_press_time[0] = 0;
				is_single_press[1] = false;
			} else {
				button_double_press_time[1] = decimal_second_count;
			}
		}
	} else if (GPIO_Pin == SW2_Pin) {
		/* The sw2 pin is pressed */
		if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 1) {
			/* sound indication */
			if (sound_enable) {
				generate_sound(300,50,&htim1);
			}
			if (is_first_press[2] == true && is_double_press[2] == false &&
				( decimal_second_count - button_double_press_counting[2]) <= double_press_interval) {
				is_single_press[2] = false;
				is_double_press[2] = true;
			} else {
				is_single_press[2] = true;
				is_double_press[2] = false;
			}
			/* start counting the holding and double press */
			button_holding_time[2] = decimal_second_count;
		}

		/* The sw2 pin is released */
		else {
			if ((decimal_second_count - button_holding_time[2]) >= holding_bound) {
				is_holding[2] = true;
				button_double_press_counting[2] = 0;
			} else {
				button_double_press_counting[2] = decimal_second_count;
			}

		}
	} else {
		/* The sw3 pin is pressed */
		if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 1) {
			/* sound indication */
			if (sound_enable) {
				generate_sound(300,50,&htim1);
			}
			if (is_first_press[3] == true && is_double_press[3] == false &&
				( decimal_second_count - button_double_press_counting[2]) <= double_press_interval) {
				is_single_press[3] = false;
				is_double_press[3] = true;
			} else {
				is_single_press[3] = true;
				is_double_press[3] = false;
				/* only start counting when first_press is done and reset the double count */
			}
			button_holding_time[3] = decimal_second_count;
		}
		/* The sw3 pin is released */
		else {
			stop_sound(&htim1);
			if ((decimal_second_count - button_holding_time[3]) >= holding_bound) {
				is_holding[3] = true;
				button_double_press_counting[3] = 0;
			} else {
				button_double_press_counting[3] = decimal_second_count;
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim6) {
		/* increment 1 second to count the time for entire project */
		seconds++;
	} else if (htim == &htim7) {
		/* increment 1 millisecond to control the brightness of 16 serial LEDS */
		if (period_count >= 100) {
			decimal_second_count++; // every 100 millisecond or 0.1 second increment 1;
			period_count = 1;
		} else {
			period_count++;
		}

		if (period_count == duty_cycle) {
			HAL_GPIO_WritePin(SFT_SER_GPIO_Port, SFT_SER_Pin, GPIO_PIN_RESET);
			for (int i = 0; i < 16; i++) {
				HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_RESET);
		} else if (period_count == 0) {
			HAL_GPIO_WritePin(SFT_SER_GPIO_Port, SFT_SER_Pin, GPIO_PIN_SET);
			for (int i = 0; i < 16; i++) {
				HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_RESET);
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
