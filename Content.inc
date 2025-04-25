D_Char_0:		equ	0x3f ;	0
D_Char_1:		equ	0x06 ;	1
D_Char_2:		equ	0x5b ;	2
D_Char_3:		equ	0x4f ;	3	
D_Char_4:		equ	0x86 ;	4
D_Char_5:		equ	0x6d ;	5
D_Char_6:		equ	0x7d ;	6
D_Char_7:		equ	0x03 ;	7
D_Char_8:		equ	0x7f ;	8
D_Char_9:		equ	0x6f ;	9
D_Char_Null		equ	0x00 ;	Null
D_Char_Neg:		equ	0x40 ;	-	
D_Char_C:		equ	0x39 ;	C
D_Char_F:		equ	0x71 ;	F
D_Char_H:		equ	0x76 ;	H
D_Char_r:		equ	0x50 ;	r
D_Char_o:		equ	0x5c ;	o
D_Char_n:		equ	0x54 ;	n
D_Char_d:		equ	0x5e ;	d
D_Char_p:		equ	0x73 ;	p
D_Char_A:		equ	0x77 ;	A




SEND_CODE1:		MACRO	
	bsf	PORTF,5
	bsf	PORTF,5
	bcf	PORTF,5
	ENDM

SEND_CODE0:		MACRO
	bsf	PORTF,5
	bcf	PORTF,5
	bcf	PORTF,5
	ENDM

