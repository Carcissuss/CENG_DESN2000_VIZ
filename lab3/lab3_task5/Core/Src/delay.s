.global delay

delay:
	LDR R1, =9000000 // 72Mhz / 8
	MUL R1, R0, R1	// frequency times the input seconds to get loop number
loop:
	SUB R1, R1, #1
	CMP R1, #0
	NOP
	BNE loop
	BX LR
