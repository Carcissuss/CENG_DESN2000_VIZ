/*
 * state.h
 *
 *  Created on: Aug 4, 2025
 *      Author: Vageesh Jha
 */

#include "main.h"

typedef enum {
	NONE,
    HOME,
    TIME,
    ALARM,
	ALARM_SET,
	FITNESS,
	OPT,
	SETTINGS,
	COUNTDOWN,
	STOPWATCH
} ScreenState;

extern volatile ScreenState currentScreen;
extern volatile ScreenState previousScreen;
