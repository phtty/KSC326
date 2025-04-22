F_Alarm_Display:
	bcf		RP0
	call	F_DisCol
	call	F_Display_Alarm

	return




; 闹钟设置模式
F_Alarm_Set:
	bcf		RP0

	movf	Secondary_Status,W
	btfsc	Z
	goto	F_AlarmHour_Set						; 闹钟时设
	movlw	1
	subwf	Secondary_Status,W
	btfsc	Z
	goto	F_AlarmMin_Set						; 小时设置
	movlw	2
	subwf	Secondary_Status,W
	btfsc	Z
	goto	F_Display_SNZ						; 贪睡时间
	movlw	3
	subwf	Secondary_Status,W
	btfsc	Z
	goto	F_AlarmWorkDay_Dis					; 工作日闹钟
	return


F_AlarmHour_Set:
	bcf		RP0
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_AlarmHour		; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	call	F_DisCol
	btfsc	Time_Flag,0
	goto	L_AlarmHour_Clear
L_KeyTrigger_NoBlink_AlarmHour:
	call	F_Display_Alarm						; 半S亮
	return
L_AlarmHour_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D0_1					; 1S灭
	return


F_AlarmMin_Set:
	bcf		RP0
	btfsc	Key_Flag,3
	goto	L_KeyTrigger_NoBlink_AlarmMin		; 存在快加时常亮
	btfss	Time_Flag,1
	return

	bcf		Time_Flag,1
	call	F_DisCol
	btfsc	Time_Flag,0
	goto	L_AlarmMin_Clear
L_KeyTrigger_NoBlink_AlarmMin:
	call	F_Display_Alarm						; 半S亮
	return
L_AlarmMin_Clear:
	bcf		Time_Flag,0
	call	F_UnDisplay_D2_3					; 1S灭
	return




; 闹钟处理
F_Alarm_Handler:
	movf	Alarm_Switch,F
	btfss	Z									; 无闹钟开启则不判断闹钟触发
	goto	Is_AlarmTrigger
	return
Is_AlarmTrigger:
	movlw	B'001'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm1_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm1_Hour,W
	btfsc	Z
	goto	Alarm1_HourMatch					; 闹钟1小时匹配
Alarm1_NoMatch:
	movlw	B'010'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm2_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm2_Hour,W
	btfsc	Z
	goto	Alarm2_HourMatch					; 闹钟2小时匹配
Alarm2_NoMatch:
	movlw	B'100'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm3_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm3_Hour,W
	btfsc	Z
	goto	Alarm3_HourMatch					; 闹钟3小时匹配
Alarm3_NoMatch:
	return


Alarm1_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm1_Min,W
	btfsc	Z
	goto	Alarm1_MinMatch						; 闹钟1分钟匹配
	goto	Alarm1_NoMatch

Alarm2_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm2_Min,W
	btfsc	Z
	goto	Alarm2_MinMatch						; 闹钟2分钟匹配
	goto	Alarm2_NoMatch

Alarm3_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm3_Min,W
	btfsc	Z
	goto	Alarm3_MinMatch						; 闹钟3分钟匹配
	goto	Alarm3_NoMatch


Alarm1_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; 秒不匹配则直接退出
	bsf		Triggered_AlarmGroup,0				; 置位闹钟1触发
	goto	Alarm_MatchHandle

Alarm2_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; 秒不匹配则直接退出
	bsf		Triggered_AlarmGroup,1				; 置位闹钟1触发
	goto	Alarm_MatchHandle

Alarm3_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; 秒不匹配则直接退出
	bsf		Triggered_AlarmGroup,2				; 置位闹钟1触发

Alarm_MatchHandle:
	bsf		Clock_Flag,1						; 置位闹钟触发和响闹状态
	bsf		Clock_Flag,2
	bsf		Beep_Flag,0
	bsf		TimeFlag_SW,1
	bcf		Time_Flag,3
	clrf	Louding_Counter
	return
