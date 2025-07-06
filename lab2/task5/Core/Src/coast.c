#include "main.h"
#include "coast.h"
#include <stdint.h>

#define PORTA_MODER   (*((volatile uint32_t *)0x48000000))
#define PORTA_OTYPE   (*((volatile uint32_t *)0x48000004))
#define PORTA_OSPEEDR (*((volatile uint32_t *)0x48000008))
#define PORTA_PUPDR   (*((volatile uint32_t *)0x4800000C))
#define PORTA_IDR     (*((volatile uint32_t *)0x48000010))
#define PORTA_ODR     (*((volatile uint32_t *)0x48000014))

#define RCC_AHBENR    (*((volatile uint32_t *)0x40021014))

#define PORTB_MODER   (*((volatile uint32_t *)0x48000400))
#define PORTB_OTYPE   (*((volatile uint32_t *)0x48000404))
#define PORTB_OSPEEDR (*((volatile uint32_t *)0x48000408))
#define PORTB_PUPDR   (*((volatile uint32_t *)0x4800040C))
#define PORTB_IDR     (*((volatile uint32_t *)0x48000410))
#define PORTB_ODR     (*((volatile uint32_t *)0x48000414))
int coast_loop_body(int counter) {
	/*if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1) {
		      if (counter > 0) counter--;
		      HAL_Delay(200);
		      //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);

		  }
		  else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == 1) {
		      if (counter < 15) counter++;
		      HAL_Delay(200);
		     // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		  }
		  ledDisplay(counter);
		  return counter;*/

		if ((PORTA_IDR &= 1<<1)) {
			if (counter > 0) counter--;
			coast_delay(200);
			//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);

		}
		else if ((PORTA_IDR &= 1<<4)) {
			if (counter < 15) counter++;
			coast_delay(200);
			// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		}
		ledDisplay(counter);
		return counter;
}
void ledDisplay (int value) {
	int LED_PINS[4] = {3, 5, 4, 10};
	for (int i = 0; i < 4; i++) {
		  if (value & (1 << (3 - i))) {
		  	//HAL_GPIO_WritePin(GPIOB, LED_PINS[i], GPIO_PIN_SET);
		  	PORTB_ODR |= 1<<LED_PINS[i];
		  } else {
		  	//HAL_GPIO_WritePin(GPIOB, LED_PINS[i], GPIO_PIN_RESET);
		  	PORTB_ODR &= ~(1<<LED_PINS[i]);
		  }
	}
}

void coast_mx_gpio_init(){

	/* GPIO Ports Clock Enable */
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOB_CLK_ENABLE();

	//enable clock for PORTA
	RCC_AHBENR |= (1<<17);

	//enable clock for PORTB
	RCC_AHBENR |= (1<<18);

	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

	PORTB_ODR &= ~(1<<5);

	/*Configure GPIO pins : PA1 PA4 */
	//GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
	//GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	//GPIO_InitStruct.Pull = GPIO_NOPULL;
	//HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//set PA1 | PA4 as input
	PORTA_MODER &= ~((0b11 << 2) | (0b11 << 8));

	//set PA1 | PA4 to have no pullup
	PORTA_PUPDR &= ~((0b11 << 2) | (0b11 << 8));

	/*Configure GPIO pins : PB10 PB3 PB4 PB5 */
	//GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	//GPIO_InitStruct.Pull = GPIO_NOPULL;

	//set PB10 | PB3 | PB4 | PB5 as output
	PORTB_MODER &= ~((0b11 << 20) | (0b11 << 6) | (0b11 << 8) | (0b11 << 10));
	PORTB_MODER |= ((0b01 << 20) | (0b01 << 6) | (0b01 << 8) | (0b01 << 10));

	//set PB10 | PB3 | PB4 | PB5 to be no pullup
	PORTB_PUPDR &= ~((0b11 << 20) | (0b11 << 6) | (0b11 << 8) | (0b11 << 10));

	//set PB speed low
	PORTB_OSPEEDR &= ~(0b11 << 10);
	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	//HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void coast_delay(uint32_t val){
	for(uint32_t i=0; i<val; i++){
		for(uint32_t j=0;j<10000;j++){

		}
	}
}
