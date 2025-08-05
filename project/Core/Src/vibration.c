/*
 * vibration.c
 *
 *  Created on: Aug 5, 2025
 *      Author: Zhaoji Gou
 */

#include "vibration.h"
//uint16_t coil_port[4] = {COILA_GPIO_Port, COILB_GPIO_Port, COILC_GPIO_Port, COILD_GPIO_Port};
//uint16_t coil_pin[4] = {COILA_Pin, COILB_Pin, COILC_Pin, COILD_Pin};
extern void coast_asm_delay(uint32_t milliseconds);

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
