/*
 * buzzer.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include "main.h"
extern void coast_asm_delay(uint32_t milliseconds);
extern bool enable_sound;

void generate_sound(uint16_t frequency, uint8_t duty_cycle, TIM_HandleTypeDef htim1) {
	/* protect the function from invalid frequency input */
	if (frequency == 0 || !enable_sound) {
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
		return;
	}
	/* millisecond trigger period */
	uint32_t period = 72000 / frequency;
	/* setup PWM channel and tim1 autoloader */
	htim1.Instance->ARR = period - 1;
	htim1.Instance->CCR3 = period / 2;
	/* start the timer1 and buzzer */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void stop_sound(TIM_HandleTypeDef htim1) {
	/* stop the timer1 and buzzer */
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
}

void play_note(uint16_t note, uint32_t duration, uint8_t volume, TIM_HandleTypeDef htim1) {
	if (!enable_sound) return;
	generate_sound(note, volume, htim1);
	coast_asm_delay(duration);
}
