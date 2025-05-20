; 2Hz任务，将每S分成半S和1S；半S执行亮点，1S执行走时、各类计时加时、灭点。
PeriodicTask_2Hz:
	btfss	Time_Flag,1
	return
	bcf		Time_Flag,1						; 复位2Hz标志

	REFLASH_DISPLAY							; 半S强制刷新一次显示
	btfsc	Time_Flag,0
	goto	Sec_Handle

; 半S处理
HalfSec_Handle:
	btfsc	SysEvent_Flag,1
	goto	DPMode_Display					; 存在DP显示标志时，显示当前的DP模式

	goto	HalfSec_Jump


; AL点显示
AL_Display:
	movf	Alarm_Switch,F					; 无任何一组闹钟开启时不显示
	btfsc	Z
	goto	SymbolAL_Clear

	goto	SymbolAL_Display				; 有闹钟开启时显示


; 1S处理
Sec_Handle:
	bcf		Time_Flag,0						; 复位1Hz标志

	call	Time_Run						; 走时
	call	Alarm_Handler					; 闹钟判断
	call	LoudManage						; 响闹管理
	call	DPMode_Mangage					; DP模式切换
	call	Return_DisMode					; 设置模式定时返回显示模式
	call	ColorChange						; 定时变色模式下，切换颜色计时
	call	Temperature_Test				; 定时测温

	btfsc	SysEvent_Flag,1					; 若有DP显示标志则跳过灭点
	return
	btfsc	Key_Flag,3						; 若有长按标志则跳过灭点
	return

	goto	Sec_Jump



; 返回时显模式计时
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
	btfss	SysEvent_Flag,1					; 若是从DP显示返回则还需要返回到时间模式
	goto	Return_DisMode_Exit
	bcf		SysEvent_Flag,1					; 清除DP标志
	clrf	Primary_Status					; 返回时间模式
Return_DisMode_Exit:
	REFLASH_DISPLAY							; 刷新显示
	HALFSEC_DISPLAY
	return




; 定时变色模式
ColorChange:
	movlw	14
	subwf	RGB_Mode,W
	btfss	Z
	return									; 当前RGB模式不为定时变色模式则不进处理

	decfsz	ColorCHG_Counter,F
	return									; 未计到0则直接退出

	movlw	5
	movwf	ColorCHG_Counter				; 复位变色时间计数

	incf	Color_Num,F
	movlw	7
	subwf	Color_Num,W
	movf	Color_Num,W
	btfsc	C
	goto	ColorNum_Overflow
	movwf	R_X
	goto	RGB_SetColor

ColorNum_Overflow:
	clrf	Color_Num
	movlw	1
	movwf	R_X
	goto	RGB_SetColor
