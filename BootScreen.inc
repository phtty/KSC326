; 上电显示0~9以及所有单点
BootScreen_Display:
	clrf	P_Temp+7
	bcf		Time_Flag,1
Number_Display_Loop:
	call	AllDigit_Display
	call	Send_RGB_Data						; 显示0~9数字

	btfss	Time_Flag,1
	goto	$-1
	bcf		Time_Flag,1

	incf	P_Temp+7,F
	movlw	10
	subwf	P_Temp+7,W
	btfss	C
	goto	Number_Display_Loop

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
	call	SymbolAL_Clear						; 显示AL点
	call	SymbolTEMP_Clear					; 显示TEMP点
	movlw	8
	movwf	T1_Counter
	movlw	2
	movwf	T0_Counter
	bcf		Time_Flag,3
	bcf		Time_Flag,0
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
