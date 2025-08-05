/*
 * asm.s
 *
 *  Created on: Aug 2, 2025
 *      Author: Zhaoji Gou
 */

.global coast_asm_delay

/*
void coast_delay(uint32_t num) {
	uint32_t i = 0;
	while (i < num ){
		uint32_t j = 0;
		while(j < 10000) {
			j++;
		}
		i++;
	}
}
*/
coast_asm_delay:
	PUSH {r4}	// 1 + 1 = 2
	PUSH {r5} 	// 2

	// uint32_t i = 0
	MOV r4, #0	// 1
	LDR r1, =11999	// 2
loop1_start:
	CMP r4, r0	// 1
	BHS loop1_end // 1 + P

	MOV r5, #0	// 1

loop2_start:
	// ldr slower but has 32-bits, mov faster only 8-bit
	CMP r5, r1		// 1
	BHS loop2_end	// 1 + P

	// j++
	ADD r5, r5, #1 // 1
	b loop2_start  // 1 + P
loop2_end:
	// i++
	ADD r4, r4, #1	// 1
	b loop1_start  	// 1 + P

loop1_end:
	pop {r5}	// 2
	pop {r4}	// 2

	BX LR 		// 1 + P


/* Summary:
	Fixed Cycles: Push + PUSH + POP + POP + MOV + LDR + BX
	2 + 2 + 2 + 2 + 1 + 2 + 3 = 14 cycles

	Loop2 Cycles: CMP + BHS(Not) + ADD + B = 1+1+1+3 =6
		Last loop: CMP + BHS(branch) = 1+3 =4
	Loop2 := (11999 × 6) + 4 = 71,994 + 4 = 71,998 cycles

	if r0 = 1
	Loop1 :=  1* loop2 = 71998
	( CMP + BHS + MOV + loop2 + ADD + b ) * 1 = 1 + 1 + 1+ 71998 + 1 + 3 = 72005
	exit loop1:
		CMP + BHS = 4
	total loop1:
		4 + 72005 = 72009

	total cycles = 72009 + 14 = 72023
	time = 72021 / 72MHz = 1.0003 ms

	Notes: the larger the r0, the closer the time to a integer ms. The decimal part would be smaller and smaller.

*/



