/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define C_PORT GPIOA
#define R_PORT GPIOB
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t key_map[4][4] = {
		  {'1', '2', '3', 'A'},
		  {'4', '5', '6', 'B'},
		  {'7', '8', '9', 'C'},
		  {'*', '0', '#', 'D'}
};

bool is_done = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void LCD_putNibble(uint8_t data);
void LCD_enable();
void LCD_command(uint8_t command);
void LCD_data(uint8_t data);
void LCD_display(const char *str);
uint8_t KEY_scan(uint16_t *cols, uint16_t *rows);
bool syntax_check(uint8_t *value_arr, uint8_t counter);
uint32_t to_digit(uint8_t *value_arr, uint8_t counter);

extern void delay(uint32_t seconds);
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
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
  LCD_putNibble(0b0011);
  LCD_enable();
  LCD_putNibble(0b0011);
  LCD_enable();
  LCD_putNibble(0b0011);
  LCD_enable();
  LCD_putNibble(0b0010);
  LCD_enable();
  LCD_command(0b00101100);
  LCD_command(0b00001000);
  LCD_command(0b00000001);
  LCD_command(0b00000110);
  LCD_command(0b00001111);

  uint16_t col_pins[4] = { KEY_COL1_Pin, KEY_COL2_Pin, KEY_COL3_Pin, KEY_COL4_Pin };
  uint16_t row_pins[4] = { KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin, KEY_ROW4_Pin };
  uint8_t value_arr[17];
  uint8_t key_value = 0;
  uint8_t counter = 0;

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SFT_SRCLK_Pin|LCD_RW_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|KEY_COL1_Pin|KEY_COL2_Pin|KEY_COL3_Pin
                          |KEY_COL4_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_D1_Pin|SFT_SER_Pin|LED_D4_Pin|LED_D2_Pin
                          |LED_D3_Pin|SFT_RCLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SFT_SRCLK_Pin LCD_RW_Pin LCD_D4_Pin LCD_D5_Pin
                           LCD_D6_Pin LCD_D7_Pin */
  GPIO_InitStruct.Pin = SFT_SRCLK_Pin|LCD_RW_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin KEY_COL1_Pin KEY_COL2_Pin KEY_COL3_Pin
                           KEY_COL4_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|KEY_COL1_Pin|KEY_COL2_Pin|KEY_COL3_Pin
                          |KEY_COL4_Pin|LCD_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_D1_Pin SFT_SER_Pin LED_D4_Pin LED_D2_Pin
                           LED_D3_Pin SFT_RCLK_Pin */
  GPIO_InitStruct.Pin = LED_D1_Pin|SFT_SER_Pin|LED_D4_Pin|LED_D2_Pin
                          |LED_D3_Pin|SFT_RCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_ROW1_Pin KEY_ROW2_Pin KEY_ROW3_Pin KEY_ROW4_Pin */
  GPIO_InitStruct.Pin = KEY_ROW1_Pin|KEY_ROW2_Pin|KEY_ROW3_Pin|KEY_ROW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_E_Pin */
  GPIO_InitStruct.Pin = LCD_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_E_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == SW1_Pin) {
		HAL_TIM_Base_Stop_IT(&htim6);
		LCD_command(0b00000001);	// clear the LCD display

		HAL_GPIO_WritePin(SFT_SER_GPIO_Port, SFT_SER_Pin, GPIO_PIN_RESET);
		for (int i = 0; i < 16; i++) {
			HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_RESET);

		is_done = false;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(SFT_SER_GPIO_Port, SFT_SER_Pin);
	for (int i = 0; i < 16; i++) {
		HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SFT_SRCLK_GPIO_Port, SFT_SRCLK_Pin, GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SFT_RCLK_GPIO_Port, SFT_RCLK_Pin, GPIO_PIN_RESET);
}

uint32_t to_digit(uint8_t *value_arr, uint8_t counter) {
	uint32_t num = 0;
	for (int i = 0; i < counter - 1; i++) {
		num = num * 10 + (value_arr[i] - '0');
	}
	return num;
}

bool syntax_check(uint8_t *value_arr, uint8_t counter) {
	uint8_t temp;
	    for (int i = 0; i < counter; i++) {
	    	temp = value_arr[i];
	    	if (temp == 'A' || temp == 'B' || temp == 'C' || temp == 'D') {
	    		return true;
	    	}
	    }
	return false;
}

void LCD_putNibble(uint8_t data) {
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, data & 1);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data >> 1) & 1);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data >> 2) & 1);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data >> 3) & 1);
}

void LCD_enable() {
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, 1);
	int delayCounter = 10000;
	while(delayCounter--);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, 0);
	delayCounter = 10000;
	while(delayCounter--);
}

void LCD_command(uint8_t command) {
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0);
	LCD_putNibble(command >> 4);
	LCD_enable();
	LCD_putNibble(command & 0x0f);
	LCD_enable();
}

void LCD_data(uint8_t data) {
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 1);
	int delayCounter = 10000;
	while(delayCounter--);
	LCD_putNibble(data >> 4);
	LCD_enable();
	LCD_putNibble(data & 0x0f);
	LCD_enable();
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, 0);
}

void LCD_display(const char *str) {

	int c = 0;
	while(*str) {
		if (c == 16) {
			LCD_command(0b11000000);
		}
		LCD_data(*str++);
		c++;
	}

}

uint8_t KEY_scan(uint16_t *cols, uint16_t *rows) {
	uint8_t key_value = 0;
	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(C_PORT, cols[i], 1);
		for (int j = 0; j < 4; j++) {
			if (HAL_GPIO_ReadPin(R_PORT, rows[j])) {
				key_value = key_map[j][i];
				break;
			}
		}
		HAL_GPIO_WritePin(C_PORT, cols[i], 0);
		if (key_value != 0) {
			break;
		}
	}
	return key_value;
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
