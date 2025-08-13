
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

RTC_TimeTypeDef sAlarm;
bool is_24_hour_format = true;
extern bool timeFormatChanged;

void timePage() {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	char buff[16];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	char *weekDayMap[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
	char *weekday = weekDayMap[sDate.WeekDay - 1];

	//uint8_t year = sDate.Year;
	sprintf(buff, "%s ", weekday);

	LCD_SendStr(buff);

	LCD_SendCmd(LCD_SECOND_LINE);

	LCD_SendStr("ALARM");


	if (is_24_hour_format) {
	    LCD_SendStr("   24H_TIME");
	} else {
	    LCD_SendStr("   12H_TIME");
	}

}

void updateTime(uint8_t row, uint8_t col) {
	char buff[8];  // Enough for HH:MM + null
	char dateBuff[8];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	uint8_t hours = sTime.Hours;
	uint8_t minutes = sTime.Minutes;
	uint8_t day = sDate.Date;
	uint8_t month = sDate.Month;

	uint8_t displayHour = hours;
	 if (!is_24_hour_format) {
	        if (hours == 0) {
	        	displayHour = 12;
	        }
	        else if (hours > 12) {
	        	displayHour = hours - 12;
	        }
	    }
	// Move cursor to desired position (row 0 or 1)
	uint8_t baseCmd = (row == 0) ? 0x80 : 0xC0; // LCD_LINE1 or LCD_LINE2
	LCD_SendCmd(baseCmd + col);

	sprintf(buff, "%02d:%02d ", hours, minutes);
	LCD_SendStr(buff);

	sprintf(dateBuff, " %02d/%02d", day, month);
	LCD_SendStr(dateBuff);
}


void alarmPage() {
	char buff[16];

	LCD_SendStr("Set Alarm for:");

	LCD_SendCmd(LCD_SECOND_LINE);

	uint8_t hours =  sAlarm.Hours;
	uint8_t minutes = sAlarm.Minutes;

	sprintf(buff, "%02d:%02d A.M", hours, minutes);
	LCD_SendStr(buff);

	if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}

void switchTimeFormat() {
    is_24_hour_format = !is_24_hour_format;
    timeFormatChanged = true;
}


void switchAMPM(){
	sAlarm.Hours += 12;
	char buff[16];
	uint8_t baseCmd = (2) ? 0x80 : 0xC0; // LCD_LINE1 or LCD_LINE2
	LCD_SendCmd(baseCmd + 6);
	if (sAlarm.TimeFormat == RTC_HOURFORMAT12_AM) {
		sprintf(buff, "A");
		LCD_SendStr(buff);
	} else if (sAlarm.TimeFormat == RTC_HOURFORMAT12_PM) {
		sprintf(buff, "P");
		LCD_SendStr(buff);
	}
}

void changeAlarmHour() {
	if (sTime.TimeFormat == RTC_HOURFORMAT_12) {
		if (sAlarm.Hours <= 12) {
			sAlarm.Hours += 1;
		} else {
			sAlarm.Hours = 1;
		}
	} else if (sTime.TimeFormat == RTC_HOURFORMAT_24) {
		if (sAlarm.Hours <= 23) {
			sAlarm.Hours += 1;
		} else {
			sAlarm.Hours = 0;
		}
	}
}

void changeAlarmMin() {
	if (sAlarm.Minutes < 60) {
		sAlarm.Minutes += 5;
	} else {
		sAlarm.Minutes = 0;
	}
}
