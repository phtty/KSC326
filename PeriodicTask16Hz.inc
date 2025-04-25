; 16Hz任务，用于蜂鸣间隔和RGB变色刷新
PeriodicTask_16Hz:
	btfss	Time_Flag,3
	return
	bcf		Time_Flag,3

	call	BeepManage							; 蜂鸣管理
	call	RGB_TickReflash						; RGB渐变刷新
	return



; RGB变色的Tick更新
RGB_TickReflash:
	movlw	32
	movwf	R_X
GRB_TickReflash_Loop:
	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	INDF0,W
	sublw	239
	btfsc	C
	goto	GRB_Tick_Overflow
	incf	INDF0,F								; 未溢出则自增1后继续循环
	goto	GRB_TickReflash_LoopJuge
GRB_Tick_Overflow:
	clrf	INDF0								; Tick达到240则说明溢出，复位此灯的Tick
GRB_TickReflash_LoopJuge:
	decfsz	R_X
	goto	GRB_TickReflash_Loop
	return
