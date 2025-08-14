/*
 * coast.h
 *
 *  Created on: Aug 4, 2025
 *      Author: Vageesh Jha
 */
#include "main.h"
#include "stdbool.h"
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
void homePage();
void OPTpage();
void updateOpt(uint32_t ldrValue);
