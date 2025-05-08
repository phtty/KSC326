Calendar_Add:								; 走日期
	bcf		RP0
	call	Is_Leap_Year
	decf	R_Date_Month,W
	movwf	R_X
	btfsc	Calendar_Flag,0
	goto	Calendar_Leap_Year
	movlw	High(Table_Month_Common)
	movwf	PMADRH
	movlw	Low(Table_Month_Common)
	goto	Calendar_DayJuge
Calendar_Leap_Year:
	movlw	High(Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(Table_Month_Leap)
Calendar_DayJuge:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W						; 查表得出当月最大天数
	subwf	R_Date_Day,W
	btfsc	C
	goto	Calendar_DayOverflow			; 当前日期天数溢出
	incf	R_Date_Day,F					; 天数未溢出，日期增加
	return
Calendar_DayOverflow:
	movlw	1
	movwf	R_Date_Day						; 重置天数，月份增加，需判断月份是否溢出
	movf	R_Date_Month,W
	sublw	12
	btfss	C
	goto	Calendar_MonthOverflow			; 月份溢出
	incf	R_Date_Month,F
	return
Calendar_MonthOverflow:
	movlw	1
	movwf	R_Date_Month					; 重置月份，增加年份
	incf	R_Date_Year,F
	movf	R_Date_Year,W
	sublw	99
	btfsc	C
	return									; 年份未溢出，增日期结束
	clrf	R_Date_Year						; 年份溢出，重置年份
	return

; 判断平闰年函数
Is_Leap_Year:
	bcf		RP0
	movlw	B'0011'
	andwf	R_Date_Year,W
	btfsc	Z
	goto	Set_LeapYear_Flag
	bcf		Calendar_Flag,0
	return
Set_LeapYear_Flag:
	bsf		Calendar_Flag,0
	return
