#include "main.h"

typedef enum {
	NONE,
    HOME,
    TIME,
    ALARM,
	FITNESS,
	FLASHLIGHT,
	COUNTDOWN,
	STOPWATCH,
	ALARM_SET
} ScreenState;

extern volatile ScreenState currentScreen;
extern volatile ScreenState previousScreen;
