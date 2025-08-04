/*
 * buzzer.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include "main.h"
extern void coast_asm_delay(uint32_t milliseconds);
void generate_sound(uint16_t frequency, uint8_t duty_cycle) {
	/* protect the function from invalid frequency input */
	if (frequency == 0) return;
	/* millisecond trigger period */
	uint32_t period = 72000 / frequency;
	/* setup PWM channel and tim1 autoloader */
	TIM1->ARR = period - 1;
	TIM1->CCR1 = (period * duty_cycle) / 100;
	/* start the timer1 and buzzer */
	TIM1->CCER |= TIM_CCER_CC1E;
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1 |= TIM_CR1_CEN;
}

void stop_sound() {
	/* stop the timer1 and buzzer */
	TIM1->CCER &= ~TIM_CCER_CC1E;
	TIM1->BDTR &= ~TIM_BDTR_MOE;
	TIM1->CR1 &= ~TIM_CR1_CEN;
}

void play_note(uint16_t note, uint32_t duration, uint8_t volume) {
	generate_sound(note, volume);
	coast_asm_delay(duration);
    Stop_Sound();
}
