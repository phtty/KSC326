; 2Hz任务，将每S分成半S和1S；半S执行亮点，1S执行走时、各类计时加时、灭点。
PeriodicTask_2Hz:
	btfss	Time_Flag,1
	return
	bcf		Time_Flag,1						; 复位2Hz标志

	REFLASH_DISPLAY							; 半S刷新一次显示
	btfsc	Time_Flag,0
	goto	Sec_Handle

; 半S处理
HalfSec_Handle:
	call	COL_Display						; 秒点闪烁

	goto	HalfSec_Jump


COL_Display:
	movf	Primary_Status,F
	btfss	Z
	goto	COL_DisplayJuge2

	movlw	1
	subwf	Secondary_Status,W
	btfss	Z
	goto	Display_COL						; 时钟模式非12/24H模式都亮S点
	return

COL_DisplayJuge2:
	movlw	3
	subwf	Primary_Status,W				; 闹钟模式亮S点
	btfss	C
	return

Display_COL:
	goto	SymbolCOL_Display				; S点亮




; 1S处理
Sec_Handle:
	bcf		Time_Flag,0						; 复位1Hz标志

	call	Time_Run						; 走时
	call	Alarm_Handler					; 闹钟判断
	call	LoudManage						; 响闹管理
	call	DPMode_Mangage					; DP模式切换
	call	Return_DisMode					; 设置模式定时返回显示模式
	call	COL_Clear						; 秒点闪烁
	call	Temperature_Test				; 定时测温

	goto	Sec_Jump


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


COL_Clear:
	movf	Primary_Status,F
	btfss	Z
	goto	COL_ClearJuge2

	movlw	2
	subwf	Secondary_Status,W
	btfss	C
	goto	Clear_COL						; 时钟模式的前两个会熄S点
	return

COL_ClearJuge2:
	movlw	3
	subwf	Primary_Status,W				; 闹钟之前的模式熄S点
	btfsc	C
	return

Clear_COL:
	goto	SymbolCOL_Clear					; S点熄灭
