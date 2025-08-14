/*
 * options.c
 *
 *  Created on: Aug 13, 2025
 *      Author: Ibrahim
 */

#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

extern bool enable_sound;
extern bool enable_vibration;
extern volatile bool flash;

void settingsPage() {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);

	LCD_SendStr("SOUND:");
	LCD_SendStr(enable_sound ? "ON " : "OFF");


	LCD_SendCmd(LCD_SECOND_LINE); // Move to second line

	LCD_SendStr("HAPT:");
	LCD_SendStr(enable_vibration ? "ON" : "OF");

	LCD_SendStr("|FLASH:");
	LCD_SendStr(flash ? "ON" : "OFF");
}


/*
 * Flashlight feature control
 *
 * */

/* switch D5–D20 */
static void switchLedBar(uint16_t value) {
    for (int i = 15; i >= 0; --i) {
        GPIO_PinState bit = (value & (1U << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(SER_Data_IN_GPIO_Port, SER_Data_IN_Pin, bit);
        HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_RESET);
}

void flash_on(void) {
    /* Direct LEDs D1–D4 + LD2 */
    HAL_GPIO_WritePin(LED_D1_GPIO_Port,   LED_D1_Pin,   GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_D2_GPIO_Port,   LED_D2_Pin,   GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_D3_GPIO_Port,   LED_D3_Pin,   GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_D2B4_GPIO_Port, LED_D2B4_Pin, GPIO_PIN_SET); // D4 on this board
    HAL_GPIO_WritePin(LD2_GPIO_Port,      LD2_Pin,      GPIO_PIN_SET);

    // led bar (D5–D20) all ON
    switchLedBar(0xFFFF);
}

void flash_off(void) {
    HAL_GPIO_WritePin(LED_D1_GPIO_Port,   LED_D1_Pin,   GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D2_GPIO_Port,   LED_D2_Pin,   GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D3_GPIO_Port,   LED_D3_Pin,   GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D2B4_GPIO_Port, LED_D2B4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD2_GPIO_Port,      LD2_Pin,      GPIO_PIN_RESET);

    // D5–D20 all OFF
    switchLedBar(0x0000);
}

/* Apply current state of flash */
void flash_state(void) {
    if (flash) flash_on(); else flash_off();
}
