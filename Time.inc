F_Time_Run:
	bcf		RP0
	btfss	Time_Flag,5
	return

	bcf		Time_Flag,5
	incf	R_Time_Sec,F
	movlw	60
	subwf	R_Time_Sec,W
	btfss	C
	return										; 未发生分钟进位

	btfsc	NightPeriod_Flag,0
	call	TimeBoundary_Detect					; 发生分钟进位时，判断一次是否进入夜间模式
	movf	R_Time_Min,W
	clrf	R_Time_Sec
	incf	R_Time_Min,F
	movlw	60	
	subwf	R_Time_Min,W
	btfss	C

	return										; 未发生小时进位
	clrf	R_Time_Min
	incf	R_Time_Hour,F
	movlw	24
	subwf	R_Time_Hour,W
	btfss	C
	return										; 未发生天进位

	clrf	R_Time_Hour
	call	F_Calendar_Add
	return




F_Clock_Display:
	bcf		RP0
	btfsc	Secondary_Status,0
	goto	L_DisDate_Mode
	goto	F_Time_Display						; 若子模式序号为0，则显示时间
L_DisDate_Mode:
	; 之后应该加一个显示温度的部分
	goto	F_Date_Display						; 子模式序号为1则显示日期

F_Time_Display:
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	call	F_Display_Time

	btfsc	Time_Flag,0
	goto	L_Dot_Clear							; 1S标志触发灭S点
	call	F_DisCol							; 半S标志触发亮S点
	return
L_Dot_Clear:
	bcf		Time_Flag,0
	call	F_ClrCol
	return




; 时钟设置模式
F_Clock_Set:
	bcf		RP0

	movf	Secondary_Status,W
	btfsc	Z
	goto	F_TimeMode_Switch					; 12/24h模式切换
	movlw	1
	subwf	Secondary_Status,W
	btfsc	Z
	goto	F_DisHour_Set						; 小时设置
	movlw	2
	subwf	Secondary_Status,W
	btfsc	Z
	goto	F_DisMin_Set						; 分钟设置
	return


; 12h/24h模式切换
F_TimeMode_Switch:
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	call	F_ClrCol
	btfss	Time_Flag,0
	goto	L_TMSwitch_Clr
	call	L_Dis_xxHr							; 半S亮
	return
L_TMSwitch_Clr:
	bcf		Time_Flag,0
	call	F_UnDisplay_D0_1					; 1S灭
	call	F_UnDisplay_D2_3
	return


; 小时设置
F_DisHour_Set:
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_Hour			; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	call	F_DisCol
	btfsc	Time_Flag,0
	goto	L_Hour_Clear
L_KeyTrigger_NoBlink_Hour:
	call	F_Display_Time						; 半S亮
	return
L_Hour_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D0_1					; 1S灭
	return

; 分钟设置
F_DisMin_Set:
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_Min			; 存在快加时常亮
	btfss	Time_Flag,1
	return
	
	bcf		Time_Flag,1
	call	F_DisCol
	btfsc	Time_Flag,0
	goto	L_Min_Clear
L_KeyTrigger_NoBlink_Min:
	call	F_Display_Time						; 半S亮
	return
L_Min_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D2_3					; 1S灭
	return
