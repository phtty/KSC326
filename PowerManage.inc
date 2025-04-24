; 常亮和省电模式切换
PowerMode_CHG:
	btfss	Power_Flag,0
	return

	bcf		Power_Flag,0
	btfsc	PORTF,3
	goto	Keep_Bright

	bcf		Power_Flag,1					; 切换到省电模式
	return

Keep_Bright:
	bsf		Power_Flag,1					; 切换到常亮模式
	return


; 省电模式的自动灭屏功能
Power_Manage:
	call	PowerMode_CHG
	btfsc	Power_Flag,1					; 在有常亮标志时不进行倒计时
	goto	Light_Keep
	btfss	Power_Flag,2					; 只有1S到来才会判断是否熄屏
	return

	bcf		Power_Flag,2
	decfsz	TurnOffScreen_Counter,F
	return									; 倒计时未归零则什么也不做

	movlw	16
	movwf	TurnOffScreen_Counter			; 倒计时归零后重装载计时
	goto	Close5020						; 进关闭5020函数

Light_Keep:
	movlw	16
	movwf	TurnOffScreen_Counter			; 重装载计时
	btfsc	PORTF,5
	call	Open5020						; 若是熄屏状态，则会打开5020
	return


Close5020:
	bcf		T1IE							; 关闭TIM1中断
	bsf		PORTF,5							; 关闭5020供电

	movlw	0xf8
	andwf	PORTF							; 重置5020通讯IO

	bsf		RP0
	bcf		EPWM0							; 除能COM线的PWM输出
	bcf		EPWM01
	bcf		RP0
	return

; 根据记忆亮度亮屏
Open5020:
	movf	BackLight_Level,W				; 根据记忆亮度等级进行亮屏
	btfsc	Z
	goto	LowLightLevel
	sublw	1
	btfsc	Z
	goto	MidLightLevel
	sublw	2
	btfsc	Z
	goto	HighLightLevel

LowLightLevel:
	bsf		RP0
	movlw	0xfa
	movwf	PWM0DL							; 低亮
	movlw	0x00
	movwf	PWM0DH
	bcf		RP0
	goto	Open5020_Continue

MidLightLevel:
	bsf		RP0
	movlw	0xf4
	movwf	PWM0DL							; 半亮
	movlw	0x01
	movwf	PWM0DH
	bcf		RP0
	goto	Open5020_Continue

HighLightLevel:
	bsf		RP0
	movlw	0xe8
	movwf	PWM0DL							; 高亮
	movlw	0x03
	movwf	PWM0DH
	bcf		RP0

Open5020_Continue:
	bsf		T1IE							; 重新打开LED发送中断
	bcf		PORTF,5							; 开启5020供电
	return
