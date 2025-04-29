; 2Hz任务，将每S分成半S和1S；半S执行亮点，1S执行走时、各类计时加时、灭点。
PeriodicTask_2Hz:
	btfss	Time_Flag,1
	return
	bcf		Time_Flag,1						; 复位2Hz标志

	btfss	Time_Flag,0
	goto	HalfSec_Handle
	goto	Sec_Handle						; 1S处理

; 半S处理
HalfSec_Handle:
	call	SymbolCOL_Display				; 秒点闪烁

	movlw	High(PeriodicTask_2HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
PeriodicTask_2HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_HalfSec
	goto	DateDis_HalfSec
	goto	TemperDis_HalfSec
	goto	AlarmDis_HalfSec
	goto	AlarmDis_HalfSec
	goto	AlarmDis_HalfSec




; 1S处理
Sec_Handle:
	bcf		Time_Flag,0						; 复位1Hz标志

	call	Time_Run						; 走时
	call	Alarm_Handler					; 闹钟判断
	call	LoudManage						; 响闹管理
	call	DPMode_Mangage					; DP模式切换
	call	Return_DisMode					; 设置模式定时返回显示模式
	call	SymbolCOL_Clear					; 秒点闪烁
	call	Temperature_Test				; 定时测温

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


Return_DisMode:
	movf	Secondary_Status,F				; 若处于设置模式则进入退出计时
	btfss	Z
	goto	Return_Counting
	btfss	SysEvent_Flag,1					; 存在DP标志位也进入退出计时
	return
Return_Counting:
	decfsz	Return_Counter,F
	return									; 未计到0则直接退出

	clrf	Secondary_Status				; 返回显示模式
	REFLASH_DISPLAY							; 刷新显示
	HALFSEC_DISPLAY
	return
