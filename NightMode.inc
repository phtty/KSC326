; 夜间时段边界检测
TimeBoundary_Detect:
	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_StartHour,W
	movwf	P_Temp+2
	movf	R_Night_StartMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	2									; 若当前时间等于开始时间则切换到夜间模式
	btfsc	Z
	goto	SetNightMode

	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_EndHour,W
	movwf	P_Temp+2
	movf	R_Night_EndMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	2									; 若当前时间等于结束时间则切换到白天模式
	btfsc	Z
	goto	SetDayMode

	return


; 夜间时段范围检测
TimeRange_Detect:
	movf	R_Night_StartHour,W
	movwf	P_Temp
	movf	R_Night_StartMin,W
	movwf	P_Temp+1
	movf	R_Night_EndHour,W
	movwf	P_Temp+2
	movf	R_Night_EndMin,W
	movwf	P_Temp+3
	call	Time_Compare						; 先判断开始时间是否大于结束时间
	sublw	1
	btfsc	Z
	goto	NightPeriod_OverDay

	call	Time_Compare
	sublw	2
	btfsc	Z
	return										; 若开始时段等于结束时段则什么也不做

	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_StartHour,W
	movwf	P_Temp+2
	movf	R_Night_StartMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	0									; 若当前时间小于开始时间则为白天模式
	btfsc	Z
	goto	SetDayMode

	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_EndHour,W
	movwf	P_Temp+2
	movf	R_Night_EndMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	1									; 若当前时间大于结束时间则为白天模式
	btfsc	Z
	goto	SetDayMode
	
	goto	SetNightMode						; 否则为夜间模式

NightPeriod_OverDay:
	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_EndHour,W
	movwf	P_Temp+2
	movf	R_Night_EndMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	0									; 若当前时间小于结束时间则为夜间模式
	btfsc	Z
	goto	SetNightMode

	movf	R_Time_Hour,W
	movwf	P_Temp
	movf	R_Time_Min,W
	movwf	P_Temp+1
	movf	R_Night_StartHour,W
	movwf	P_Temp+2
	movf	R_Night_StartMin,W
	movwf	P_Temp+3
	call	Time_Compare
	sublw	1									; 若当前时间大于开始时间则为夜间模式
	btfsc	Z
	goto	SetNightMode

	goto	SetDayMode							; 否则为白天模式



; 时间比较函数
; P_Temp=时间1小时，P_Temp+1=时间1分钟
; P_Temp+2=时间2小时，P_Temp+3=时间2分钟
; W=比较结果，时间1>时间2 W=1；时间2>时间1 W=0；时间1=2 W=2
Time_Compare:
	movf	P_Temp+2,W							; 小时相减
	subwf	P_Temp,W
	btfss	C
	retlw	0									; 时间2>时间1

	btfss	Z
	retlw	1									; 时间1>时间2

	movf	P_Temp+3,W							; 分钟相减
	subwf	P_Temp+1,W
	btfss	C
	retlw	0									; 时间2>时间1

	btfss	Z
	retlw	1									; 时间1>时间2

	retlw	2									; 时间1=时间2


SetDayMode:
	bsf		RP0
	movlw	0xe8								; 亮度设为高亮
	movwf	PWM0DL								; 先设置低位
	movlw	0x03
	movwf	PWM0DH
	bcf		RP0
	return

SetNightMode:
	bsf		RP0
	movlw	0xf4								; 亮度设为半亮
	movwf	PWM0DL								; 先设置低位
	movlw	0x01
	movwf	PWM0DH
	bcf		RP0
	return
