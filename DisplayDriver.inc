; 数字显示，R_X==第几个Digit、W==要显示的数字
Digit_Num_Display:
	movwf	P_Temp
	movlw	High(Digit_NumTable)
	movwf	PCLATH
	movf	P_Temp,W
	call	Digit_NumTable

Digit_BitHandle:
	movwf	P_Temp
	btfsc	P_Temp,0						; 将8字中的亮点置1
	call	DigitSet_SegA
	btfsc	P_Temp,1
	call	DigitSet_SegB
	btfsc	P_Temp,2
	call	DigitSet_SegC
	btfsc	P_Temp,3
	call	DigitSet_SegD
	btfsc	P_Temp,4
	call	DigitSet_SegE
	btfsc	P_Temp,5
	call	DigitSet_SegF
	btfsc	P_Temp,6
	call	DigitSet_SegG

	btfss	P_Temp,0						; 将8字中的灭点置0
	call	DigitClr_SegA
	btfss	P_Temp,1
	call	DigitClr_SegB
	btfss	P_Temp,2
	call	DigitClr_SegC
	btfss	P_Temp,3
	call	DigitClr_SegD
	btfss	P_Temp,4
	call	DigitClr_SegE
	btfss	P_Temp,5
	call	DigitClr_SegF
	btfss	P_Temp,6
	call	DigitClr_SegG

	return


; 字符显示，R_X==第几个Digit、P_Temp==要显示的字符
Digit_Word_Display:
	movlw	High(Word_NumTable)
	movwf	PCLATH
	movf	P_Temp,W
	call	Word_NumTable
	goto	Digit_BitHandle




SymbolPM_Display:							; 亮PM点
	movlw	0x01
	iorwf	DisSwtich+0,F
	return

SymbolPM_Clear:								; 灭PM点
	movlw	0xfe
	andwf	DisSwtich+0,F
	return


SymbolAL_Display:							; 亮AL点
	movlw	0x04
	iorwf	DisSwtich+0,F
	return

SymbolAL_Clear:								; 灭AL点
	movlw	0xfb
	andwf	DisSwtich+0,F
	return


SymbolTEMP_Display:							; 亮TEMP点
	movlw	0x02
	iorwf	DisSwtich+2,F
	return

SymbolTEMP_Clear:							; 灭TEMP点
	movlw	0xfd
	andwf	DisSwtich+2,F
	return


SymbolCOL_Display:							; 亮COL点
	movlw	0x02
	iorwf	DisSwtich+0,F
	movlw	0x04
	iorwf	DisSwtich+2,F
	return

SymbolCOL_Clear:							; 灭COL点
	movlw	0xfd
	andwf	DisSwtich+0,F
	movlw	0xfb
	andwf	DisSwtich+2,F
	return



; 发送33个灯的RGB数据
Send_RGB_Data:
	call	Dot_Clear						; 发送前先将灭的灯的灰度数据清零

	bcf		GIE								; 发送时不响应中断，避免发送时序被中断干扰
	movlw	32
	movwf	R_X
Send_GRB_Loop:
	Search_TickTable						; 使用宏定义节约call调用的时间
	Send_RGB
	decfsz	R_X,F
	goto	Send_GRB_Loop

	bsf		GIE								; 发送完成重新开启总中断
	return



; 发送所有RGB灯数据前先将不亮的灯的GRB_Tick设置为时刻表中灰度为0的时刻表偏移
Dot_Clear:
	movf	DisSwtich+0,W
	movwf	P_Temp+0
	movf	DisSwtich+1,W
	movwf	P_Temp+1
	movf	DisSwtich+2,W
	movwf	P_Temp+2
	movf	DisSwtich+3,W
	movwf	P_Temp+3
	movf	DisSwtich+4,W
	movwf	P_Temp+4						; 将DisSwtich存入P_Temp中以便循环右移

	bcf		C
	clrf	R_X
Dot_ClearLoop:
	rrf		P_Temp+4,F
	rrf		P_Temp+3,F
	rrf		P_Temp+2,F
	rrf		P_Temp+1,F
	rrf		P_Temp+0,F
	btfsc	C
	goto	Dot_ClearLoop_Juge				; 移位出1时不作操作
	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movlw	242
	movwf	INDF0							; 移位出0，则将GRB_Tick,R_X置为242
