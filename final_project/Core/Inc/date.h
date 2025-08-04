/*
 * date.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#ifndef INC_DATE_H_
#define INC_DATE_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SUN = 0,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT
} Weekday;

typedef struct {
	uint32_t year;
	uint32_t month;
	uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    Weekday weekday;
} DateTime;

typedef struct {
	bool start;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
} Alarm;



DateTime setDefaultTime();
void timeToStr(char *strTime, DateTime time);
void strToDateTime(const char *str, DateTime *dt);
//DateTime updateTime(DateTime dt);
DateTime updateTime(DateTime dt, uint32_t elapsedSeconds);
Alarm setAlarm(Alarm alarm, uint32_t hour, uint32_t minute, uint32_t second);
Alarm changeAlarmState(Alarm alarm);
bool isSameTime(Alarm alarm, DateTime dt);
bool isFinishCountDown(Alarm countdown);
bool isAlarmEnabled(Alarm alarm);
//DateTime strToTime(char *strTime);



#endif /* INC_DATE_H_ */
