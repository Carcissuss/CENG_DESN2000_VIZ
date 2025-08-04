/*
 * buzzer.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Admin
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"
void generate_sound(uint16_t frequency, uint8_t duty_cycle);
void stop_sound();
void play_note(uint16_t note, uint32_t duration, uint8_t volume);

#endif /* INC_BUZZER_H_ */
