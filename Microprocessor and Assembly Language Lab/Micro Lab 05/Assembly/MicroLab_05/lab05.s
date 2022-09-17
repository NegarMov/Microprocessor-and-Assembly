	AREA myData, DATA
PIO_PER  EQU 0x400E0E00
PIO_OER  EQU 0x400E0E10
PIO_SODR EQU 0x400E0E30
PIO_CODR EQU 0x400E0E34
PIO_ISR  EQU 0x400E104C
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	
	AREA myCode, CODE, READONLY
	EXPORT __main
	ENTRY

__main
	BL led_off
	BL button1_handler
	BL enable_pio
	MOV R1, #0
loop
	ADD R1, R1, #1
	BL led_on
	BL delay
	BL led_off
	B loop
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

enable_pio
	MOV R4, #2_111
	LDR R5, =PIO_PER
	STR R4, [R5]
	LDR R5, =PIO_OER
	STR R4, [R5]
	BX LR
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

led_on
	MOV R4, R1
	LDR R5, =PIO_SODR
	STR R4, [R5]
	BX LR
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		

led_off
	MOV R4, #2_111
	LDR R5, =PIO_CODR
	STR R4, [R5]
	BX LR
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

delay
	PUSH { LR }
	BL button2_handler
	POP { LR }
	MOV R4, #0
	LDR R5, =0x0040000
delay_loop
	ADD R4, R4, #1
	CMP R4, R5
	BNE delay_loop
	BX LR
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

button1_handler
	LDR R7, =PIO_ISR
	LDR R6, [R7]
	AND R7, R6, #2_01
	CMP R7, #2_01
	BNE button1_handler
	BX LR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

button2_handler
	LDR R7, =PIO_ISR
	LDR R6, [R7]
	AND R7, R6, #2_10
	CMP R7, #2_10
	BNE over2
	B __main
over2
	BX LR

	END