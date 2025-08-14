/*
 * fitness.h
 *
 *  Created on: Aug 14, 2025
 *      Author: zhaoj
 */

#ifndef INC_FITNESS_H_
#define INC_FITNESS_H_

#include "main.h"
#include "stdbool.h"

typedef struct {
	uint32_t minute;
	uint32_t second;
	bool countdown_enable;
} Countdown;

typedef struct {
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
	bool stopwatch_enable;
} Stopwatch;

extern void coast_asm_delay(uint32_t milliseconds);

/* fitness page display functions */
void fitnessPage();
void updateFitness(uint8_t row, uint8_t col);

/* declaration of countdown functions */
void countdownPage(Countdown countdown);
void toggleCountdown(Countdown *countdown);
void mintueCountdown(Countdown *countdown);
void secondCountdown(Countdown *countdown);
void updateCountdown(Countdown countdown);
void resetCountdown(Countdown *countdown);
void runCountdown(Countdown *countdown, uint32_t *lastSecond, uint32_t second, TIM_HandleTypeDef htim1, bool enable_sound, bool enable_vibration);

/* declaration of stopwatch functions */
void stopwatchPage(Stopwatch stopwatch);
void lapStopwatch(Stopwatch stopwatch);
void resetStopwatch(Stopwatch *stopwatch);
void toggleStopwatch(Stopwatch *stopwatch);
void updateStopwatch(Stopwatch stopwatch);
void runStopwatch(Stopwatch *stopwatch, uint32_t *lastSecond, uint32_t second);


#endif /* INC_FITNESS_H_ */
