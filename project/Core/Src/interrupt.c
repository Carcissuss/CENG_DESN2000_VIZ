/*
 * interrupt.c
 *
 *  Created on: Aug 4, 2025
 *      Author: zhaoj
 */
#include "interrupt.h"
void check_double_press(int button_index,
                         bool is_single_press,
                         bool is_double_press,
                         bool is_holding,
						 uint32_t decimal_second_count,
						 uint32_t double_press_interval,
						 uint32_t button_double_press_time[],
						 uint32_t button_holding_time[])
{
    if (is_single_press == true &&
        is_double_press == false &&
        (decimal_second_count - button_double_press_time[button_index]) <= double_press_interval) {
        is_double_press = true;
        is_single_press = false;
        is_holding = false;

    } else {
        is_single_press = true;
        is_double_press = false;
        is_holding = false;
    }

    button_holding_time[button_index] = decimal_second_count;
}

void check_holding(int button_index,
                   bool is_single_press,
                   bool is_double_press,
                   bool is_holding,
                   uint32_t decimal_second_count,
                   uint32_t holding_bound,
                   uint32_t button_double_press_time,
                   uint32_t button_holding_time)
{
    if ((decimal_second_count - button_holding_time[button_index]) >= holding_bound) {
        is_holding = true;
        is_double_press = false;
        is_single_press = false;
    } else {
        button_double_press_time[button_index] = decimal_second_count;
    }
}
