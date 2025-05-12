
DP1_TimeDis:
	decfsz	DP_Counter,F
	return
	movlw	2
	movwf	DP_Counter						; DP1时间模式计时结束切换到日期显示后,将计时设为2S
	movlw	1
	movwf	Primary_Status
	REFLASH_DISPLAY							; 更新显示
	HALFSEC_DISPLAY
	return
DP1_DateDis:
	decfsz	DP_Counter,F
	return
	movlw	2
	movwf	DP_Counter						; DP1日期模式计时结束切换到温度显示后,将计时设为2S
	movlw	2
	movwf	Primary_Status
	call	SymbolTEMP_Display
	call	SymbolAL_Clear					; 温度模式灭AL点亮TEMP点
	REFLASH_DISPLAY							; 更新显示
	HALFSEC_DISPLAY
	return
DP1_TemperDis:
	decfsz	DP_Counter,F
	return
	movlw	10
	movwf	DP_Counter						; DP1温度模式计时结束切换到时间显示后,将计时设为10S
	clrf	Primary_Status
	call	SymbolTEMP_Clear				; 退出温度模式灭TEMP点
	REFLASH_DISPLAY							; 更新显示
	HALFSEC_DISPLAY
	return
DP1_OtherDis:
	decfsz	DP_Counter,F
	return
	movlw	10
	movwf	DP_Counter						; DP1其他模式显示10S切换到时间显示
	clrf	Primary_Status
	REFLASH_DISPLAY							; 更新显示
	HALFSEC_DISPLAY
	return



DPMode_2:
	movlw	10
	movwf	DP_Counter						; DP2不会主动切换模式
	return



DPMode_3:
	movf	Primary_Status,F
	btfsc	Z								; DP3非时间显示模式15S切换回时间显示
	return

	decfsz	DP_Counter,F
	return
	movlw	15
	movwf	DP_Counter
	clrf	Primary_Status
	return
