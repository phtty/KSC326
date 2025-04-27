; 省电模式判断
PowerSave_Juge:
	btfsc	SysEvent_Flag,3
	goto	PowerSave_Mode					; 已经处于省电模式，则不判断入口条件

	btfsc	PORTF,0							; 当PF0==0时，进入省电模式
	return

	bcf		T1ON							; 省电模式下关闭T1及其中断
	bcf		T1IE

	clrf	IOCF							; 关闭按键口中断

	LED_OFF									; 关闭RGB灯供电

	bcf		T2ON							; 关闭响闹和蜂鸣器
	bsf		TRISA,7
	bcf		Clock_Flag,1
	clrf	Louding_Counter
	clrf	Triggered_AlarmGroup

	clrf	Primary_Status
	clrf	Secondary_Status				; 回到时间显示模式

; 省电模式
PowerSave_Mode:
	btfsc	PORTF,0							; 当PF0==0时，进入省电模式循环
	goto	PowerSave_Loop					; 在省电模式下检测到有DC5V接入则退出省电模式

	bsf		T1ON							; 退出省电模式打开T1及其中断
	bsf		T1IE
	bcf		T1IF

	movf	PORTF,F							; 重新启用按键口电平变化中断
	bcf		RFIF
	movlw	0xe0
	movwf	IOCF

	bsf		PORTA,1							; 打开RGB灯供电
	return

PowerSave_Loop:
	btfss	Time_Flag,0
	goto	PowerSave_Mode

	bcf		Time_Flag,0
	call	Time_Run						; 省电模式中只走时
	sleep									; 绿色模式
	goto	PowerSave_Mode
