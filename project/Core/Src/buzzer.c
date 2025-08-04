/*
 * buzzer.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include "main.h"
extern void coast_asm_delay(uint32_t milliseconds);
void generate_sound(uint16_t frequency, uint8_t duty_cycle, TIM_HandleTypeDef htim1) {
	/* protect the function from invalid frequency input */
	if (frequency == 0) return;
	/* millisecond trigger period */
	uint32_t period = 72000 / frequency;
	/* setup PWM channel and tim1 autoloader */
	TIM1->ARR = period - 1;
	TIM1->CCR1 = (period * duty_cycle) / 100;
	/* start the timer1 and buzzer */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void stop_sound(TIM_HandleTypeDef htim1) {
	/* stop the timer1 and buzzer */
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
}

void play_note(uint16_t note, uint32_t duration, uint8_t volume, TIM_HandleTypeDef htim1) {
	generate_sound(note, volume, htim1);
	coast_asm_delay(duration);
    stop_sound(htim1);
}
