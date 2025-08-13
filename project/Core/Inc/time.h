#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdint.h>
#include <stdbool.h>
void timePage();
void updateTime(uint8_t row, uint8_t col);
void alarmPage();
void switchTimeFormat();
void switchAMPM();
void changeAlarmHour();
void changeAlarmMin();
void alarmConfirm();
void updateAlarm(uint8_t row, uint8_t col);

typedef struct {
	uint32_t second;
	uint32_t minute;
	uint32_t hour;
	bool enable;
} Countdown;

#endif /* INC_BUZZER_H_ */
