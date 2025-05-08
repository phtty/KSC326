; 走时程序，带分钟、小时进位和日期更新
Time_Run:
	incf	R_Time_Sec,F
	movlw	60
	subwf	R_Time_Sec,W
	btfss	C
	return										; 未发生分钟进位

	clrf	R_Time_Sec
	incf	R_Time_Min,F
	movlw	60	
	subwf	R_Time_Min,W
	btfss	C
	return										; 未发生小时进位

	clrf	R_Time_Min
	incf	R_Time_Hour,F
	call	TimeBoard_Detect					; 发生小时进位时，判断一次是否进入夜间模式
	movlw	24
	subwf	R_Time_Hour,W
	btfss	C
	return										; 未发生天进位

	clrf	R_Time_Hour							; 发送天进位时更新日期
	call	Calendar_Add
	return




; 夜间时段边界检测（19:00&6:00）
TimeBoard_Detect:
	movlw	19
	subwf	R_Time_Hour,W
	btfss	Z
	goto	NightMode_Juge

	goto	Enter_NightMode					; 若时间等于19点则进入夜间时段

NightMode_Juge:
	movlw	6
	subwf	R_Time_Hour,W
	btfss	Z
	goto	TimeRange_Detect_Exit

	clrf	Light_Level						; 若时间等于6点则退出夜间时段
	goto	TimeRange_Detect_Exit

Enter_NightMode:
	movlw	3
	movwf	Light_Level						; 亮度设为低亮

TimeRange_Detect_Exit:
	REFLASH_DISPLAY
	return
