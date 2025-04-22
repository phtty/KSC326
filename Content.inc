D_Char_0:		equ	11000000B ;	;0
D_Char_1:		equ	11001111B ;	;1
D_Char_2:		equ	10100100B ;	;2
D_Char_3:		equ	10000110B ;	;3	
D_Char_4:		equ	10001011B ;	;4
D_Char_5:		equ	10010010B ;	;5
D_Char_6:		equ	10010000B ;	;6
D_Char_7:		equ	11000111B ;	;7
D_Char_8:		equ	10000000B ;	;8
D_Char_9:		equ	10000010B ;	;9
D_Char_Null		equ	11111111B ;	;A
D_Char_Neg:		equ	10111111B ;	;-	;B	
D_Char_C:		equ	11110000B ;	;C	;C
D_Char_F:		equ	10110001B ;	;F	;D
D_Char_H:		equ	10001001B ;	;H	;E
D_Char_L:		equ	11111000B ;	;L	;F
D_Char_o:		equ	10011100B
D_Char_N:		equ	11000001B
D_Char_S:		equ	10010010B
D_Char_d:		equ	10001100B
D_Char_p:		equ	10100001B
D_Char_A:		equ	10000001B
D_Char_r:		equ	10111101B

LED_CODE1:	MACRO	
	bsf	PORTF,5
	bsf	PORTF,5
	bcf	PORTF,5
	ENDM

LED_CODE0:	MACRO
	bsf	PORTF,5
	bcf	PORTF,5
	bcf	PORTF,5
	ENDM

