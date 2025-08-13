/*
 * date.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

#include "date.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

static int isLeapYear(int year);
static int getDaysInMonth(int year, int month);

// set default time
DateTime setDefaultTime() {
	DateTime defaultTime = {2025, 8, 3, "Sun", 0, 0, 0};
	return defaultTime;
}

// transfer time to string
void timeToStr(char *strTime, DateTime time) {
	sprintf(strTime, "%04ld-%02ld-%02ld %s %02ld:%02ld:%02ld",
			time.year, time.month, time.day,
			time.weekday, time.hour, time.minute, time.second);
}

// transfer string to time
void strToDateTime(const char *str, DateTime *dt) {

    sscanf(str, "%lu-%lu-%lu %s %lu:%lu:%lu",
    		&dt->year, &dt->month, &dt->day,
			dt->weekday, &dt->hour, &dt->minute, &dt->second);
}

// update the time
DateTime updateTime(DateTime dt, uint32_t elapsedSeconds) {
    uint32_t totalSeconds = dt.second + elapsedSeconds;
    dt.second = totalSeconds % 60;

    uint32_t totalMinutes = dt.minute + totalSeconds / 60;
    dt.minute = totalMinutes % 60;

    uint32_t totalHours = dt.hour + totalMinutes / 60;
    dt.hour = totalHours % 24;

    uint32_t daysToAdd = totalHours / 24;

    while (daysToAdd > 0) {
        if (dt.day < getDaysInMonth(dt.year, dt.month)) {
            dt.day++;
            daysToAdd--;
        } else {
            dt.day = 1;
            dt.month++;
            if (dt.month > 12) {
                dt.month = 1;
                dt.year++;
            }
        }
    }

    uint32_t weekdayIndex = 0;
    while (strcmp(weekdays[weekdayIndex], dt.weekday) != 0 && weekdayIndex < 7) {
        weekdayIndex++;
    }
    weekdayIndex = (weekdayIndex + daysToAdd) % 7;
    strcpy(dt.weekday, weekdays[weekdayIndex]);

    return dt;
}

// set alarm by the given time
Alarm setAlarm(Alarm alarm, uint32_t hour, uint32_t minute, uint32_t second) {
	alarm.hour = hour;
	alarm.minute = minute;
	alarm.second = second;
	alarm.is_enable = true;
	return alarm;
}

Alarm changeAlarmState(Alarm alarm) {
	alarm.is_enable = !alarm.is_enable;
	return alarm;
}

bool isSameTime(Alarm alarm, DateTime dt) {
	return (alarm.hour == dt.hour && alarm.minute == dt.minute && alarm.second == dt.second);
}

bool isFinishCountDown(Alarm countdown) {
	return (countdown.hour == 0 && countdown.minute == 0 && countdown.second == 0);
}

bool isAlarmEnabled(Alarm alarm) {
	return alarm.is_enable;
}

// Helper function to determine if it's a leap year
static int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

// Get the number of days in a given month of a given year
static int getDaysInMonth(int year, int month) {
    switch(month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            if (isLeapYear(year))
                return 29;
            else
                return 28;
        default:
            return 30;
    }
}

