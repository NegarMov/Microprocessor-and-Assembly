	AREA myData, DATA
X EQU 8
Y EQU 6
	
	EXPORT __main
	AREA myCode, CODE, READONLY
	ENTRY
	
__main
	LDR r0, =0  ; r0 -> lcm
	LDR r1, =X	; r1 -> a, gcd
	LDR r2, =Y  ; r2 -> b
	MUL r4, r1, r2
	
while
	MOV r3, r2
loop
	SUB r1, r1, r2
	CMP r1, r2
	BHS loop
	MOV r2, r1
	MOV r1, r3
	CMP r2, #0
	BNE while
	
while2
	SUBS r4, r4, r1
	ADD r0, r0, #1
	BNE while2
	
HERE B HERE
	END
