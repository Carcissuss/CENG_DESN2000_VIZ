/*
 * vibration.h
 *
 *  Created on: Aug 5, 2025
 *      Author: Admin
 */

#ifndef INC_VIBRATION_H_
#define INC_VIBRATION_H_

#include "main.h"

// One mechanical revolution of your stepper
#define STEPS_PER_REV 512

void vibration_call(uint16_t steps);   // schedule N steps (non-blocking)
void vibration_stop();
void vibration_tick_1ms();            // call from TIM7 1ms ISR
void generate_vibration();
#endif /* INC_VIBRATION_H_ */
