BeepManage:
	btfss	Beep_Flag,0
	return

	movf	Beep_Serial,F
	btfsc	Z
	goto	NoBeep_Serial_Mode
	decf	Beep_Serial,F
	btfss	Beep_Serial,0
	goto	NoBeep_Serial_Mode
	bcf		TRISA,7						; PA7输出启用
	bsf		T2ON						; 开启TIM2
	return
NoBeep_Serial_Mode:
	btfsc	Beep_Flag,1
	clrf	Beep_Flag					; 若有按键音标志位则关闭蜂鸣处理

	bcf		T2ON						; 关闭TIM2
	bsf		TRISA,7						; PA7输出关闭
	return


LoudManage:
	btfss	Clock_Flag,1
	return

	movlw	4
	movwf	Beep_Serial					; 响闹序列为两声
	incf	Louding_Counter,F
	movlw	60
	subwf	Louding_Counter,W			; 判断是否响闹结束
	btfss	C
	return

Close_Loud:
	btfss	Beep_Flag,1					; 若存在按键提示音，则不关闭蜂鸣器
	goto	KeyBeep_NoClose
	bcf		T2ON
	bsf		TRISA,7
KeyBeep_NoClose:
	bcf		Clock_Flag,1
	clrf	Triggered_AlarmGroup
	clrf	Louding_Counter
	return
