/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define SFT_SRCLK_Pin GPIO_PIN_0
#define SFT_SRCLK_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LED_D1_Pin GPIO_PIN_10
#define LED_D1_GPIO_Port GPIOB
#define KEY_ROW1_Pin GPIO_PIN_11
#define KEY_ROW1_GPIO_Port GPIOB
#define KEY_ROW2_Pin GPIO_PIN_12
#define KEY_ROW2_GPIO_Port GPIOB
#define KEY_ROW3_Pin GPIO_PIN_13
#define KEY_ROW3_GPIO_Port GPIOB
#define KEY_ROW4_Pin GPIO_PIN_14
#define KEY_ROW4_GPIO_Port GPIOB
#define SFT_SER_Pin GPIO_PIN_15
#define SFT_SER_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_6
#define LCD_RW_GPIO_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D4_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D5_GPIO_Port GPIOC
#define KEY_COL1_Pin GPIO_PIN_8
#define KEY_COL1_GPIO_Port GPIOA
#define KEY_COL2_Pin GPIO_PIN_9
#define KEY_COL2_GPIO_Port GPIOA
#define KEY_COL3_Pin GPIO_PIN_10
#define KEY_COL3_GPIO_Port GPIOA
#define KEY_COL4_Pin GPIO_PIN_11
#define KEY_COL4_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_15
#define LCD_RS_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_11
#define LCD_D7_GPIO_Port GPIOC
#define LCD_E_Pin GPIO_PIN_2
#define LCD_E_GPIO_Port GPIOD
#define LED_D4_Pin GPIO_PIN_3
#define LED_D4_GPIO_Port GPIOB
#define LED_D2_Pin GPIO_PIN_4
#define LED_D2_GPIO_Port GPIOB
#define LED_D3_Pin GPIO_PIN_5
#define LED_D3_GPIO_Port GPIOB
#define SFT_RCLK_Pin GPIO_PIN_7
#define SFT_RCLK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
