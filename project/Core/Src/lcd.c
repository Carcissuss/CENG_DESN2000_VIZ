
#include "main.h"
#include "coast.h"
#include "lcd.h"

/* initialize the lcd */
void coast_lcd_init(){
	// 1. wait for enough time to stabilise
	 HAL_Delay(50);
	 // 2. send command 0011 (function set) and wait for >=4.1 ms (enough wait inside
	 // the pulse)
	 LCD_PutNibble(0b0011);
	 LCD_Pulse();
	 HAL_Delay(5);
	 // 3. send command 0011 (function set) again and wait for >=100 us
	 // fill this
	 LCD_PutNibble(0b0011);
	 LCD_Pulse();
	 HAL_Delay(1);
	 // 4. send command 0011 (function set) again
	 // fill this
	 LCD_PutNibble(0b0011);
	 LCD_Pulse();
	 // 5. send command 0010 to set to 4-bit bus mode
	 // fill this
	 LCD_PutNibble(0b0010);
	 LCD_Pulse();
	 // 6. send command 0010 1100 (function set: 4-bit mode, 2-lines, 5x8 font)
	 LCD_SendCmd(0b00101100);
	 // 7. Send command 0000 1000 to display ON/OFF
	 LCD_SendCmd(0b00001000);
	 // 8. Send command to clear the display
	 // fill this
	 LCD_SendCmd(LCD_CLEAR_DISPLAY);
	 // 9. Send command set entry mode (increment cursor, no display shift)
	 // fill this
	 LCD_SendCmd(0b00000110);
	 // 10. send command 0000 1111 to display on, cursor on, blink on
	 // fill this
	 LCD_SendCmd(0b00001111);
}

/* Enable for one LCD input */
void LCD_Pulse(){
	HAL_GPIO_WritePin(LCD_E_Port, LCD_E_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_E_Port, LCD_E_Pin, 0);
	HAL_Delay(1);
}

/* input the values every 4 bits once */
void LCD_PutNibble(uint8_t nibble){
	uint8_t D4 = nibble & 1;
	uint8_t D5 = (nibble >> 1) & 1;
	uint8_t D6 = (nibble >> 2) & 1;
	uint8_t D7 = (nibble >> 3) & 1;
	HAL_GPIO_WritePin(LCD_Data_Port, LCD_D4_Pin, D4? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_Data_Port, LCD_D5_Pin, D5? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_Data_Port, LCD_D6_Pin, D6? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_Data_Port, LCD_D7_Pin, D7? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* send Command information to LCD */
void LCD_SendCmd(uint8_t c){
	LCD_PutNibble(c >> 4);
	LCD_Pulse();
	LCD_PutNibble(c);
	LCD_Pulse();
}

/* Send data expected to display on LCD */
void LCD_SendData(uint8_t c){
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, 1);
	LCD_PutNibble(c >> 4);
	LCD_Pulse();
	LCD_PutNibble(c);
	LCD_Pulse();
	HAL_GPIO_WritePin(LCD_RS_Port, LCD_RS_Pin, 0);
}

/* send in a string of information to display on the LCD */
void LCD_SendStr(char *str){
	for (int i = 0; str[i]; i++) {
		if (i == 15) {
			LCD_SendCmd(LCD_SECOND_LINE);
		}
		LCD_SendData((uint8_t)str[i]);
	}
}


