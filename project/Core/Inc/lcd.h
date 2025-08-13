#include "main.h"


void coast_lcd_init();
void LCD_Pulse();
void LCD_PutNibble(uint8_t nibble);
void LCD_SendCmd(uint8_t c);
void LCD_SendData(uint8_t c);
void LCD_SendStr(char *str);
void LCD_SendTime(const char *str);


//commands
#define LCD_CLEAR_DISPLAY 0b00000001
//... LCD_RETURN_HOME, LCD_SECOND_LINE....
#define LCD_RETURN_HOME  0b00000010
#define LCD_SECOND_LINE  0b11000000

