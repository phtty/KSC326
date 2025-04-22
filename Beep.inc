F_BeepManage:
	btfss	Beep_Flag,0
	return

	movf	Beep_Serial,F
	btfsc	Z
	goto	L_NoBeep_Serial_Mode
	decf	Beep_Serial
	btfss	Beep_Serial,0
	goto	L_NoBeep_Serial_Mode
	bcf		TRISA,7						; PA7输出启用
	bsf		T2ON						; 开启TIM2
	return
L_NoBeep_Serial_Mode:
	bcf		T2ON						; 关闭TIM2
	bsf		TRISA,7						; PA7输出关闭
	return


LoudManage:
	btfss	Clock_Flag,2
	return
	
	movlw	4
	movwf	Beep_Serial					; 响闹序列为两声
	incf	Louding_Counter
	movlw	60
	subwf	Louding_Counter,W			; 判断是否响闹结束
	btfss	C
	return

	clrf	Louding_Counter
	btfss	Beep_Flag,1					; 若存在按键提示音，则不关闭蜂鸣器
	goto	KeyBeep_NoClose
	bcf		T2ON
	bsf		TRISA,7
KeyBeep_NoClose:
	bcf		Clock_Flag,1
	bcf		Clock_Flag,2
	clrf	Triggered_AlarmGroup
	return
