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
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
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

	HAL_GPIO_WritePin(LCD_D_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D_Port, LCD_D7_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_D_Port, &GPIO_InitStruct);


	HAL_GPIO_WritePin(LCD_E_Port, LCD_E_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_E_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_E_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RS_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LCD_RW_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD_RW_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_RW_Port, &GPIO_InitStruct);

}

void coast_gpio_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
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
		GPIO_InitStruct.Pin = SW1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SW1_Port, &GPIO_InitStruct);

	coast_gpio_lcd_init();
	coast_gpio_keypad_init();

}

void coast_loop_body(){

	LCD_SendStr("Hello");
	LCD_SendCmd(LCD_SECOND_LINE);
	LCD_SendStr("World");
	while(1){
		uint8_t c = scan_keypad();
		if (HAL_GPIO_ReadPin(SW1_Port, SW1_Pin) == 1) {
			LCD_SendCmd(LCD_CLEAR_DISPLAY);
			c = 0;
		}
		if(c){
			HAL_GPIO_WritePin(LED_Port, LED1_Pin, 1);
			LCD_SendData(c);
		}
	}

}
