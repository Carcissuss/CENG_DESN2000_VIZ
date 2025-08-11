#include "main.h"

typedef enum {
	NONE,
    HOME,
    TIME,
    ALARM,
	FITNESS,
	FLASHLIGHT,
	COUNTDOWN,
	ALARM,
	STOPWATCH
} ScreenState;

extern volatile ScreenState currentScreen;
extern volatile ScreenState previousScreen;
