
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"

void timePage() {

	char buff[16];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	char *weekDayMap [7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
	char *weekday = weekDayMap[sDate.WeekDay];

	//uint8_t year = sDate.Year;
	sprintf(buff, "%s ", weekday);

	LCD_SendStr(buff);

	LCD_SendCmd(LCD_SECOND_LINE);

	LCD_SendStr("ALARM   ");
	LCD_SendStr("24H TIME");

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

	// Move cursor to desired position (row 0 or 1)
	uint8_t baseCmd = (row == 0) ? 0x80 : 0xC0; // LCD_LINE1 or LCD_LINE2
	LCD_SendCmd(baseCmd + col);

	sprintf(buff, "%02d:%02d", hours, minutes);
	LCD_SendStr(buff);

	sprintf(dateBuff, " %02d/%02d", day, month);
	LCD_SendStr(dateBuff);
}


/*
void alarmPage() {
	char buff[16];

	LCD_SendStr("Set Alarm for:");

	LCD_SendCmd(LCD_SECOND_LINE);


	LCD_SendStr("%02d:%02d ");

	if (!(hrtc.Init.HourFormat == RTC_HOURFORMAT_24)) {
		if (hrtc.Init.HourFormat == RTC_HOURFORMAT12_AM) {

		}
	}
}*/
/*rmat() {
	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1) {
		switch (hrtc.Init.HourFormat) {
			case RTC_HOURFORMAT12:
				hrtc.Init.HourFormat = RTC_HOURFORMAT24;
			case RTC_HOURFORMAT24:
				hrtc.Init.HourFormat = RTC_HOURFORMAT12;
		}
	}
}*/
/*
void timeDateInit() {
	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};

	  hrtc.Instance = RTC;
	    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	    hrtc.Init.AsynchPrediv = 127;
	    hrtc.Init.SynchPrediv = 255;
	    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	    if (HAL_RTC_Init(&hrtc) != HAL_OK)
	    {
	      Error_Handler();
	    }

      sTime.Hours = 0x9;
	  sTime.Minutes = 0x45;
	  sTime.Seconds = 0;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	     Error_Handler();
	  }

	  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	  sDate.Month = RTC_MONTH_AUGUST;
	  sDate.Date = 0x4;
	  sDate.Year = 0x25;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
	     Error_Handler();
	  }
}*/
