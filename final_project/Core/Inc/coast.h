#ifndef INC_COAST_H_
#define INC_COAST_H_

#include "stm32f3xx_hal.h"

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
//...

#define KEY_ROW_Port GPIOB
#define KEY_ROW1_Pin GPIO_PIN_11
#define KEY_ROW2_Pin GPIO_PIN_12
#define KEY_ROW3_Pin GPIO_PIN_13
#define KEY_ROW4_Pin GPIO_PIN_14

#define KEY_COL_Port GPIOA
#define KEY_COL1_Pin GPIO_PIN_8
#define KEY_COL2_Pin GPIO_PIN_9
#define KEY_COL3_Pin GPIO_PIN_10
#define KEY_COL4_Pin GPIO_PIN_11

//...

// call this at the end of MX_GPIO_Init() in main.c
void coast_gpio_init();
void coast_gpio_sw_init();
void coast_gpio_lcd_init();
void coast_gpio_keypad_init();

// call this inside while(1) in main.c
uint32_t coast_loop_body(uint8_t enable);

#endif /* INC_COAST_H_ */
