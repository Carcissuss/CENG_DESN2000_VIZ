/*
 * vibration.c
 *  Created on: Aug 5, 2025
 *      Author: Zhaoji, Ibrahim
 */

/* vibration.c */
#include "vibration.h"

/* ---- Module state ---- */
static volatile uint16_t steps_left = 0;
static volatile uint8_t  phase = 0;
static volatile uint16_t ms_since_last_step = 0;

/* Step speed: every 1 ms */
#define STEP_PERIOD_MS 2

/* 2-coil full-step sequence (matches your earlier working pattern) */
static inline void write_phase(uint8_t p) {
    switch (p & 3) {	// (p&3) for keeping value b/w 0-3
        case 0: // B+C
            HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, GPIO_PIN_RESET);
            break;
        case 1: // A+B
            HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, GPIO_PIN_RESET);
            break;
        case 2: // A+D
            HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, GPIO_PIN_SET);
            break;
        case 3: // C+D
            HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, GPIO_PIN_SET);
            break;
    }
}


/* for asynchronous functioanlity via TIM7 tick */
void vibration_call(uint16_t steps) {
    if (steps == 0) return;
    steps_left = steps;
    write_phase(phase);
    ms_since_last_step = 0;
}

/* stoping vibration */
void vibration_stop() {
    steps_left = 0;
    HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, GPIO_PIN_RESET);
}

/* TIM7 ISR - vibration ticker per 1 ms */
void vibration_tick_1ms() {
    if (steps_left == 0) return;

    ms_since_last_step++;
    if (ms_since_last_step < STEP_PERIOD_MS) return;
    ms_since_last_step = 0;

    // advance one step;
    phase = (phase + 1) & 3;
    write_phase(phase);

    if (--steps_left == 0) {
    	vibration_stop();
    }
}

void generate_vibration(void) {
//	for (int i = 0; i < 4; i++) {
//		HAL_GPIO_WritePin(coil_port[i], coil_pin[i], GPIO_PIN_RESET);
//	}
	// 1
	HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, 0);
	HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, 1);
	HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, 1);
	HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, 0);
	coast_asm_delay(1);

	// 2
	HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, 1);
	HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, 1);
	HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, 0);
	HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, 0);
	coast_asm_delay(1);

	// 3
	HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, 1);
	HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, 0);
	HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, 0);
	HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, 1);
	coast_asm_delay(1);
	// 4
	HAL_GPIO_WritePin(COILA_GPIO_Port, COILA_Pin, 0);
	HAL_GPIO_WritePin(COILB_GPIO_Port, COILB_Pin, 0);
	HAL_GPIO_WritePin(COILC_GPIO_Port, COILC_Pin, 1);
	HAL_GPIO_WritePin(COILD_GPIO_Port, COILD_Pin, 1);
	coast_asm_delay(1);
}
