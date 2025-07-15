#include "main.h"
#include "coast.h"
#include "lcd.h"
#include <stdio.h>
void coast_gpio_lcd_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	// initialize the LCD_DB
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D7_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D_Port, &GPIO_InitStruct);

	// initialize the RW
	HAL_GPIO_WritePin(LCD_RW_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RW_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RW_Port, &GPIO_InitStruct);

	// initialize the RS
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RS_Port, &GPIO_InitStruct);

	// initialize the E
	HAL_GPIO_WritePin(LCD_E_Port, LCD_E_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_E_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_E_Port, &GPIO_InitStruct);
}

void coast_gpio_init(){
	__HAL_RCC_GPIOD_CLK_ENABLE();

	coast_gpio_lcd_init();
	coast_lcd_init();
}
void coast_loop_body(int16_t counter) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%d", counter);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr(buffer);
}
