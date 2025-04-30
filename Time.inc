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
	call	TimeRange_Detect					; 发生小时进位时，判断一次是否进入夜间模式
	movlw	24
	subwf	R_Time_Hour,W
	btfss	C
	return										; 未发生天进位

	clrf	R_Time_Hour							; 发送天进位时更新日期
	call	Calendar_Add
	return




; 夜间时段范围检测（19:00~6:00）
TimeRange_Detect:
	movlw	19
	subwf	R_Time_Hour,W
	btfsc	C
	goto	Enter_NightMode					; 若小时大于等于19点则进入夜间时段

	movlw	6
	subwf	R_Time_Hour,W
	btfss	C
	goto	Enter_NightMode					; 若小时小于6点也进入夜间时段
	
	clrf	Light_Level						; 亮度设为高亮
	REFLASH_DISPLAY
	return

Enter_NightMode:
	movlw	3
	movwf	Light_Level						; 亮度设为低亮
	REFLASH_DISPLAY
	return
