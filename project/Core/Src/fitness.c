/*
 * fitness.c
 *
 *  Created on: Aug 14, 2025
 *      Author: zhaoj
 */
#include <stdio.h>
#include "main.h"
#include "coast.h"
#include "lcd.h"
#include "buzzer.h"
#include "fitness.h"
#include "time.h"
#include "vibration.h"

extern void coast_asm_delay(uint32_t milliseconds);

extern bool is_24_hour_format;

/* display the fitness information on LCD when enter the page */
void fitnessPage() {
	char buffer[16];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	uint8_t hours = sTime.Hours;
	uint8_t minutes = sTime.Minutes;

	uint8_t displayHour = hours;
	if (!is_24_hour_format) {
		if (hours == 0) {
			displayHour = 12;
		}
		else if (hours > 12) {
			displayHour = hours - 12;
		}
	}

	sprintf(buffer, "%02d:%02d", displayHour, minutes);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("Fitness:");
	LCD_SendStr(" 150 BPM");
	LCD_SendCmd(LCD_SECOND_LINE);
	uint8_t baseCmd = (0) ? 0x80 : 0xC0;
	LCD_SendCmd(baseCmd + 7);  // position cursor
	LCD_SendStr(" 3400 STP");
	LCD_SendStr(buffer);
}

/* update the fitness information on LCD every second */
void updateFitness(uint8_t row, uint8_t col) {
    char buffer[16];

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    uint8_t hours = sTime.Hours;
    uint8_t minutes = sTime.Minutes;

    uint8_t displayHour = hours;
    if (!is_24_hour_format) {
        if (hours == 0) {
            displayHour = 12;
        }
        else if (hours > 12) {
            displayHour = hours - 12;
        }
    }

    uint8_t baseCmd = (row == 0) ? 0x80 : 0xC0;
    LCD_SendCmd(baseCmd + col);  // position cursor

    sprintf(buffer, "%02d:%02d", displayHour, minutes);
    LCD_SendStr(buffer);         // print at given row/col
}

/* display the countdown information when enter the page */
void countdownPage(Countdown countdown) {
	char buffer[16];
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("Countdown:");
	LCD_SendCmd(LCD_SECOND_LINE);
	snprintf(buffer, sizeof(buffer), "%02ld:%02ld", countdown.minute, countdown.second);
	LCD_SendStr(buffer);
}

/* enable/disable the countdown */
void toggleCountdown(Countdown *countdown) {
	if (countdown->countdown_enable) {
		countdown->countdown_enable = false;
	} else {
		countdown->countdown_enable = true;
	}
}

/* add the countdown minute by one */
void mintueCountdown(Countdown *countdown) {
	if (countdown->minute >= 60) {
		countdown->minute = 60;
	} else {
		(countdown->minute)++;
	}
}

/* add the countdown second by one */
void secondCountdown(Countdown *countdown) {
	if (countdown->second >= 60) {
		countdown->second = 60;
	} else {
		(countdown->second)++;
	}
}

/* update the countdown display on LCD every second */
void updateCountdown(Countdown countdown) {
	char buffer[16];
	LCD_SendCmd(LCD_SECOND_LINE);
	snprintf(buffer, sizeof(buffer), "%02ld:%02ld", countdown.minute, countdown.second);
	LCD_SendStr(buffer);
}

/* reset all values of countdown to zero and disable the countdown */
void resetCountdown(Countdown *countdown) {
	countdown->minute = 0;
	countdown->second = 0;
	countdown->countdown_enable = false;
}

/* update the countdown value every second when the countdown is enabled */
void runCountdown(Countdown *countdown, uint32_t *lastSecond, uint32_t second,
			TIM_HandleTypeDef htim1, bool enable_sound, bool enable_vibration) {
	if (*lastSecond >= second) {
		return;
	}
	if (countdown->second > 0) {
		countdown->second--;
	} else if (countdown->minute > 0) {
		countdown->second = 59;
		countdown->minute--;
	} else {
		countdown->second = 0;
		countdown->minute = 0;
	}
	if (countdown->second == 0 && countdown->minute == 0) {
		countdown->countdown_enable = false;
		if (enable_sound) {
			play_note(460, 300, 50, htim1);
			play_note(0, 50, 50, htim1);
			play_note(300, 150, 50, htim1);
			play_note(0, 50, 50, htim1);
			play_note(220, 300, 50, htim1);
			play_note(0, 50, 50, htim1);
			play_note(460, 300, 50, htim1);
			play_note(0, 50, 50, htim1);
			play_note(300, 150, 50, htim1);
			play_note(0, 50, 50, htim1);
			play_note(220, 300, 50, htim1);
			play_note(0, 50, 50, htim1);
		}
		if (enable_vibration) {vibration_call(STEPS_PER_REV);}
		LCD_SendCmd(LCD_CLEAR_DISPLAY);
		LCD_SendStr("Countdown Done");
		stop_sound(htim1);
		coast_asm_delay(1500);
		LCD_SendCmd(LCD_CLEAR_DISPLAY);
		LCD_SendStr("Countdown:");
	}
	*lastSecond = second;
}

/* display the stopwatch page when first enter the page */
void stopwatchPage(Stopwatch stopwatch) {
	char buffer[16];
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("Stopwatch:  lap:");

	LCD_SendCmd(LCD_SECOND_LINE);
	snprintf(buffer, sizeof(buffer), "%02ld:%02ld", stopwatch.minute, stopwatch.second);
	LCD_SendStr(buffer);
}

/* record the current stopwatch value and display on the LCD Row 1 */
void lapStopwatch(Stopwatch stopwatch) {
	char buffer[16];
	coast_asm_delay(11);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	coast_asm_delay(11);
	LCD_SendCmd(LCD_RETURN_HOME);
	snprintf(buffer, sizeof(buffer), "lap:%02ld:%02ld:%02ld", stopwatch.hour, stopwatch.minute, stopwatch.second);
	LCD_SendStr(buffer);
}

/* reset the stopwatch values to zero and disable it */
void resetStopwatch(Stopwatch *stopwatch) {
	stopwatch->minute = 0;
	stopwatch->second = 0;
	stopwatch->stopwatch_enable = false;
}

/* enable/disable the stopwatch */
void toggleStopwatch(Stopwatch *stopwatch) {
	if (stopwatch->stopwatch_enable) {
		stopwatch->stopwatch_enable = false;
	} else {
		stopwatch->stopwatch_enable = true;
	}
}

/* update the stopwatch value every second on LCD */
void updateStopwatch(Stopwatch stopwatch) {
	char buffer[16];
	LCD_SendCmd(LCD_SECOND_LINE);
	snprintf(buffer, sizeof(buffer), "%02ld:%02ld:%02ld", stopwatch.hour, stopwatch.minute, stopwatch.second);
	LCD_SendStr(buffer);
}

/* update the stopwatch value every second when the stopwatch is enabled */
void runStopwatch(Stopwatch *stopwatch, uint32_t *lastSecond, uint32_t second) {
	if (*lastSecond >= second) {
		return;
	}
	if (stopwatch->hour == 99) {
		stopwatch->hour = 0;
	} else if (stopwatch->minute == 60) {
		stopwatch->hour++;
		stopwatch->second = 0;
		stopwatch->minute = 0;
	} else if (stopwatch->second == 60) {
		stopwatch->second = 0;
		stopwatch->minute++;
	} else {
		stopwatch->second++;
	}
	*lastSecond = second;
}
