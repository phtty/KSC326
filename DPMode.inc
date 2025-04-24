DPMode_Mangage:
	movf	Secondary_Status,F				; 设置模式下不进行DP切换
	btfss	Z
	return
	incf	DP_Counter,F					; 递增DP计数

	movlw	High(DPMode_JumpTable)
	movwf	PCLATH
	movf	DP_Mode,W
DPMode_JumpTable:
	addwf	PCL,F
	goto	DPMode_1
	goto	DPMode_2
	goto	DPMode_3
	
DPMode_1:
	movlw	3
	subwf	Primary_Status,W
	btfsc	C
	goto	DP1_OtherDis					; 非时间、日期、温度模式

	movlw	High(DP1_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
DP1_JumpTable:
	addwf	PCL,F
	goto	DP1_TimeDis
	goto	DP1_DateDis
	goto	DP1_TemperDis
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
