Time_Run:
	bcf		RP0
	btfss	Time_Flag,5
	return

	bcf		Time_Flag,5
	incf	R_Time_Sec,F
	movlw	60
	subwf	R_Time_Sec,W
	btfss	C
	return										; 未发生分钟进位

	movf	R_Time_Min,W
	clrf	R_Time_Sec
	incf	R_Time_Min,F
	movlw	60	
	subwf	R_Time_Min,W
	btfss	C
	return										; 未发生小时进位

	btfsc	SysEvent_Flag,2
	call	TimeBoundary_Detect					; 发生小时进位时，判断一次是否进入夜间模式
	clrf	R_Time_Min
	incf	R_Time_Hour,F
	movlw	24
	subwf	R_Time_Hour,W
	btfss	C
	return										; 未发生天进位

	clrf	R_Time_Hour
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
	return


; 时间模式1S程序
Time_Display_Sec:
	movlw	High(TimeSetDis_JumpTable)			; 1S灭
	movwf	PCLATH
	movf	Secondary_Status,W
TimeSetDis_JumpTable:
	addwf	PCL,F
	return
	goto	Undisplay_Digit
	goto	Undisplay_01
	goto	Undisplay_23
	return
