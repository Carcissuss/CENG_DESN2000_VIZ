
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

void OPTpage(uint32_t ldrValue) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%ld", ldrValue);

	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr(buffer);

	LCD_SendCmd(LCD_SECOND_LINE); // Move to second line

	LCD_SendStr("SETTINGS  ");
	LCD_SendStr("|RESET");
}

void updateOpt(uint32_t ldrValue) {
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%ld", ldrValue);
	LCD_SendCmd(LCD_RETURN_HOME);
	LCD_SendStr(buffer);
}
