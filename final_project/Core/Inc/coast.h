/*
 * coast.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#ifndef INC_COAST_H_
#define INC_COAST_H_

#include "main.h"

/* LED_Data	*/
#define LCD_D_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D7_Pin GPIO_PIN_11
/* LED Enable */
#define LCD_E_Port GPIOD
#define LCD_E_Pin GPIO_PIN_2

#define LCD_RS_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_15

#define LCD_RW_Port GPIOC
#define LCD_RW_Pin GPIO_PIN_6

/* Keypad rows and cols */

#define KEY_ROW_Port GPIOB
#define KEY_ROW1_Pin GPIO_PIN_11
#define KEY_ROW2_Pin GPIO_PIN_12
#define KEY_ROW3_Pin GPIO_PIN_13
#define KEY_ROW4_Pin GPIO_PIN_14

#define KEY_COL_Port GPIOA
#define KEY_COL1_Pin GPIO_PIN_8
#define KEY_COL2_Pin GPIO_PIN_9
#define KEY_COL3_Pin GPIO_PIN_10
#define KEY_COL4_Pin GPIO_PIN_11


/* SW */
#define SW1_Port GPIOA
#define SW1_Pin GPIO_PIN_1
#define SW2_Port GPIOA
#define SW2_Pin GPIO_PIN_4
#define SW3_Port GPIOB
#define SW3_Pin GPIO_PIN_0
#define SW4_Port GPIOC
#define SW4_Pin GPIO_PIN_1

/* LED */
#define LED_Port GPIOB
#define LED1_Pin GPIO_PIN_10
#define LED2_Pin GPIO_PIN_4
#define LED3_Pin GPIO_PIN_5
#define LED4_Pin GPIO_PIN_3

/* time delay */
#define DELAY_1MS  1      // coast_asm_delay(1) ≈ 1ms
#define DELAY_50MS 50     // ≈ 50ms
#define DELAY_100MS 100   // ≈ 100ms
#define DELAY_500MS 500   // ≈ 500ms
#define DELAY_1000MS 1000 // ≈ 1000ms

//...

// call this at the end of MX_GPIO_Init() in main.c
void coast_gpio_init();

// call this inside while(1) in main.c
//void coast_loop_body();


#endif /* INC_COAST_H_ */
