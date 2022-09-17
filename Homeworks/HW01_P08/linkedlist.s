	AREA myData, DATA
HEAD EQU 0x20000000
	
	EXPORT __main
	AREA myCode, CODE, READONLY
	ENTRY
	
__main
	LDR r4, =HEAD
	MOV r1, r4
	BL REVERSE
	
HERE B HERE

REVERSE ; input -> r1 (a pointer to the current node), returned value -> r0
	CMP r1, #0
	BNE OVER1
	MOV r0, r1
	BX LR
OVER1
	LDR r2, [r1] ; data
	LDR r3, [r1, #4] ; next
	CMP r3, #0
	BNE OVER2
	MOV r0, r1
	BX LR
OVER2
	PUSH {r1-r3, LR}
	MOV r1, r3 ; call the function again with the next node as argument
	BL REVERSE
	POP {r1-r3, LR}
	STR r1, [r3, #4]
	MOV r4, #0
	STR r4, [r1, #4]
	BX LR
	END
