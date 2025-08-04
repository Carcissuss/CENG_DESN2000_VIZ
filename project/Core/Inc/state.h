#include "main.h"

typedef enum {
	NONE,
    HOME,
    TIME,
    ALARM
} ScreenState;

extern volatile ScreenState currentScreen;
extern volatile ScreenState previousScreen;
