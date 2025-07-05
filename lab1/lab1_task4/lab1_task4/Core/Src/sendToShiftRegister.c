#include "main.h"
#include "sendToShiftRegister.h"

void init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		GPIO_InitStruct.Pin = SER_PIN;
		HAL_GPIO_Init(SER_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SRCLK_PIN;
		HAL_GPIO_Init(SRCLK_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = RCLK_PIN;
		HAL_GPIO_Init(RCLK_PORT, &GPIO_InitStruct);
}

void sendToShiftRegister(uint16_t pattern) {
	for (int i = 0; i <= 15; i++) {
			HAL_GPIO_WritePin(SER_PORT, SER_PIN, (pattern & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

			HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);


		}
		HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_RESET);

}
