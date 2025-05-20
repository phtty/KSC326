; 上电显示0~9以及所有单点
BootScreen_Display:
	call	RGB_ModeSwitch

	call	Display_All
	call	Send_RGB_Data						; 全显1S
	btfss	Time_Flag,0
	goto	$-1
	bcf		Time_Flag,0

	call	Clear_All							; 开始数字全显阶段
	clrf	P_Temp+7
	bcf		Time_Flag,1

	call	AllDigit_Display
	call	Send_RGB_Data						; 全显数字0
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	1
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Red
	call	Send_RGB_Data						; 全显数字1
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	2
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Yellow
	call	Send_RGB_Data						; 全显数字2
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	3
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Green
	call	Send_RGB_Data						; 全显数字3
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	4
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Cyan
	call	Send_RGB_Data						; 全显数字4
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	5
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Blue
	call	Send_RGB_Data						; 全显数字5
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	6
	movwf	P_Temp+7
	call	AllDigit_Display
	call	SingleColor_Purple
	call	Send_RGB_Data						; 全显数字6
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	7
	movwf	P_Temp+7
	call	AllDigit_Display
	call	DigitalAlter_Horizon_Fixed
	call	Send_RGB_Data						; 全显数字7
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	8
	movwf	P_Temp+7
	call	AllDigit_Display
	call	DigitalAlter_Vertical_Fixed
	call	Send_RGB_Data						; 全显数字8
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	movlw	9
	movwf	P_Temp+7
	call	AllDigit_Display
	call	ChaosColor_Fixed
	call	Send_RGB_Data						; 全显数字9
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	call	SingleColor_White
	call	Undisplay_All
	call	SymbolPM_Display					; 显示PM点
	call	Send_RGB_Data
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	call	SymbolPM_Clear
	call	SymbolCOL_Display					; 显示COL点
	call	Send_RGB_Data
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	call	SymbolCOL_Clear
	call	SymbolAL_Display					; 显示AL点
	call	SymbolTEMP_Display					; 显示TEMP点
	call	Send_RGB_Data
	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	call	SymbolAL_Clear						; 清除AL点
	call	SymbolTEMP_Clear					; 清除TEMP点
	movlw	8
	movwf	T1_Counter
	movlw	2
	movwf	T0_Counter
	bcf		Time_Flag,3
	bcf		Time_Flag,0
	bcf		IOCF,0

	call	Key_Beep
	return


; 所有的8字都根据P_Temp+7值显示数字
AllDigit_Display:
	clrf	R_X
	movf	P_Temp+7,W
	call	Digit_Num_Display

	movlw	1
	movwf	R_X
	movf	P_Temp+7,W
	call	Digit_Num_Display

	movlw	2
	movwf	R_X
	movf	P_Temp+7,W
	call	Digit_Num_Display

	movlw	3
	movwf	R_X
	movf	P_Temp+7,W
	call	Digit_Num_Display
	return
