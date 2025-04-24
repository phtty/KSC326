; 1Hz处理
PeriodicTask_1Hz:
	btfss	Time_Flag,0
	return
	bcf		Time_Flag,0						; 复位1Hz标志

; 还有一些加时1S的程序
	call	Time_Run						; 走时
	call	Alarm_Handler					; 闹钟判断
	call	LoudManage						; 响闹管理
	call	DPMode_Mangage					; DP模式切换
	call	Return_DisMode					; 设置模式定时返回显示模式
	call	DotCOL_Sec						; 秒点闪烁

	movlw	High(PeriodicTask_1HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
PeriodicTask_1HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_Sec
	goto	DateDis_Sec
	return
	goto	AlarmDis_Sec
	goto	AlarmDis_Sec
	goto	AlarmDis_Sec
	return




Alarm1Dis_Sec:
	movlw	High(Alarm1ModeDis_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
Alarm1ModeDis_JumpTable:
	addwf	PCL,F
	goto	Alarm1_Display
	goto	Alarm1SW_Dis
	goto	Alarm1HourSet_Dis
	goto	Alarm1MinSet_Dis
	return



Alarm2Dis_Sec:
	movlw	High(Alarm2ModeDis_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
Alarm2ModeDis_JumpTable:
	addwf	PCL,F
	goto	Alarm2_Display
	goto	Alarm2SW_Dis
	goto	Alarm2HourSet_Dis
	goto	Alarm2MinSet_Dis
	return



Alarm3Dis_Sec:
	movlw	High(Alarm3ModeDis_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
Alarm3ModeDis_JumpTable:
	addwf	PCL,F
	goto	Alarm3_Display
	goto	Alarm3SW_Dis
	goto	Alarm3HourSet_Dis
	goto	Alarm3MinSet_Dis
	return



Return_DisMode:
	movf	Secondary_Status,F				; 若处于设置模式则进入计时
	btfss	Z
	goto	Return_Counting
	btfss	SysEvent_Flag,1					; 存在DP标志位也进入计时
	return
Return_Counting:
	decfsz	Return_Counter,F
	return									; 未计到0则直接退出
	movlw	15
	movwf	Return_Counter					; 复位设置模式返回计时
	clrf	Secondary_Status
	nop										; 刷新显示
	return
