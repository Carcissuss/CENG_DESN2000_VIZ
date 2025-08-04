#include "coast.h"

uint8_t scan_keypad(){
	uint8_t key_map[4][4] = {
			{'1','2','3','A'},
			{'4','5','6','B'},
			{'7','8','9','C'},
			{'*','0','#','D'}
	};
	uint16_t Row_Pin[4] = {KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin, KEY_ROW4_Pin};
	uint16_t Col_Pin[4] = {KEY_COL1_Pin, KEY_COL2_Pin, KEY_COL3_Pin, KEY_COL4_Pin};
	uint8_t ret = 0;
	for (int c=0; c<4; c++) {
		HAL_GPIO_WritePin(KEY_COL_Port, Col_Pin[c], 1);
		for (int r=0; r<4; r++) {
			if (HAL_GPIO_ReadPin(KEY_ROW_Port, Row_Pin[r]) == 1) {
				// debounce check
				HAL_Delay(100);
				if (HAL_GPIO_ReadPin(KEY_ROW_Port, Row_Pin[r]) == 1) {
					ret = key_map[r][c];
				}
			}
		}
		HAL_GPIO_WritePin(KEY_COL_Port, Col_Pin[c], 0);
	}
	return ret;
}
