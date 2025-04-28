;32Hz任务，主要用于按键处理
PeriodicTask_32Hz:
	btfss	Time_Flag,4
	return
	bcf		Time_Flag,4

	call	Key_Handler
	return




Key_Handler:
	btfss	Key_Flag,0					; 无扫键标志则不处理
	return

	btfss	Key_Flag,2					; 首次触发需要消抖
	goto	Key_Scan

	decfsz	Debounce_Counter,F			; 消抖延时计数，递减到0则消抖完成
	return

	movlw	0xe0
	movwf	IOCF						; 消抖完成启用PF5~7电平变化中断
	bcf		Key_Flag,2					; 首次触发时不扫描按键
	movf	PORTF,W
	andlw	0xe0
	subwf	PORTF_BackUp,W
	btfss	Z
	goto	Key_HandleOver				; 若去抖后PF5~7的状态与去抖前不同，则为意外触发

	call	Key_Beep					; 确定触发后响按键音
	goto	Key_TriggerJuge				; 首次触发按键直接进入按键触发判断
Key_Scan:
	call	Depress_Juge				; 松键判断

	btfsc	Key_Flag,3
	goto	LongPress_Handle
	movlw	64
	subwf	LongPress_Counter,W
	btfsc	C
	goto	LongPress_Trigger
	incf	LongPress_Counter,F
	return
LongPress_Trigger:
	bsf		Key_Flag,3
	bsf		TimeFlag_SW,0				; 打开4Hz计数，置位快加
	bcf		Time_Flag,2
	return
LongPress_Handle:
	btfss	Time_Flag,2
	return
	bcf		Time_Flag,2
Key_TriggerJuge:
	movlw	0xe0
	andwf	PORTF,W
	movwf	P_Temp
	sublw	0x20
	btfsc	Z
	goto	Key_KeyDTriggered

	movlw	0x40
	subwf	P_Temp,W
	btfsc	Z
	goto	Key_KeyUTriggered

	movlw	0x80
	subwf	P_Temp,W
	btfsc	Z
	goto	Key_KeySTriggered

Key_HandleOver:
	movlw	0xe0
	andwf	PORTF,W
	movwf	PORTF_BackUp				; 更新历史状态
	bcf		TimeFlag_SW,1
	bcf		TimeFlag_SW,2				; 关闭4Hz和32Hz的计数开关

	clrf	Key_Flag
	clrf	LongPress_Counter			; 复位快加/长按计数值
	clrf	Depress_Flag
	return

Depress_Juge:
	btfss	Key_Flag,1					; 松键事件处理
	return

	btfsc	Depress_Flag,0
	goto	Key_KeyDDepress
	btfsc	Depress_Flag,1
	goto	Key_KeyUDepress
	btfsc	Depress_Flag,2
	goto	Key_KeySDepress
	goto	Key_HandleOver


Key_KeyDTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver
	
	movf	Primary_Status,F
	btfss	Z
	goto	KeyD_NoDisMode
	call	SwitchState_TemperDis
	goto	Key_HandleOver				; Down键在显示模式只执行1次功能
KeyD_NoDisMode:
	movf	Primary_Status,F
	btfss	Z
	goto	KeyD_NoTimeSet
	goto	SubNum_CS
KeyD_NoTimeSet:
	movlw	1
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyD_NoDateSet
	goto	SubNum_DS
KeyD_NoDateSet:
	movlw	3
	subwf	Primary_Status,W
	btfss	C
	goto	Key_HandleOver				; 非时间、日期、闹钟设置则为无效按键
	goto	SubNum_AS

Key_KeyDTriggered_Exit:
	return


Key_KeyUTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver

	movf	Primary_Status,F
	btfss	Z
	goto	KeyU_NoDisMode
	call	SwitchState_DPDis
	goto	Key_HandleOver				; Up键在显示模式只执行1次功能
KeyU_NoDisMode:
	movf	Primary_Status,F
	btfss	Z
	goto	KeyU_NoTimeSet
	goto	AddNum_CS
KeyU_NoTimeSet:
	movlw	1
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyU_NoDateSet
	goto	AddNum_DS
KeyU_NoDateSet:
	movlw	3
	subwf	Primary_Status,W
	btfss	C
	goto	Key_HandleOver				; 非时间、日期、闹钟设置则为无效按键
	goto	AddNum_AS

Key_KeyUTriggered_Exit:
	return


Key_KeySTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver
	
	movf	Secondary_Status,F
	btfss	Z
	goto	KeyS_SetMode

	btfss	Key_Flag,3
	goto	KeyS_DisMode_LongPress
	bsf		Depress_Flag,2				; 显示模式短按置位松键处理标志
	return
KeyS_DisMode_LongPress:
	bcf		Depress_Flag,2				; 显示模式长按会执行设置模式切换功能
KeyS_SetMode:
	movlw	2
	subwf	Primary_Status,W
	btfss	Z							; 若为温度显示，则不会进行设置切换
	call	SwitchState_SetMode
	goto	Key_HandleOver


Key_KeyDDepress:
	return

Key_KeyUDepress:
	return

Key_KeySDepress:
	bcf		Depress_Flag,2
	call	SwitchState_DisplayMode
	goto	Key_HandleOver


Universal_TriggerHandle:
	call	Key_CloseLoud
	clrf	DP_Counter					; 复位计数
	return


Key_CloseLoud:
	movf	Triggered_AlarmGroup,F		; 按键是否打断响闹
	btfsc	Z
	return

	bsf		Key_Flag,4					; 打断响闹的按键无功能
	goto	Close_Loud



Key_Beep:
	movlw	B'10'
	movwf	Beep_Serial
	return
