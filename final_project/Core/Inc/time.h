/*
 * time.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#ifndef INC_TIME_H_
#define INC_TIME_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef struct {
	char label;
	uint32_t durationMin;
	uint32_t durationSec;
	uint32_t countDownMin;
	uint32_t countDownSec;
} Timer;

Timer setLabTimer(Timer timer, char label, uint32_t second, uint32_t minute);
void showLabTimer(Timer *timer, Timer *lastTimers);
void *updateLabTimer(Timer *timer, uint32_t seconds, uint32_t *lastSecond,
						bool *timer1, bool *timer2, bool *timer3, bool *timer4, TIM_HandleTypeDef htim1, Timer *lastTimers);
bool isFinishedLabTimer(Timer timer);
void labTimersToStr(char *str, Timer *timer);
void strToLabTimers(char *str, Timer *timer);

#endif /* INC_TIME_H_ */
