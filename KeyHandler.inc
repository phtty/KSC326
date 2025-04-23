;32Hz任务，主要用于按键处理
PeriodicTask_32Hz:
	call	Key_Handler
	nop									; 颜色变换
	return




Key_Handler:
	btfss	Key_Flag,0					; 无扫键标志则不处理
	return

	btfss	Key_Flag,2					; 首次触发需要消抖
	goto	Key_Scan

	decfsz	Debounce_Over,F				; 消抖延时计数，递减到0则消抖完成
	return

	movlw	0xe0
	movwf	IOCF						; 消抖完成启用PF5~7电平变化中断
	bcf		Key_Flag,2					; 首次触发时不扫描按键
	movf	PORTF,W
	andlw	0xe0
	subwf	PORTF_BackUp,W
	btfss	Z
	goto	Key_HandlExit				; 若去抖后PF5~7的状态与去抖前不同，则为意外触发
	goto	Key_TriggerJuge				; 首次触发按键直接进入按键触发判断
Key_Scan:
	call	Depress_Juge				; 松键判断

	btfsc	Key_Flag,3
	goto	LongPress_Handle
	movlw	64
	subwf	LongPress_Counter,W
	btfsc	C
	goto	LongPress_Trigger
	incf	LongPress_Counter
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

Key_HandlExit:
	movlw	0xe0
	andwf	PORTF,W
	movwf	PORTF_BackUp				; 更新历史状态

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
	goto	Key_HandlExit


Key_KeyDTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	return
	nop
	return


Key_KeyUTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	return
	nop
	return


Key_KeySTriggered:
	call	Universal_TriggerHandle
	btfsc	Key_Flag,4					; 跳过功能
	return
	nop
	return


Key_KeyDDepress:
	return

Key_KeyUDepress:
	return

Key_KeySDepress:
	return


Universal_TriggerHandle:
	call	Key_CloseLoud
	movlw	15
	movwf	Return_Counter				; 复位计数
	clrf	DP_Counter
	return


Key_CloseLoud:
	movf	Triggered_AlarmGroup,F		; 按键是否打断响闹
	btfsc	Z
	return

	bsf		Key_Flag,4					; 打断响闹的按键无功能
	goto	Close_Loud



L_Key_Beep:
	movlw	B'11'
	movwf	Beep_Serial
	return
