/*
 * state.c
 *
 *  Created on: Aug 4, 2025
 *      Author: Vageesh Jha
 */

/* Initialises states for currentScreen and previousScreen */
#include "state.h"

volatile ScreenState currentScreen = HOME;
volatile ScreenState previousScreen = NONE;

