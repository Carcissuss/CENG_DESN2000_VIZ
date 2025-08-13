/*
 * time.c
 *
 *  Created on: Aug 03, 2025
 *      Author: Zhaoji Gou
 */

#include "time.h"
#include "lcd.h"
#include "main.h"
#include "buzzer.h"
#include "coast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

extern void coast_asm_delay(uint32_t milliseconds);

Timer setLabTimer(Timer timer, char label, uint32_t second, uint32_t minute) {
	timer.label = label;
	timer.durationMin = minute;
	timer.durationSec = second;
	timer.countDownMin = 0;
	timer.countDownSec = 0;
	return timer;
}

void showLabTimer(Timer *timer, Timer *lastTimers) {

	bool is_same = true;
	for (int i = 0; i < 4; i++) {
		if (timer[i].countDownMin != lastTimers[i].countDownMin || timer[i].countDownSec != lastTimers[i].countDownSec
				|| timer[i].label != lastTimers[i].label) {
			is_same = false;
			break;
		}
	}

	if (is_same) {
		return;
	}

	char temp[35];
	sprintf(temp, "%c %02lu:%02lu  %c %02lu:%02lu%c %02lu:%02lu  %c %02lu:%02lu",
			timer[0].label, timer[0].countDownMin, timer[0].countDownSec,
			timer[1].label, timer[1].countDownMin, timer[1].countDownSec,
			timer[2].label, timer[2].countDownMin, timer[2].countDownSec,
			timer[3].label, timer[3].countDownMin, timer[3].countDownSec);
	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	LCD_SendStr(temp);
}

void labTimersToStr(char *str, Timer *timer) {
	sprintf(str, "%c:%02lu:%02lu-%c:%02lu:%02lu-%c:%02lu:%02lu-%c:%02lu:%02lu",
			timer[0].label, timer[0].durationMin, timer[0].durationSec,
			timer[1].label, timer[1].durationMin, timer[1].durationSec,
			timer[2].label, timer[2].durationMin, timer[2].durationSec,
			timer[3].label, timer[3].durationMin, timer[3].durationSec);
}

void strToLabTimers(char *str, Timer *timer) {
    const char *s = str;
    char *token;
    int index = 0;

    while ((token = strtok(s, "-")) != NULL) {
        if (index >= 4) break;  // 假设只有4个timer
        s = NULL;  // 在第一次调用之后，strtok要求将s设为NULL

        if (strlen(token) > 0) {
            sscanf(token, "%c:%02lu:%02lu", &timer[index].label, &timer[index].durationMin, &timer[index].durationSec);
            timer[index].countDownMin = 0;
            timer[index].countDownSec = 0;
        }
        index++;
    }
}

void *updateLabTimer(Timer *timer, uint32_t seconds, uint32_t *lastSecond,
						bool *timer1, bool *timer2, bool *timer3, bool *timer4, TIM_HandleTypeDef htim1, Timer *lastTimers) {
	if (*lastSecond >= seconds) {
		return NULL;
	}


	for (int i = 0; i < 4; i++) {
		if (timer[i].label == ' ' || isFinishedLabTimer(timer[i])) {
			continue;
		}
	    if (timer[i].countDownSec > 0) {
	    	timer[i].countDownSec--;
	    } else if (timer[i].countDownMin > 0) {
	    	timer[i].countDownSec = 59;
	    	timer[i].countDownMin--;
	    } else {
	    	timer[i].countDownSec = 0;
	    	timer[i].countDownMin = 0;
	    }
	    if (isFinishedLabTimer(timer[i])) {
	    	showLabTimer(timer, lastTimers);
	    	if (i == 0) {
	    		*timer1 = true;
	    		generate_sound(262, 50);
	    	} else if (i == 1) {
	    		*timer2 = true;
	    		generate_sound(330, 50);
	    	} else if (i == 2) {
	    		*timer3 = true;
	    		generate_sound(392, 50);
	    	} else {
	    		*timer4 = true;
	    		generate_sound(523, 50);
	    	}
	    	coast_asm_delay(500);
	    	LCD_SendCmd(LCD_CLEAR_DISPLAY);
	    	stop_sound();
	    }
	}
	*lastSecond = seconds;
}

bool isFinishedLabTimer(Timer timer) {
	return (timer.countDownMin == 0 && timer.countDownSec == 0);
}
