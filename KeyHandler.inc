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

	goto	Key_TriggerJuge				; 首次触发按键直接进入按键触发判断
Key_Scan:
	movlw	0xe0
	andwf	PORTF,W
	sublw	0xe0						; 检测到无按键时结束扫键，并执行松键功能
	btfss	Z
	goto	Key_LongPress_Juge

	call	Depress_Juge				; 松键功能判断
	goto	Key_HandleOver

Key_LongPress_Juge:
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
	call	Key_DPExit
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver

	movf	Secondary_Status,F
	btfss	Z
	goto	KeyD_SetMode				; 设置模式下D键只有短按功能

	btfss	Key_Flag,3
	goto	KeyD_ShortTrigger			; 长按到来前只置位松键处理标志

	bcf		Depress_Flag,0				; 长按触发则复位松键处理
	call	Key_Beep
	call	Switch_LightLevel			; 显示模式下长按切换亮度
	goto	Key_HandleOver

KeyD_ShortTrigger:
	bsf		Depress_Flag,0				; 置位D键松键处理标志
	call	Key_Beep_Cancel				; 按键音在松键处理中响

KeyD_SetMode:
	movf	Primary_Status,F
	btfss	Z
	goto	KeyD_NoTimeSet				; 非时间设置

	goto	SubNum_CS					; 时设模式执行减数

KeyD_NoTimeSet:
	movlw	1
	subwf	Primary_Status,W
	btfss	Z
	goto	KeyD_NoDateSet				; 非日期设置

	goto	SubNum_DS					; 日设模式执行减数

KeyD_NoDateSet:
	movlw	3
	subwf	Primary_Status,W
	btfss	C
	goto	Key_KeyDTriggered_Exit		; 非闹钟设置

	goto	SubNum_AS					; 闹设模式执行减数

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
	call	Switch_RGBMode				; 时显模式切换RGB模式
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
	call	Key_DPExit
	btfsc	Key_Flag,4					; 跳过功能
	goto	Key_HandleOver

	movf	Secondary_Status,F
	btfss	Z
	goto	KeyS_SetMode				; 设置模式下S键只有短按功能

	btfsc	Key_Flag,3
	goto	KeyS_DisMode_LongPress
	call	Key_Beep_Cancel				; 松键有效的按键在松键时才有按键音
	bsf		Depress_Flag,2				; 显示模式短按置位松键处理标志
	return
KeyS_DisMode_LongPress:
	call	Key_Beep
	bcf		Depress_Flag,2				; 显示模式长按会执行设置模式切换功能
KeyS_SetMode:
	movlw	2
	subwf	Primary_Status,W
	btfss	Z							; 若为温度显示，则不会进行设置切换
	call	SwitchState_SetMode
	goto	Key_HandleOver



; D键松键处理
Key_KeyDDepress:
	bcf		Depress_Flag,0
	call	Key_Beep
	goto	Switch_TemperMode			; 切换至温显或者切换温标

; U键松键处理（未使用）
Key_KeyUDepress:
	bcf		Depress_Flag,1
	return

; S键松键处理
Key_KeySDepress:
	bcf		Depress_Flag,2
	call	Key_Beep
	goto	SwitchState_DisplayMode		; 切换显示模式


; 通用按键处理
Universal_TriggerHandle:
	btfss	Key_Flag,3
	call	Key_Beep					; 按键音，长按不生效
	call	Key_CloseLoud
	movlw	10
	movwf	DP_Counter					; 复位DP模式计数
	movlw	15							; 复位返回时显模式计数
	movwf	Return_Counter
	return


Key_CloseLoud:
	movf	Triggered_AlarmGroup,F		; 判断按键是否打断了响闹
	btfsc	Z
	return								; 未打断响闹则执行其按键功能

	bsf		Key_Flag,4					; 打断响闹则此次按键无功能
	goto	Close_Loud

Key_DPExit:
	btfss	SysEvent_Flag,1
	return

	bcf		SysEvent_Flag,1				; 清除DP标志
	movlw	10
	movwf	DP_Counter
	clrf	Primary_Status				; 切换到时间显示模式并刷新显示
	REFLASH_DISPLAY
	HALFSEC_DISPLAY
	bsf		Key_Flag,4					; 退出DP模式则此次按键无功能
	return



; 按键音
Key_Beep:
	call	NoBeep_Serial_Mode
	movlw	B'10'
	movwf	Beep_Serial
	bsf		Beep_Flag,0
	bsf		Beep_Flag,1
	return

; 取消按键音
Key_Beep_Cancel:
	call	NoBeep_Serial_Mode
	clrf	Beep_Serial
	return
