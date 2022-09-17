	AREA myData, DATA
CONST EQU 5
ASTART EQU 0x4000DD0
AEND EQU 0x4000DFF
	
	EXPORT __main
	AREA myCode, CODE, READONLY
	ENTRY
	
__main
	LDR r0, =CONST
	LDR r1, =0
	LDR r2, =ASTART
	LDR r3, =AEND
	
loop
	LDRB r4, [r2]
	CMP r4, r0
	BLS continue
	ADD r1, #1
continue
	ADD r2, #1
	CMP r2, r3
	BLS loop
	
HERE B HERE
	END
