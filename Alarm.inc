; 闹钟处理
Alarm_Handler:
	movf	Secondary_Status,F
	btfss	Z									; 处于设置模式则不判断闹钟触发
	return

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
	bsf		Clock_Flag,1						; 置位响闹状态
	bsf		Beep_Flag,0
	bcf		Time_Flag,3
	clrf	Louding_Counter
	return
