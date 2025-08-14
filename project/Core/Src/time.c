/*
 * time.c
 *
 *  Created on: Aug 4, 2025
 *      Author: Vageesh Jha
 */
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_AlarmTypeDef sAlarm;

bool is_24_hour_format = true;
extern bool timeFormatChanged;

/* Sets up the Time Page and displays in either 24H or 12H depending on format flag */
void timePage() {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	char buff[16];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	char *weekDayMap[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
	char *weekday = weekDayMap[sDate.WeekDay - 1];

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

/* For custom time setting, sets up the page */
void timeSetPage() {
	char buff[20];
	LCD_SendCmd(LCD_CLEAR_DISPLAY);

	LCD_SendCmd(0x80);
	LCD_SendStr("Set Time to:");

	LCD_SendCmd(LCD_SECOND_LINE);
	uint8_t h = sTime.Hours;
	uint8_t m = sTime.Minutes;

	if (is_24_hour_format) {
		snprintf(buff, sizeof(buff), "%02d:%02d", h, m);
	} else {
		const char *ampm = (h >= 12) ? "P.M" : "A.M";
		uint8_t dh = h % 12;
		if (dh == 0) dh = 12;
		snprintf(buff, sizeof(buff), "%02d:%02d %s", dh, m, ampm);
	}
	LCD_SendStr(buff);
}

/* Confirms new time selection based on user input */
void timeSetConfirm() {
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	LCD_SendCmd(LCD_CLEAR_DISPLAY);   // clear display for confirmation
	coast_asm_delay(2);

	LCD_SendStr("TIME SET TO:");    // top line
	LCD_SendCmd(LCD_SECOND_LINE);
	
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", sTime.Hours, sTime.Minutes);
	LCD_SendStr(buffer);
}

/* Refreshes time being set by user on the CUSTOM_TIME page */
void updateSetTime(uint8_t row, uint8_t col) {
	char buff[16];
	uint8_t h = sTime.Hours;
	uint8_t m = sTime.Minutes;

	uint8_t baseCmd = (row == 0) ? 0x80 : 0xC0;
	LCD_SendCmd(baseCmd + col);

	if (is_24_hour_format) {
		snprintf(buff, sizeof(buff), "%02d:%02d", h, m);
	} else {
		const char *ampm = (h >= 12) ? "P.M" : "A.M";
		uint8_t dh = h % 12; if (dh == 0) dh = 12;
		snprintf(buff, sizeof(buff), "%02d:%02d %s", dh, m, ampm);
	}
		LCD_SendStr(buff);
}

/* Updates time and date on Home and Time pages */
void updateTime(uint8_t row, uint8_t col) {
	char buff[8];
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

	sprintf(dateBuff, " %02d/%02d", day, month);
	LCD_SendStr(dateBuff);

	sprintf(buff, " %02d:%02d", displayHour, minutes);
	LCD_SendStr(buff);
}

/* Sets up the page to change the alarm */
void alarmPage() {
	char buff[20];
	LCD_SendCmd(LCD_CLEAR_DISPLAY);

	LCD_SendCmd(0x80);
	LCD_SendStr("Set Alarm for:");

	LCD_SendCmd(LCD_SECOND_LINE);
	uint8_t h = sAlarm.AlarmTime.Hours;
	uint8_t m = sAlarm.AlarmTime.Minutes;

	if (is_24_hour_format) {
		snprintf(buff, sizeof(buff), "%02d:%02d", h, m);
	} else {
		const char *ampm = (h >= 12) ? "P.M" : "A.M";
		uint8_t dh = h % 12;
		if (dh == 0) dh = 12;
		snprintf(buff, sizeof(buff), "%02d:%02d %s", dh, m, ampm);
	}
	LCD_SendStr(buff);
}

/* Refreshes the alarm the user is setting on the alarm page */
void updateAlarm(uint8_t row, uint8_t col) {

	char buff[16];
	uint8_t h = sAlarm.AlarmTime.Hours;
	uint8_t m = sAlarm.AlarmTime.Minutes;

	uint8_t baseCmd = (row == 0) ? 0x80 : 0xC0;
	LCD_SendCmd(baseCmd + col);

	if (is_24_hour_format) {
		snprintf(buff, sizeof(buff), "%02d:%02d", h, m);
	} else {
		const char *ampm = (h >= 12) ? "P.M" : "A.M";
		uint8_t dh = h % 12; if (dh == 0) dh = 12;
		snprintf(buff, sizeof(buff), "%02d:%02d %s", dh, m, ampm);
	}
	LCD_SendStr(buff);
}

/* Flag for switching between 24H and 12H */
void switchTimeFormat() {
    is_24_hour_format = !is_24_hour_format;
    timeFormatChanged = true;
}

/* Switches between AM or PM on the Alarm */
void switchAMPM(){

	if (is_24_hour_format) return;
	uint8_t h = sAlarm.AlarmTime.Hours; // 0..23
	    if (h >= 12) {
	        sAlarm.AlarmTime.Hours = h - 12; // PM -> AM
	    } else {
	        sAlarm.AlarmTime.Hours = h + 12; // AM -> PM
	    }
}

/* Switches between AM or PM on the custom time setting */
void switchTimeAMPM(){
	if (is_24_hour_format) return;
	uint8_t h = sTime.Hours; // 0..23
	    if (h >= 12) {
	        sTime.Hours = h - 12; // PM -> AM
	    } else {
	        sTime.Hours = h + 12; // AM -> PM
	    }
}

/* Changes hour of alarm by iterating by 1 hour on SW3 press */
void changeAlarmHour() {
	uint8_t h = sAlarm.AlarmTime.Hours;
	h = (h + 1) % 24;
	sAlarm.AlarmTime.Hours = h;
}

/* Changes minute of alarm by iterating by 5 minutes on Blue B1 press */
void changeAlarmMin() {
	uint8_t m = sAlarm.AlarmTime.Minutes;
	m = (m + 5) % 60;
	sAlarm.AlarmTime.Minutes = m;
}

/* Changes hour of Time by 1 */
void changeTimeHour() {
	uint8_t h = sTime.Hours;
	h = (h + 1) % 24;
	sTime.Hours = h;
}

/* Changes minute of custom Time by 5 if double press or 1 if single press */
void changeTimeMin(int interval) {
	uint8_t m = sTime.Minutes;
	if (interval == 5) {
		m = (m + 5) % 60;
	} else {
		m = (m + 1) % 60;
	}

	sTime.Minutes = m;
}

/* Confirms the selected alarm */
void alarmConfirm(void) {
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

	LCD_SendCmd(LCD_CLEAR_DISPLAY);   // clear display for confirmation
	coast_asm_delay(2);

	LCD_SendStr("ALARM SET FOR:");    // top line
	LCD_SendCmd(LCD_SECOND_LINE);
	
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%02d:%02d", sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes);
	LCD_SendStr(buffer);
}


