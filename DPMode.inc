
DP1_TimeDis:
	movlw	10
	subwf	DP_Counter,W
	btfss	C
	return
	clrf	DP_Counter						; DP1时间显示10S切换到日期显示
	movlw	1
	movwf	Primary_Status
	return
DP1_DateDis:
	movlw	2
	subwf	DP_Counter,W
	btfss	C
	return
	clrf	DP_Counter						; DP1日期显示2S切换到温度显示
	movlw	2
	movwf	Primary_Status
	return
DP1_TemperDis:
	movlw	2
	subwf	DP_Counter,W
	btfss	C
	return
	clrf	DP_Counter						; DP1温度显示2S切换到时间显示
	clrf	Primary_Status
	return
DP1_OtherDis:
	movlw	10
	subwf	DP_Counter,W
	btfss	C
	return
	clrf	DP_Counter						; DP1其他模式显示10S切换到时间显示
	clrf	Primary_Status
	return



DPMode_2:
	clrf	DP_Counter						; DP2不会主动切换模式
	return



DPMode_3:
	movf	Primary_Status,F
	btfsc	Z								; DP3非时间显示模式15S切换回时间显示
	return

	movlw	15
	subwf	DP_Counter,W
	btfss	C
	return
	clrf	DP_Counter
	clrf	Primary_Status
	return
