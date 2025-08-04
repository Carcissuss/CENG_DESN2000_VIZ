/*
 * Cloack.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "date.h"
#include "coast.h"
#include "lcd.h"
#include "keypad.h"
#include "buzzer.h"


extern void coast_asm_delay(uint32_t milliseconds);
static bool setAlarmTimeNum(uint32_t *time, uint32_t maxTime);

// show the clock
void standardClock(uint32_t *lastSecond, uint32_t seconds, DateTime baseTime, DateTime *currentTime) {
	if (*lastSecond < seconds) {
		LCD_SendCmd(LCD_CLEAR_DISPLAY);
		*currentTime = updateTime(baseTime, seconds);
		char timeStr[30];
		timeToStr(timeStr, *currentTime);
		LCD_SendTime(timeStr);
		*lastSecond = seconds;
	}
}

// show the countdown timer
void standardTimerShow(Alarm countdown, char *str, Alarm lastCountdown) {
	if (lastCountdown.hour == countdown.hour && lastCountdown.minute == countdown.minute && lastCountdown.second == countdown.second) {
		return;
	}
	char buffer[10];
	sprintf(buffer, "%02lu:%02lu:%02lu", countdown.hour, countdown.minute, countdown.second);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr(buffer);
	LCD_SendCmd(LCD_SECOND_LINE);
	LCD_SendStr(str);
}

// set the alarm time
Alarm userSetAlarm(Alarm alarm) {
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr("Set alarm");
	coast_asm_delay(1000);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);

	int enterTime = 0;
	uint32_t hour = 25;
	uint32_t minute = 61;
	uint32_t second = 61;
	bool is_break = false;
	while (1) {
		if (enterTime == 0) {
			LCD_SendStr("Hour: ");
			is_break = setAlarmTimeNum(&hour, 23);
			enterTime = 1;
		} else if (enterTime == 1) {
			LCD_SendCmd(LCD_CLEAR_DISPLAY);
			LCD_SendStr("Minute: ");
			is_break = setAlarmTimeNum(&minute, 60);
			enterTime = 2;
		} else {
			LCD_SendCmd(LCD_CLEAR_DISPLAY);
			LCD_SendStr("Second: ");
			is_break = setAlarmTimeNum(&second, 60);
			break;
		}

		if (is_break) break;
	}

	if (!is_break) {
		LCD_SendCmd(LCD_CLEAR_DISPLAY);
		LCD_SendStr("Alarm has been set");
		coast_asm_delay(1000);
		LCD_SendCmd(LCD_CLEAR_DISPLAY);
	}
	alarm = setAlarm(alarm, hour, minute, second);
	return alarm;
}

// update the countdown timer
Alarm countDownUpdate(Alarm countDownTimer, uint32_t seconds, uint32_t *savedSecond, TIM_HandleTypeDef htim1, Alarm lastCountdown) {
	if (isFinishCountDown(countDownTimer)) {
		return countDownTimer;
	}
	/* if enter a new second, upgrade the down_counter */
	if (*savedSecond < seconds) {
	    if (countDownTimer.second > 0) {
	    	countDownTimer.second--;
	    } else if (countDownTimer.minute > 0) {
	    	countDownTimer.second = 59;
	        countDownTimer.minute--;
	    } else if (countDownTimer.hour > 0) {
	    	countDownTimer.second = 59;
	    	countDownTimer.minute = 59;
	    	countDownTimer.hour--;
	    } else {
	    	countDownTimer.second = 0;
	        countDownTimer.minute = 0;
	        countDownTimer.hour = 0;
	    }
	/* upgrade the lastSecond to the current second */
	    *savedSecond = seconds;

	/* After the countdown finished, the buzzer reminds the user */
		if (isFinishCountDown(countDownTimer)) {
			standardTimerShow(countDownTimer, "Countdown", lastCountdown);
			generate_sound(494, 50);
			HAL_GPIO_WritePin(LED_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED4_Pin, GPIO_PIN_SET);
			coast_asm_delay(450);
			HAL_GPIO_WritePin(LED_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED4_Pin, GPIO_PIN_RESET);
			generate_sound(440, 50);
			coast_asm_delay(150);
			HAL_GPIO_WritePin(LED_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_Port, LED4_Pin, GPIO_PIN_SET);
			generate_sound(392, 50);
			coast_asm_delay(300);
			HAL_GPIO_WritePin(LED_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_Port, LED4_Pin, GPIO_PIN_RESET);
			LCD_SendCmd(LCD_CLEAR_DISPLAY);
			stop_sound(htim1);
		}
	}
	return countDownTimer;
}

Alarm stopwatchUpdate(Alarm stopwatch, uint32_t seconds, uint32_t *lastSecond) {
    if (*lastSecond >= seconds) {
        return stopwatch;
    }

    // Increment the seconds
    stopwatch.second++;
    if (stopwatch.second >= 60) {
        stopwatch.second = 0;
        stopwatch.minute++;
        if (stopwatch.minute >= 60) {
            stopwatch.minute = 0;
            if (stopwatch.hour < 24) {
                stopwatch.hour++;
            }
        }
    }

    *lastSecond = seconds;
    return stopwatch;
}

// input the time in type of char and transfer it to number
static bool setAlarmTimeNum(uint32_t *time, uint32_t maxTime) {
	char key_value;
	bool is_break = false;
	char buffer[3];
	int i = 0;

	uint32_t returnedTime = 61;

	while (1) {
		key_value = scan_keypad();
		if (key_value != 0) {
			coast_asm_delay(10);
			if (key_value == scan_keypad()) {
				LCD_SendData(key_value);
				if (key_value < '0' || key_value > '9') {
					LCD_SendCmd(LCD_CLEAR_DISPLAY);
					LCD_SendStr("Invalid num");
					is_break = true;
					coast_asm_delay(500);
					LCD_SendCmd(LCD_CLEAR_DISPLAY);
					break;
				}

				buffer[i] = key_value;
				while (scan_keypad() == key_value);
				i++;
				if (i > 1) {
					buffer[i] = '\0';
					returnedTime = (uint32_t) atoi(buffer);
					if (returnedTime > maxTime) {
						LCD_SendCmd(LCD_CLEAR_DISPLAY);
						LCD_SendStr("Invalid hour");
						is_break = true;
						coast_asm_delay(500);
						LCD_SendCmd(LCD_CLEAR_DISPLAY);
						break;
					}
					break;
				}
			}
		}
	}

	*time = returnedTime;

	return is_break;
}


