; 时间显示
Time_Display:
	call	TimeMin_Display
	call	TimeHour_Display

; 分钟显示
TimeMin_Display:
	movf	R_Time_Min,W
	call	Hex2BCD
	movf	P_Temp5,W
	movwf	P_Temp

	movlw	2
	movwf	R_X
	call	Digit_Num_Display				; 显示分钟个位

	movf	P_Temp6,W
	movwf	P_Temp
	movlw	3
	movwf	R_X
	call	Digit_Num_Display				; 显示分钟个位
	return

; 小时显示
TimeHour_Display:
	btfss	Clock_Flag,0
	goto	L_24hMode_Time

	movlw	12
	subwf	R_Time_Hour,W					; 判断当前时间是上午或下午
	btfsc	C
	goto	Time12h_PM

	call	SymbolPM_Clear					; 上午需灭PM点

	movf	R_Time_Hour,W					; 再判断是否在0点
	btfsc	Z
	goto	Time_0Hour
	goto	Start_DisTime_Hour
Time12h_PM:
	call	SymbolPM_Display				; 下午亮PM点
	movlw	12
	subwf	R_Time_Hour,W					; 调整为12h模式显示
	btfss	Z
	goto	Start_DisTime_Hour
Time_0Hour:
	movlw	12								; 0点在12h模式里是12点
	goto	Start_DisTime_Hour

Time_24hMode:
	call	SymbolPM_Clear					; 24h模式下没有PM点
	movf	R_Time_Hour,W
Start_DisTime_Hour:
	call	Hex2BCD							; 显示最终的结果
	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	clrf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display
	return



; 日期显示
Date_Display:
	call	Day_Display
	call	Month_Display
	return

; 日显示
Day_Display:
	movf	R_Date_Day,W
	call	Hex2BCD
	movlw	3
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	movlw	2
	movwf	R_X
	movf	P_Temp6,W
	btfss	Z
	goto	Day_Tens_NoZero
	movlw	10
Day_Tens_NoZero:
	call	Digit_Num_Display
	return

; 月显示
Month_Display:
	movf	R_Date_Month,W
	call	Hex2BCD
	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	clrf	R_X
	movf	P_Temp6,W
	btfss	Z
	goto	Day_Tens_NoZero
	movlw	10
Day_Tens_NoZero:
	call	Digit_Num_Display
	return



; 年份显示
Year_Display:
	movf	R_Date_Year,W
	call	Hex2BCD
	movlw	3
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	movlw	2
	movwf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display

	movlw	1
	movwf	R_X
	movlw	0
	call	Digit_Num_Display

	clrf	R_X
	movlw	2
	call	Digit_Num_Display
	return







; W==要转化的值
; P_Temp5、6、7为个、十、百位
Hex2BCD:
	movwf	P_Temp
	clrf	P_Temp+5
	clrf	P_Temp+6
	clrf	P_Temp+7
LOOP_HUND:
	movlw	D'100'
	subwf	P_Temp,W					; W = BIN - 100
	btfss	C
	goto	LOOP_TEN					; 不够减，退出循环
	movwf	P_Temp						; 保存减法结果到BIN
	incf	P_Temp7,F					; 百位加1
	goto	LOOP_HUND
LOOP_TEN:								; 计算十位
	movlw	D'10'
	subwf	P_Temp,W					; W = BIN - 10
	btfss	C
	goto	END_TEN
	movwf	P_Temp						; 保存减法结果到BIN
	incf 	P_Temp6,F					; 十位加1
	goto 	LOOP_TEN
END_TEN:
	movf	P_Temp,W					; 剩余值即为个位
	movwf	P_Temp5
	return




L_LSR_4Bit:
	bcf		C
	rrf		P_Temp
	rrf		P_Temp
	rrf		P_Temp
	rrf		P_Temp
	return
