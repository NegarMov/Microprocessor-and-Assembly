	AREA myData, DATA
var0 EQU 0
var1 EQU -5
var2 EQU 10
var3 EQU 1
	
	EXPORT __main
	AREA myCode, CODE, READONLY
	ENTRY
	
__main
	LDR r0, =var0
	LDR r1, =var1
	LDR r2, =var2
	LDR r3, =var3

	CMP r1, #0
	BLT OVER1
	CMP r0, #0
	BNE OVER1
	ADD r2, r2, #1
	B OVER2
OVER1
	CMP r2, #10
	BNE OVER2
	MOV r2, #0
	ADD r3, r3, #1
OVER2
	
HERE B HERE
	END
