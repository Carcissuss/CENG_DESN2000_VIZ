/*
 * ldr.h
 *
 *  Created on: Aug 14, 2025
 *      Author: zhaoj
 */

#ifndef INC_LDR_H_
#define INC_LDR_H_

// 可调参数
#define ADC_MAX    4095.0f
#define DUTY_MIN   5.0f     // 不让全灭，可按需改到0
#define DUTY_MAX   95.0f    // 不让全亮，可按需改到100
#define ALPHA      0.05f    // IIR低通(0~1，越小越稳)
#define SLEW_STEP  1.0f     // 每次最大变化百分比(越小越顺滑)
#define INVERT     1        // 1: 光强→LED更暗；0: 光强→LED更亮


#endif /* INC_LDR_H_ */
