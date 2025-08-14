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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define SRCLK_Pin GPIO_PIN_0
#define SRCLK_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_1
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI1_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_4
#define SW2_GPIO_Port GPIOA
#define SW2_EXTI_IRQn EXTI4_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define COILA_Pin GPIO_PIN_5
#define COILA_GPIO_Port GPIOC
#define SW3_Pin GPIO_PIN_0
#define SW3_GPIO_Port GPIOB
#define SW3_EXTI_IRQn EXTI0_IRQn
#define COILD_Pin GPIO_PIN_2
#define COILD_GPIO_Port GPIOB
#define LED_D1_Pin GPIO_PIN_10
#define LED_D1_GPIO_Port GPIOB
#define SER_Data_IN_Pin GPIO_PIN_15
#define SER_Data_IN_GPIO_Port GPIOB
#define Control_RW_Pin GPIO_PIN_6
#define Control_RW_GPIO_Port GPIOC
#define COILC_Pin GPIO_PIN_7
#define COILC_GPIO_Port GPIOC
#define Data_D4_Pin GPIO_PIN_8
#define Data_D4_GPIO_Port GPIOC
#define Data_D5_Pin GPIO_PIN_9
#define Data_D5_GPIO_Port GPIOC
#define COILB_Pin GPIO_PIN_12
#define COILB_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Control_RS_Pin GPIO_PIN_15
#define Control_RS_GPIO_Port GPIOA
#define Data_D6_Pin GPIO_PIN_10
#define Data_D6_GPIO_Port GPIOC
#define Data_D7_Pin GPIO_PIN_11
#define Data_D7_GPIO_Port GPIOC
#define Control_E_Pin GPIO_PIN_2
#define Control_E_GPIO_Port GPIOD
#define LED_D4_Pin GPIO_PIN_3
#define LED_D4_GPIO_Port GPIOB
#define LED_D2_Pin GPIO_PIN_4
#define LED_D2_GPIO_Port GPIOB
#define LED_D3_Pin GPIO_PIN_5
#define LED_D3_GPIO_Port GPIOB
#define RCLK_Latch_Pin GPIO_PIN_7
#define RCLK_Latch_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define LCD_RW_Pin GPIO_PIN_6
#define LCD_RW_Port GPIOC

#define LCD_Data_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D7_Pin GPIO_PIN_11

#define LCD_RS_Pin GPIO_PIN_15
#define LCD_RS_Port GPIOA

#define LCD_E_Pin GPIO_PIN_2
#define LCD_E_Port GPIOD

#define LED_D4_Pin GPIO_PIN_3
#define LED_D4_GPIO_Port GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
