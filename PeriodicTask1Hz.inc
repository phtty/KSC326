; 1Hz处理
PeriodicTask_1Hz:
	btfss	Time_Flag,0
	return
	bcf		Time_Flag,0						; 复位1Hz标志

; 还有一些加时1S的程序
	call	F_Time_Run						; 走时
	call	F_Alarm_Handler					; 闹钟判断
	call	LoudManage						; 响闹管理
	call	DPMode_Mangage					; DP模式切换
	call	Return_DisMode					; 设置模式定时返回显示模式

	movlw	High(PeriodicTask_1HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
PeriodicTask_1HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_Sec
	goto	DateDis_Sec
	goto	TemperDis_Sec
	goto	Alarm1Dis_Sec
	goto	Alarm2Dis_Sec
	goto	Alarm3Dis_Sec
	return



TimeDis_Sec:
	movlw	High(TimeModeDis_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
TimeModeDis_JumpTable:
	addwf	PCL,F
	goto	Time_Display
	goto	TimeMode_Dis
	goto	TimeHourSet_Dis
	goto	TimeMinSet_Dis
	return



DateDis_Sec:
	movlw	High(DateModeDis_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
DateModeDis_JumpTable:
	addwf	PCL,F
	goto	Date_Display
	goto	DateYearSet_Dis
	goto	DateMonthSet_Dis
	goto	DateDaySet_Dis
	return



TemperDis_Sec:
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
	movf	Secondary_Status,F				; 若不处于设置模式则直接退出
	btfsc	Z
	return

	decfsz	Return_Counter,F
	return									; 未计到0则直接退出
	movlw	15
	movwf	Return_Counter					; 复位设置模式返回计时
	clrf	Secondary_Status
	nop										; 刷新显示
	return
