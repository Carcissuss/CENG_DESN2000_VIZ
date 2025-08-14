#include "main.h"
#include "stdbool.h"
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
void homePage();
void OPTpage(uint32_t ldrValue);
void updateOpt(uint32_t ldrValue);
