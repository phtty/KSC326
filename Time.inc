; 走时程序，带分钟、小时进位和日期更新
Time_Run:
	incf	R_Time_Sec,F
	movlw	60
	subwf	R_Time_Sec,W
	btfss	C
	return										; 未发生分钟进位

	clrf	R_Time_Sec
	incf	R_Time_Min,F
	call	CompenTime_Juge						; 每次进位分钟更新一次补偿时间
	movlw	60	
	subwf	R_Time_Min,W
	btfss	C
	return										; 未发生小时进位

	clrf	R_Time_Min
	incf	R_Time_Hour,F
	call	TimeAddBoard_Detect					; 发生小时进位时，判断一次是否进入夜间模式
	movlw	24
	subwf	R_Time_Hour,W
	btfss	C
	return										; 未发生天进位

	clrf	R_Time_Hour							; 发送天进位时更新日期
	call	Calendar_Add
	return


Time_HS_Display:
	call	TimeSymbol_Display
	goto	Time_Display


TimeMode_HS_Display:
	call	TimeModeSymbol_Display
	goto	TimeMode_Display



Time_Sec_Display:
	call	SymbolCOL_Clear
	btfsc	Clock_Flag,1
	call	SymbolAL_Clear					; 响闹时1S灭AL点
	goto	Time_Display


TimeMode_Sec_Display:
	call	TimeModeSymbol_Display
	goto	Undisplay_All


TimeHour_HS_Display:
	call	TimeSymbol_Display
	goto	Undisplay_01


TimeMin_HS_Display:
	call	TimeSymbol_Display
	goto	Undisplay_23




; 时增加的夜间时段边界检测（19:00&6:00）
TimeAddBoard_Detect:
	movlw	19
	subwf	R_Time_Hour,W
	btfsc	Z
	goto	Enter_NightMode					; 若时间等于19点则进入夜间时段

	movlw	6
	subwf	R_Time_Hour,W
	btfsc	Z
	clrf	Light_Level						; 若时间等于6点则退出夜间时段

	goto	TimeRange_Detect_Exit

Enter_NightMode:
	movlw	3
	movwf	Light_Level						; 亮度设为低亮

TimeRange_Detect_Exit:
	;REFLASH_DISPLAY
	return




; 时减少的夜间时段边界检测（5:00&18:00）
TimeSubBoard_Detect:
	movlw	5
	subwf	R_Time_Hour,W
	btfsc	Z
	goto	TS_Enter_NightMode				; 若时间等于19点则进入夜间时段

	movlw	18
	subwf	R_Time_Hour,W
	btfsc	Z
	clrf	Light_Level						; 若时间等于6点则退出夜间时段

	return

TS_Enter_NightMode:
	movlw	3
	movwf	Light_Level						; 亮度设为低亮
	return
