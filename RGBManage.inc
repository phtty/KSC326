; RGB模式切换
RGB_Manage:

	return


; 横滚渐变
RGB_Horizon_Roll:
	clrf	P_Temp							; 初始化外层循环计数
	clrf	P_Temp+1						; 初始化内层循环计数
	clrf	P_Temp+2						; 初始化总偏移计数
	clrf	P_Temp+3						; 初始化初值
HorizonDigit_Loop:
	movlw	High(IncTable_HorizonRolling)
	movwf	PCLATH
	movf	P_Temp,W
	call	IncTable_HorizonRolling
	movwf	P_Temp+1

HorizonSeg_Loop:
	movlw	High(RGBTable_HorizonRolling)
	movwf	PCLATH
	movf	P_Temp+2,W
	call	RGBTable_HorizonRolling
	movwf	R_X

	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	P_Temp+3,W
	movwf	INDF0

	incf	P_Temp+2,F						; 递增总偏移

	decfsz	P_Temp+1,F
	goto	HorizonSeg_Loop					; 内层循环

	movlw	8
	subwf	RGB_Mode,W
	btfsc	Z
	goto	Fixed_Mode
	movlw	80
	goto	InitialData_CHG
Fixed_Mode:
	movlw	40
InitialData_CHG:
	addwf	P_Temp+3						; 根据是固定模式还是渐变模式选择每个8字的初值

	incf	P_Temp,F
	movlw	4
	subwf	P_Temp,W
	btfss	C
	goto	HorizonDigit_Loop				; 外层循环

	return
