#include "main.h"
#include "coast.h"
#include "lcd.h"
#include "keypad.h"

void coast_gpio_keypad_init(){
	// keypad row pins initialize
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(KEY_ROW_Port, KEY_ROW4_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = KEY_ROW1_Pin|KEY_ROW2_Pin|KEY_ROW3_Pin|KEY_ROW4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(KEY_ROW_Port, &GPIO_InitStruct);

	// keypad col pins initialize
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
	coast_gpio_keypad_init();
	coast_lcd_init();
}

void coast_loop_body(){

//	LCD_SendStr("Hello");
//	LCD_SendCmd(LCD_SECOND_LINE);
//	LCD_SendStr("World");
//	LCD_SendCmd(0b00001100);

	while(1){
		uint8_t c = scan_keypad();
		if(c){
			LCD_SendData(c);
		}
	}

}
