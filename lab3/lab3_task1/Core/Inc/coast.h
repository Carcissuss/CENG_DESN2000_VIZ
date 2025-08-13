#ifndef INC_COAST_H_
#define INC_COAST_H_

#include "main.h"

#define LCD_D_Port GPIOC
#define LCD_D4_Pin GPIO_PIN_8
#define LCD_D5_Pin GPIO_PIN_9
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D7_Pin GPIO_PIN_11
//...
#define LCD_E_Port GPIOD
#define LCD_E_Pin GPIO_PIN_2

#define LCD_RS_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_15

#define LCD_RW_Port GPIOC
#define LCD_RW_Pin GPIO_PIN_6
void coast_gpio_init();
void coast_gpio_lcd_init();
void coast_loop_body(int16_t counter);
#endif /* INC_COAST_H_ */
