
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void homePage() {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("FITNESS ");

	char buff[16];

	int temp = 21; // placeholder
	sprintf(buff, "%dC ", temp);
	LCD_SendStr(buff);

	LCD_SendStr(" 57%");

	LCD_SendCmd(LCD_SECOND_LINE); // Move to second line

	LCD_SendStr("OPT ");
}

void flashlightPage(bool enable_flash) {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	char buff[16];
	LCD_SendStr("FLASHLIGHT:");
	if (enable_flash) {
		LCD_SendStr("ON");
		flash_on();
	} else {
		LCD_SendStr("OFF");
		flash_off();
	}

	LCD_SendStr(LCD_SECOND_LINE);
	LCD_SendStr("SW1:Toggle");
}

//void flash_on() {
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//
//	HAL_GPIO_WritePin(Ser_Data_IN_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_SET);
//	for (int i = 0; i < 16; i++) {
//		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_RESET);
//	}
//	HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_RESET);
//}
//
//void flash_off() {
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//	HAL_GPIO_WritePin(Ser_Data_IN_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_RESET);
//	for (int i = 0; i < 16; i++) {
//		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SER_Data_IN_Pin, GPIO_PIN_RESET);
//	}
//	HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_RESET);
//}
