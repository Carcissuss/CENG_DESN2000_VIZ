
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"
#include "time.h"

RTC_AlarmTypeDef sAlarm;
bool is_24_hour_format = true;
extern bool timeFormatChanged;
extern void coast_asm_delay(uint32_t milliseconds);

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

	uint8_t hours =  sTime.Hours;
	uint8_t minutes = sTime.Minutes;

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

	if (is_24_hour_format) return;
	uint8_t h = sAlarm.AlarmTime.Hours; // 0..23
	    if (h >= 12) {
	        sAlarm.AlarmTime.Hours = h - 12; // PM -> AM
	    } else {
	        sAlarm.AlarmTime.Hours = h + 12; // AM -> PM
	    }
}

void changeAlarmHour() {
	uint8_t h = sAlarm.AlarmTime.Hours;
	h = (h + 1) % 24;
	sAlarm.AlarmTime.Hours = h;
}

void changeAlarmMin() {
	uint8_t m = sAlarm.AlarmTime.Minutes;
	m = (m + 5) % 60;
	sAlarm.AlarmTime.Minutes = m;
}

void alarmConfirm(void) {

	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	LCD_SendCmd(LCD_CLEAR_DISPLAY);   // clear display for confirmation
	coast_asm_delay(2);

	LCD_SendStr("ALARM SET FOR:");    // top line
	LCD_SendCmd(LCD_SECOND_LINE);
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes);
	LCD_SendStr(buffer);
}


void countdownPage(Countdown *countdown) {
	char buffer[16];

	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("Countdown:");
	LCD_SendCmd(LCD_SECOND_LINE);
	snprintf(buffer, sizeof(buffer), "%02ld:%02ld:%02ld", countdown->hour, countdown->minute, countdown->second);
	LCD_SendStr(buffer);
}