Dot_ClearLoop_Juge:
	incf	R_X,F
	movlw	32
	subwf	R_X,W
	btfss	C
	goto	Dot_ClearLoop

	return




DigitSet_SegA:
	movlw	High(SegA_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegA_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegA
	goto	Digit1Set_SegA
	goto	Digit2Set_SegA
	goto	Digit3Set_SegA

Digit0Set_SegA:
	bsf		DisSwtich+3,3
	return
Digit1Set_SegA:
	bsf		DisSwtich+3,6
	return
Digit2Set_SegA:
	bsf		DisSwtich+0,4
	return
Digit3Set_SegA:
	bsf		DisSwtich+0,7
	return


DigitSet_SegB:
	movlw	High(SegB_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegB_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegB
	goto	Digit1Set_SegB
	goto	Digit2Set_SegB
	goto	Digit3Set_SegB

Digit0Set_SegB:
	bsf		DisSwtich+3,4
	return
Digit1Set_SegB:
	bsf		DisSwtich+3,7
	return
Digit2Set_SegB:
	bsf		DisSwtich+0,5
	return
Digit3Set_SegB:
	bsf		DisSwtich+1,0
	return


DigitSet_SegC:
	movlw	High(SegC_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegC_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegC
	goto	Digit1Set_SegC
	goto	Digit2Set_SegC
	goto	Digit3Set_SegC

Digit0Set_SegC:
	bsf		DisSwtich+2,6
	return
Digit1Set_SegC:
	bsf		DisSwtich+2,3
	return
Digit2Set_SegC:
	bsf		DisSwtich+1,6
	return
Digit3Set_SegC:
	bsf		DisSwtich+1,2
	return


DigitSet_SegD:
	movlw	High(SegD_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegD_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegD
	goto	Digit1Set_SegD
	goto	Digit2Set_SegD
	goto	Digit3Set_SegD

Digit0Set_SegD:
	bsf		DisSwtich+2,7
	return
Digit1Set_SegD:
	bsf		DisSwtich+2,4
	return
Digit2Set_SegD:
	bsf		DisSwtich+1,7
	return
Digit3Set_SegD:
	bsf		DisSwtich+1,3
	return


DigitSet_SegE:
	movlw	High(SegE_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegE_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegE
	goto	Digit1Set_SegE
	goto	Digit2Set_SegE
	goto	Digit3Set_SegE

Digit0Set_SegE:
	bsf		DisSwtich+3,0
	return
Digit1Set_SegE:
	bsf		DisSwtich+2,5
	return
Digit2Set_SegE:
	bsf		DisSwtich+2,0
	return
Digit3Set_SegE:
	bsf		DisSwtich+1,4
	return


DigitSet_SegF:
	movlw	High(SegF_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegF_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegF
	goto	Digit1Set_SegF
	goto	Digit2Set_SegF
	goto	Digit3Set_SegF

Digit0Set_SegF:
	bsf		DisSwtich+3,2
	return
Digit1Set_SegF:
	bsf		DisSwtich+3,5
	return
Digit2Set_SegF:
	bsf		DisSwtich+0,3
	return
Digit3Set_SegF:
	bsf		DisSwtich+0,6
	return


DigitSet_SegG:
	movlw	High(SegG_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegG_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegG
	goto	Digit1Set_SegG
	goto	Digit2Set_SegG
	goto	Digit3Set_SegG

Digit0Set_SegG:
	bsf		DisSwtich+3,1
	return
Digit1Set_SegG:
	bsf		DisSwtich+4,0
	return
Digit2Set_SegG:
	bsf		DisSwtich+1,5
	return
Digit3Set_SegG:
	bsf		DisSwtich+1,1
	return




DigitClr_SegA:
	movlw	High(SegAClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegAClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegA
	goto	Digit1Clr_SegA
	goto	Digit2Clr_SegA
	goto	Digit3Clr_SegA

Digit0Clr_SegA:
	bcf		DisSwtich+3,3
	return
Digit1Clr_SegA:
	bcf		DisSwtich+3,6
	return
Digit2Clr_SegA:
	bcf		DisSwtich+0,4
	return
Digit3Clr_SegA:
	bcf		DisSwtich+0,7
	return


DigitClr_SegB:
	movlw	High(SegBClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegBClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegB
	goto	Digit1Clr_SegB
	goto	Digit2Clr_SegB
	goto	Digit3Clr_SegB

Digit0Clr_SegB:
	bcf		DisSwtich+3,4
	return
Digit1Clr_SegB:
	bcf		DisSwtich+3,7
	return
Digit2Clr_SegB:
	bcf		DisSwtich+0,5
	return
Digit3Clr_SegB:
	bcf		DisSwtich+1,0
	return


DigitClr_SegC:
	movlw	High(SegCClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegCClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegC
	goto	Digit1Clr_SegC
	goto	Digit2Clr_SegC
	goto	Digit3Clr_SegC

Digit0Clr_SegC:
	bcf		DisSwtich+2,6
	return
Digit1Clr_SegC:
	bcf		DisSwtich+2,3
	return
Digit2Clr_SegC:
	bcf		DisSwtich+1,6
	return
Digit3Clr_SegC:
	bcf		DisSwtich+1,2
	return


DigitClr_SegD:
	movlw	High(SegDClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegDClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegD
	goto	Digit1Clr_SegD
	goto	Digit2Clr_SegD
	goto	Digit3Clr_SegD

Digit0Clr_SegD:
	bcf		DisSwtich+2,7
	return
Digit1Clr_SegD:
	bcf		DisSwtich+2,4
	return
Digit2Clr_SegD:
	bcf		DisSwtich+1,7
	return
Digit3Clr_SegD:
	bcf		DisSwtich+1,3
	return


DigitClr_SegE:
	movlw	High(SegEClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegEClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegE
	goto	Digit1Clr_SegE
	goto	Digit2Clr_SegE
	goto	Digit3Clr_SegE

Digit0Clr_SegE:
	bcf		DisSwtich+3,0
	return
Digit1Clr_SegE:
	bcf		DisSwtich+2,5
	return
Digit2Clr_SegE:
	bcf		DisSwtich+2,0
	return
Digit3Clr_SegE:
	bcf		DisSwtich+1,4
	return


DigitClr_SegF:
	movlw	High(SegFClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegFClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegF
	goto	Digit1Clr_SegF
	goto	Digit2Clr_SegF
	goto	Digit3Clr_SegF

Digit0Clr_SegF:
	bcf		DisSwtich+3,2
	return
Digit1Clr_SegF:
	bcf		DisSwtich+3,5
	return
Digit2Clr_SegF:
	bcf		DisSwtich+0,3
	return
Digit3Clr_SegF:
	bcf		DisSwtich+0,6
	return


DigitClr_SegG:
	movlw	High(SegGClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegGClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegG
	goto	Digit1Clr_SegG
	goto	Digit2Clr_SegG
	goto	Digit3Clr_SegG

Digit0Clr_SegG:
	bcf		DisSwtich+3,1
	return
Digit1Clr_SegG:
	bcf		DisSwtich+4,0
	return
Digit2Clr_SegG:
	bcf		DisSwtich+1,5
	return
Digit3Clr_SegG:
	bcf		DisSwtich+1,1
	return




Digit_NumTable:
	addwf	PCL,F
	retlw	D_Char_0
	retlw	D_Char_1
	retlw	D_Char_2
	retlw	D_Char_3
	retlw	D_Char_4
	retlw	D_Char_5
	retlw	D_Char_6
	retlw	D_Char_7
	retlw	D_Char_8
	retlw	D_Char_9
	retlw	D_Char_Null


Word_NumTable:
	addwf	PCL,F
	retlw	D_Char_C
	retlw	D_Char_F
	retlw	D_Char_A
	retlw	D_Char_o
	retlw	D_Char_n
	retlw	D_Char_Neg
	retlw	D_Char_d
	retlw	D_Char_p
	retlw	D_Char_H
	retlw	D_Char_r
