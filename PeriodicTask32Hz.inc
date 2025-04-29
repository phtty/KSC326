;32Hz任务，主要用于按键处理
PeriodicTask_32Hz:
	btfss	Time_Flag,4
	return
	bcf		Time_Flag,4

	btfsc	Time_Flag,3
	call	PeriodicTask_16Hz

	call	Key_Handler
	return



; 16Hz任务，用于蜂鸣间隔和RGB变色刷新
PeriodicTask_16Hz:
	bcf		Time_Flag,3

	call	BeepManage							; 蜂鸣管理
	call	RGB_TickReflash						; RGB渐变刷新
	return


; RGB变色的Tick更新
RGB_TickReflash:
	btfss	SysEvent_Flag,3
	return

	clrf	R_X
GRB_TickReflash_Loop:
	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	INDF0,W
	sublw	239
	btfss	C
	goto	GRB_Tick_Overflow
	incf	INDF0,F								; 未溢出则自增1后继续循环
	goto	GRB_TickReflash_LoopJuge
GRB_Tick_Overflow:
	clrf	INDF0								; Tick达到240则说明溢出，复位此灯的Tick
GRB_TickReflash_LoopJuge:
	incf	R_X,F
	movlw	33
	subwf	R_X,W
	btfss	C
	goto	GRB_TickReflash_Loop
	REFLASH_DISPLAY								; 刷新显示
	return

