/*
 * coast.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include "main.h"
#include "coast.h"
#include "lcd.h"
#include "keypad.h"

void coast_gpio_keypad_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* keypad row pins initialize */
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW4_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = KEY_ROW1_Pin|KEY_ROW2_Pin|KEY_ROW3_Pin|KEY_ROW4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY_ROW_Port, &GPIO_InitStruct);

	/* keypad col pins initialize */
	HAL_GPIO_WritePin(KEY_COL_Port, KEY_COL1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_COL_Port, KEY_COL2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_COL_Port, KEY_COL3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_COL_Port, KEY_COL4_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = KEY_COL1_Pin|KEY_COL2_Pin|KEY_COL3_Pin|KEY_COL4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(KEY_COL_Port, &GPIO_InitStruct);
}

void coast_gpio_lcd_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* initialize the lcd D*/
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D7_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D_Port, &GPIO_InitStruct);

	/* initialize the lcd E*/
	HAL_GPIO_WritePin(LCD_E_Port, LCD_E_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_E_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_E_Port, &GPIO_InitStruct);

	/* initialize the lcd RS*/
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RS_Port, &GPIO_InitStruct);

	/* initialize the lcd RW*/
	HAL_GPIO_WritePin(LCD_RW_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RW_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RW_Port, &GPIO_InitStruct);

	/* initialize the lcd Display*/
	coast_lcd_init();

}

void coast_gpio_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* SW and LED initialize */
	HAL_GPIO_WritePin(LED_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_Port, LED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_Port, LED3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_Port, LED4_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(SW1_Port, SW1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW2_Port, SW2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW3_Port, SW3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW4_Port, SW4_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin|SW3_Pin|SW4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SW1_Port, &GPIO_InitStruct);
	HAL_GPIO_Init(SW2_Port, &GPIO_InitStruct);
	HAL_GPIO_Init(SW3_Port, &GPIO_InitStruct);
	HAL_GPIO_Init(SW4_Port, &GPIO_InitStruct);

	/* clocks initialize */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	coast_gpio_lcd_init();
	coast_gpio_keypad_init();

}

