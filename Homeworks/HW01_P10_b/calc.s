	AREA myData, DATA
N EQU 4
	
	EXPORT __main
	AREA myCode, CODE, READONLY
	ENTRY
	
__main
	LDR r1, =N
	PUSH {r1}
	BL CALC	
	POP {r1}

HERE B HERE

CALC
	LDR r0, =0 ; r0 -> result
loop
	MUL r2, r1, r1
	ADD r0, r0, r2
	SUBS r1, r1, #1
	BNE loop
	BX LR
	
	END
