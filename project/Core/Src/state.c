/*
 * state.c
 *
 *  Created on: Aug 4, 2025
 *      Author: Vageesh Jha
 */

#include "state.h"

volatile ScreenState currentScreen = HOME;
volatile ScreenState previousScreen = NONE;
