/*
 * lcd.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Admin
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

void coast_lcd_init();
void LCD_PutNibble(uint8_t nibble);
void LCD_SendData(uint8_t c);
void LCD_SendCmd(uint8_t c);
void LCD_SendStr(char *str);
void LCD_Pulse();
void LCD_SendTime(const char *str);
//commands
#define LCD_CLEAR_DISPLAY 0b00000001
//... LCD_RETURN_HOME, LCD_SECOND_LINE....
#define LCD_RETURN_HOME  0b00000010
#define LCD_SECOND_LINE  0b11000000
#define LCD_CURSOR_LEFT  0b00010000
#define LCD_CURSOR_RIGHT 0b00010100
#define LCD_SHIFT_LEFT   0b00011000
#define LCD_SHIFT_RIGHT  0b00011100

#endif /* INC_LCD_H_ */


