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
	movwf	P_Temp
	movlw	High(Word_NumTable)
	movwf	PCLATH
	movf	P_Temp,W
	call	Word_NumTable
	goto	Digit_BitHandle



; 一些符号灯的定义
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
	clrf	P_Temp+2						; 位选byte的循环计数初始化
	clrf	P_Temp+4						; 总计数初始化

	bcf		GIE								; 发送时不响应中断，避免发送时序被中断干扰
SendByte_Loop:
	movlw	Low(DisSwtich)
	addwf	P_Temp+2,W						; 位选byte循环计数作为偏移，查相应的位选内存
	movwf	FSR1L
	movlw	High(DisSwtich)
	movwf	FSR1H
	movf	INDF1,W
	movwf	P_Temp+3						; 查出来的内存进行拷贝，在位选bit的8次循环中右移

	clrf	P_Temp+1
SendBit_Loop:
	bcf		C
	rrf		P_Temp+3,F
	btfsc	C
	goto	SendBit_SearchTable

	Grayscale_0								; 移位出0说明该灯灭，将发送缓冲区清0
	goto	SendBit_JugeOver
SendBit_SearchTable:
	Search_TickTable						; 移位出1说明该灯亮，查对应的时刻表放入发送缓冲区
SendBit_JugeOver:
	Send_RGB								; 发送1个灯的三通道灰度数据

	incf	P_Temp+4,F
	movlw	33
	subwf	P_Temp+4,W
	btfsc	C
	goto	SendingLoop_Over				; 当发送计数计到33时，发送完成，直接退出循环

	incf	P_Temp+1,F
	movlw	8
	subwf	P_Temp+1,W
	btfss	C
	goto	SendBit_Loop					; 发送位选bit的循环

	incf	P_Temp+2,F
	movlw	5
	subwf	P_Temp+2,W
	btfss	C
	goto	SendByte_Loop					; 发送位选byte的循环

SendingLoop_Over:
	bsf		GIE								; 发送完成重新开启总中断
	return


;======================================================================================


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


; 全显
Display_All:
	movlw	0xff
	movwf	DisSwtich
	movwf	DisSwtich+1
	movwf	DisSwtich+2
	movwf	DisSwtich+3
	movlw	0x01
	movwf	DisSwtich+4
	return

; 全清
Clear_All:
	clrf	DisSwtich
	clrf	DisSwtich+1
	clrf	DisSwtich+2
	clrf	DisSwtich+3
	clrf	DisSwtich+4
	return
