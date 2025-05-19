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
	REFLASH_DISPLAY							; 更新显示
	HALFSEC_DISPLAY
	return
DP1_TemperDis:
	decfsz	DP_Counter,F
	return
	movlw	10
	movwf	DP_Counter						; DP1温度模式计时结束切换到时间显示后,将计时设为10S
	clrf	Primary_Status
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
	movlw	15
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


DPMode_Reload:
	movf	DP_Mode,W
	btfsc	Z
	goto	DP1_Reload

	movlw	15
	movwf	DP_Counter						; 当前为DP-2、3，重装载计数15
	return

DP1_Reload:
	movlw	10
	movwf	DP_Counter						; 当前为DP-1，重装载计数10
	return
