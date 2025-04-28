; 走时程序，带分钟、小时进位和日期更新
Time_Run:
	incf	R_Time_Sec,F
	movlw	60
	subwf	R_Time_Sec,W
	btfss	C
	return										; 未发生分钟进位

	REFLASH_DISPLAY								; 发生分钟进位时刷新显示
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





; 时间模式半S程序
TimeDis_HalfSec:
	movf	Secondary_Status,F
	btfss	Z
	goto	TimeDis_SetMode

	btfsc	SysEvent_Flag,1						; 显示模式下，有DP标志时不显示时间
	return

TimeDis_SetMode:
	movlw	High(TimeSetDis_HS_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
TimeSetDis_HS_JumpTable:
	addwf	PCL,F
	goto	Time_Display
	goto	TimeMode_Display
	goto	Time_Display
	goto	Time_Display



; 时间模式1S程序
TimeDis_Sec:
	movlw	High(TimeSetDis_JumpTable)			; 1S灭
	movwf	PCLATH
	movf	Secondary_Status,W
TimeSetDis_JumpTable:
	addwf	PCL,F
	return
	goto	Undisplay_All
	goto	Undisplay_01
	goto	Undisplay_23




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
