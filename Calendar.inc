Calendar_Add:								; 走日期
	bcf		RP0
	bsf		Calendar_Flag,1
	call	Is_Leap_Year
	decf	R_Date_Month,W
	movf	R_X
	btfsc	Calendar_Flag,0
	goto	L_Leap_Year
	movlw	High(L_Table_Month_Common)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Common)
	goto	L_Day_Juge
L_Leap_Year:
	movlw	High(L_Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Leap)
L_Day_Juge:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W						; 查表得出当月最大天数
	subwf	R_Date_Day,W
	btfsc	C
	goto	L_Day_Overflow					; 当前日期天数溢出
	incf	R_Date_Day,F					; 天数未溢出，日期增加
	return
L_Day_Overflow:
	movlw	1
	movwf	R_Date_Day						; 重置天数，月份增加，需判断月份是否溢出
	movf	R_Date_Month,W
	sublw	12
	btfss	C
	goto	L_month_Overflow				; 月份溢出
	incf	R_Date_Month,F
	return
L_month_Overflow:
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
	goto	L_Set_LeapYear_Flag
	bcf		Calendar_Flag,0
	return
L_Set_LeapYear_Flag:
	bsf		Calendar_Flag,0
	return



; 平年的每月份天数表
L_Table_Month_Common:
	db	31	; January
	db	28	; February
	db	31	; March
	db	30	; April
	db	31	; May
	db	30	; June
	db	31	; July
	db	31	; August
	db	30	; September
	db	31	; October
	db	30	; November
	db	31	; December

; 闰年的每月份天数表
L_Table_Month_Leap:
	db	31	; January
	db	29	; February
	db	31	; March
	db	30	; April
	db	31	; May
	db	30	; June
	db	31	; July
	db	31	; August
	db	30	; September
	db	31	; October
	db	30	; November
	db	31	; December
