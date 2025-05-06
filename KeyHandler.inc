Key_Handler:
	btfss	Key_Flag,0					; 无扫键标志则不处理
	return

	btfss	Key_Flag,2					; 首次触发需要消抖
	goto	Key_Scan

	movf	Debounce_Counter,W
	decfsz	Debounce_Counter,F			; 消抖延时计数，递减到0则消抖完成
	return

	bcf		Key_Flag,2					; 首次触发时不扫描按键
	movf	PORTF,W
	andlw	0xe0
	subwf	PORTF_BackUp,W
	btfss	Z
	goto	Key_HandleOver				; 若去抖后PF5~7的状态与去抖前不同，则为意外触发

	call	Key_Beep					; 确定触发后响按键音
	goto	Key_TriggerJuge				; 首次触发按键直接进入按键触发判断
Key_Scan:
	movlw	0xe0
	andwf	PORTF,W
	sublw	0xe0						; 检测到无按键时结束扫键，并执行松键功能
	btfss	Z
	goto	Key_Keep

	call	Depress_Juge				; 松键功能判断
	goto	Key_HandleOver

Key_Keep:
	btfsc	Key_Flag,3
	goto	LongPress_Handle
	movlw	64
	subwf	LongPress_Counter,W
	btfsc	C
	goto	LongPress_Trigger
	incf	LongPress_Counter,F
	return
LongPress_Trigger:						; 计满2S后长按触发，置位标志位并执行功能
	bsf		Key_Flag,3
	bcf		Time_Flag,2
	goto	Key_TriggerJuge
LongPress_Handle:
	btfss	Time_Flag,2					; 有长按标志时，4Hz执行一次按键功能
	return
	bcf		Time_Flag,2
Key_TriggerJuge:
	movlw	0xe0
	andwf	PORTF,W
	movwf	P_Temp
	sublw	0xc0
	btfsc	Z
	goto	Key_KeyDTriggered			; D键触发

	movlw	0xa0
	subwf	P_Temp,W
	btfsc	Z
	goto	Key_KeyUTriggered			; U键触发

	movlw	0x60
	subwf	P_Temp,W
	btfsc	Z
	goto	Key_KeySTriggered			; S键触发

Key_HandleOver:
	clrf	Key_Flag
	clrf	LongPress_Counter			; 复位快加/长按计数值
	clrf	Depress_Flag

	movf	PORTF,F
	bcf		RFIF
	movlw	0xe0
	movwf	IOCF						; 按键扫描完成重新启用PF5~7电平变化中断
	return

; 松键功能判断
Depress_Juge:
	movf	Depress_Flag,F
	btfsc	Z							; 无任何松键功能则跳过
	return

	btfsc	Depress_Flag,0
	goto	Key_KeyDDepress
	btfsc	Depress_Flag,1
	goto	Key_KeyUDepress
	btfsc	Depress_Flag,2
	goto	Key_KeySDepress
	goto	Key_HandleOver


; D键触发
Key_KeyDTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver

	movf	Primary_Status,F
	btfss	Z
	goto	KeyD_NoTimeMode				; 非时间模式

	movf	Secondary_Status,F
	btfss	Z
	goto	SubNum_CS					; 时设模式执行增数
	call	TemperMode_Change			; 切换至温显
	goto	Key_HandleOver				; 时显模式只执行1次按键功能

KeyD_NoTimeMode:
	movlw	1
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyD_NoDateMode				; 非日期模式

	movf	Secondary_Status,F
	btfss	Z
	goto	SubNum_DS					; 日设模式执行减数
	call	TemperMode_Change			; 切换至温显
	goto	Key_HandleOver				; 日显模式只执行1次按键功能

KeyD_NoDateMode:
	movlw	2
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyD_NoTemperMode			; 非温度模式

	call	TemperMode_Change			; 切换温标
	goto	Key_HandleOver				; 温显模式只执行1次按键功能

KeyD_NoTemperMode:
	movf	Secondary_Status,F
	btfss	Z
	goto	SubNum_AS					; 闹设模式执行减数
	call	TemperMode_Change			; 切换至温显
	goto	Key_HandleOver				; 闹显模式只执行1次按键功能
Key_KeyDTriggered_Exit:
	return


; U键触发
Key_KeyUTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver

	movf	Primary_Status,F
	btfss	Z
	goto	KeyU_NoTimeMode				; 非时间模式

	movf	Secondary_Status,F
	btfss	Z
	goto	AddNum_CS					; 时设模式执行增数
	call	RGB_MODE_CHG				; 时显模式切换RGB模式
	goto	Key_HandleOver				; 时显模式只执行1次按键功能

KeyU_NoTimeMode:
	movlw	1
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyU_NoDateMode				; 非日期模式

	movf	Secondary_Status,F
	btfss	Z
	goto	AddNum_DS					; 日设模式执行增数
	call	SwitchState_DPDis			; 日显模式切换DP显示
	goto	Key_HandleOver				; 日显模式只执行1次按键功能

KeyU_NoDateMode:
	movlw	2
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyU_NoTemperMode			; 非温度模式

	call	SwitchState_DPDis			; 温显模式切换DP显示
	goto	Key_HandleOver				; 温显模式只执行1次按键功能

KeyU_NoTemperMode:
	movf	Secondary_Status,F
	btfss	Z
	goto	AddNum_AS					; 闹设模式执行增数
	call	Alarm_SetSwitch				; 闹显模式开关当前闹钟
	goto	Key_HandleOver				; 闹显模式只执行1次按键功能
Key_KeyUTriggered_Exit:
	return


; S键触发
Key_KeySTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver
	
	movf	Secondary_Status,F
	btfss	Z
	goto	KeyS_SetMode

	btfsc	Key_Flag,3
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
	bcf		Depress_Flag,0
	return

Key_KeyUDepress:
	bcf		Depress_Flag,1
	return

Key_KeySDepress:
	bcf		Depress_Flag,2
	call	SwitchState_DisplayMode
	goto	Key_HandleOver


Universal_TriggerHandle:
	call	Key_CloseLoud
	clrf	DP_Counter					; 复位DP模式计数
	movlw	15							; 复位返回时显模式计数
	movwf	Return_Counter
	bcf		SysEvent_Flag,1				; 清除DP标志
	return


Key_CloseLoud:
	movf	Triggered_AlarmGroup,F		; 判断按键是否打断了响闹
	btfsc	Z
	return								; 未打断响闹则执行其按键功能

	bsf		Key_Flag,4					; 打断响闹则此次按键无功能
	goto	Close_Loud



; 按键音
Key_Beep:
	movlw	B'10'
	movwf	Beep_Serial
	bsf		Beep_Flag,0
	bsf		Beep_Flag,1
	return
