	org		0a80H
Temper_Table:
	db	0x14
	db	0x14
	db	0x14
	db	0x15
	db	0x16
	db	0x17
	db	0x17
	db	0x18
	db	0x18
	db	0x1A
	db	0x19
	db	0x1B
	db	0x1B
	db	0x1C
	db	0x1D
	db	0x1D
	db	0x1E
	db	0x1E
	db	0x1F
	db	0x20
	db	0x20
	db	0x21
	db	0x21
	db	0x22
	db	0x22
	db	0x23
	db	0x24
	db	0x23
	db	0x25
	db	0x24
	db	0x25
	db	0x25
	db	0x26
	db	0x26
	db	0x26
	db	0x26
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x27
	db	0x26
	db	0x26
	db	0x26
	db	0x26
	db	0x25
	db	0x26
	db	0x24
	db	0x25
	db	0x24
	db	0x24
	db	0x23
	db	0x23
	db	0x23
	db	0x22
	db	0x21
	db	0x22
	db	0x20
	db	0x21
	db	0x1F
	db	0x20
	db	0x1E
	db	0x1F
	db	0x1D
	db	0x1E
	db	0x1C
	db	0x1D
	db	0x1B
	db	0x1B
	db	0x1B
	db	0x1A
	db	0x1A
	db	0x19
	db	0x18
	db	0x18
	db	0x18
	db	0x17
	db	0x16
	db	0x16
	db	0x16
	db	0x15
	db	0x15

HighLightCompen_Table:
	addwf	PCL,F
	retlw	1
	retlw	2
	retlw	3
	retlw	4
	retlw	5
	retlw	6
	retlw	7
	retlw	8
	retlw	9
	retlw	10
	retlw	12

;LowLightCompen_Table:
;	addwf	PCL,F
;	retlw	1
