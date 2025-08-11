
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void homePage() {

	LCD_SendStr("FITNESS ");

	char buff[16];

	int temp = 21; // placeholder
	sprintf(buff, "%dC ", temp);
	LCD_SendStr(buff);

	LCD_SendStr(" 57%");

	LCD_SendCmd(LCD_SECOND_LINE); // Move to second line

	LCD_SendStr("OPT ");
}
