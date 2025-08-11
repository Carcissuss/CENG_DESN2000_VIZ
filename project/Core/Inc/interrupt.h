/*
 * interrupt.h
 *
 *  Created on: Aug 4, 2025
 *      Author: zhaoj
 */

#ifndef INC_INTERRUPT_H_
#define INC_INTERRUPT_H_
#include <stdint.h>
#include <stdbool.h>
void check_double_press(int button_index,
                         bool is_single_press,
                         bool is_double_press,
                         bool is_holding,
						 uint32_t decimal_second_count,
						 uint32_t double_press_interval,
						 uint32_t button_double_press_time[],
						 uint32_t button_holding_time[]);

void check_holding(int button_index,
                   bool is_single_press,
                   bool is_double_press,
                   bool is_holding,
                   uint32_t decimal_second_count,
                   uint32_t holding_bound,
                   uint32_t button_double_press_time[],
                   uint32_t button_holding_time[]);
#endif /* INC_INTERRUPT_H_ */
