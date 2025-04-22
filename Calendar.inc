F_Calendar_Add:								; 走日期
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


; 通过当前日期计算当前星期数
L_GetWeek:
	bcf		RP0
	call	Is_Leap_Year
	decf	R_Date_Day,W
	call	L_MOD_A_7
	movwf	P_Temp+1						; 当前日期相对当月首日的星期偏移

	decf	R_Date_Month,W
	movwf	R_X

	btfsc	Calendar_Flag,0
	goto	L_DateToWeek_Leap

	movlw	High(L_Table_Gap_CommonMonth)
	movwf	PMADRH
	movlw	low(L_Table_Gap_CommonMonth)	; 平年当月首日的星期偏移
	goto	L_Get_Week
L_DateToWeek_Leap:
	movlw	High(L_Table_Gap_LeapMonth)
	movwf	PMADRH
	movlw	low(L_Table_Gap_LeapMonth)		; 闰年当月首日的星期偏移
L_Get_Week:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W
	movwf	P_Temp+2

	movf	R_Date_Year,W
	bcf		C
	rrf		R_Date_Year,W					; 年份除以2作为索引查表
	movwf	R_X
	movlw	High(L_Table_WeekInYear)
	movwf	PMADRH
	movlw	low(L_Table_WeekInYear)			; 查询当年首日的星期
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W
	btfsc	R_Date_Year,0
	goto	L_Odd_Year						; 偶数年份取低四位
	andlw	B'00000111'
	goto	L_Get_Weak_YearFirstDay
L_Odd_Year:
	movwf	P_Temp+3
	swapf	P_Temp+3,F						; 奇数年份取高四位
	movlw	B'00000111'
	andwf	P_Temp+3,W
L_Get_Weak_YearFirstDay:
	addwf	P_Temp+1,W
	addwf	P_Temp+2,W						; 将所有年月日的星期偏移加在一起对7求余即为当前星期
	call	L_MOD_A_7
	movwf	R_Date_Week
	return




; 日期显示
F_Date_Display:
	bcf		RP0
	call	F_ClrCol
	movlw	led_pm
	movwf	R_X
	call	F_ClrSymbol
	call	F_Display_Date
	return




; 年份设置
F_DisYear_Set:
	bcf		RP0
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_Year			; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	btfsc	Time_Flag,0
	goto	L_Year_Clear
L_KeyTrigger_NoBlink_Year:
	call	L_DisDate_Year
	return
L_Year_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D0_1
	call	F_UnDisplay_D2_3
	return




; 月份设置
F_DisMonth_Set:
	bcf		RP0
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_Month			; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	btfsc	Time_Flag,0
	goto	L_Month_Clear
L_KeyTrigger_NoBlink_Month:
	call	L_DisDate_Month
	call	L_DisDate_Day
	return
L_Month_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D0_1
	return




; 日期设置
F_DisDay_Set:
	bcf		RP0
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_Day			; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	btfsc	Time_Flag,0
	goto	L_Day_Clear
L_KeyTrigger_NoBlink_Day:
	call	L_DisDate_Month
	call	L_DisDate_Day
	return
L_Day_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D2_3
	return




; 对7求余数
L_MOD_A_7:
	movwf	P_Temp
	movlw	7
	subwf	P_Temp,W
	btfss	C
	goto	L_MOD_A_7Over
	goto	L_MOD_A_7
L_MOD_A_7Over:
	movf	P_Temp,W
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

L_Table_WeekInYear:
	db	0x1E	; 2001,2000 E="1110"代表2000年1月1日是星期六(110),是闰年(1)
	db	0x32	; 2003,2002
	db	0x6C	; 2005,2004
	db	0x10	; 2007,2006
	db	0x4A	; 2009,2008
	db	0x65	; 2011,2010
	db	0x28	; 2013,2012
	db	0x43	; 2015,2014
	db	0x0D	; 2017,2016
	db	0x21	; 2019,2018
	db	0x5B	; 2021,2020
	db	0x06	; 2023,2022
	db	0x39	; 2025,2024
	db	0x54	; 2027,2026
	db	0x1E	; 2029,2028
	db	0x32	; 2031,2030
	db	0x6C	; 2033,2032
	db	0x10	; 2035,2034
	db	0x4A	; 2037,2036
	db	0x65	; 2039,2038
	db	0x28	; 2041,2040
	db	0x43	; 2043,2042
	db	0x0D	; 2045,2044
	db	0x21	; 2047,2046
	db	0x5B	; 2049,2048
	db	0x06	; 2051,2050
	db	0x39	; 2053,2052
	db	0x54	; 2055,2054
	db	0x1E	; 2057,2056
	db	0x32	; 2059,2058
	db	0x6C	; 2061,2060
	db	0x10	; 2063,2062
	db	0x4A	; 2065,2064
	db	0x65	; 2067,2066
	db	0x28	; 2069,2068
	db	0x43	; 2071,2070
	db	0x0D	; 2073,2072
	db	0x21	; 2075,2074
	db	0x5B	; 2077,2076
	db	0x06	; 2079,2078
	db	0x39	; 2081,2080
	db	0x54	; 2083,2082
	db	0x1E	; 2085,2084
	db	0x32	; 2087,2086
	db	0x6C	; 2089,2088
	db	0x10	; 2091,2090
	db	0x4A	; 2093,2092
	db	0x65	; 2095,2094
	db	0x28	; 2097,2096
	db	0x43	; 2099,2098

; 平年里每月份首日对当前年份首日的星期偏移
L_Table_Gap_CommonMonth:
	db	0	; 1月1日
	db	3	; 2月1日
	db	3	; 3月1日
	db	6	; 4月1日
	db	1	; 5月1日
	db	4	; 6月1日
	db	6	; 7月1日
	db	2	; 8月1日
	db	5	; 9月1日
	db	0	; 10月1日
	db	3	; 11月1日
	db	5	; 12月1日

; 闰年里每月份首日对当前年份首日的星期偏移
L_Table_Gap_LeapMonth:
	db	0	; 1月1日
	db	3	; 2月1日
	db	4	; 3月1日
	db	0	; 4月1日
	db	2	; 5月1日
	db	5	; 6月1日
	db	0	; 7月1日
	db	3	; 8月1日
	db	6	; 9月1日
	db	1	; 10月1日
	db	4	; 11月1日
	db	6	; 12月1日
