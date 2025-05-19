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


;======================================================================================


; 单色模式，根据R_X的值（0~6）对33个时刻内存赋值
RGB_SetColor:
	clrf	P_Temp							; 循环计数内存

	movf	R_X,F
	btfsc	Z
	goto	SetColor_White
	movlw	1
	subwf	R_X,W
	movwf	P_Temp+1
	bcf		C
	rlf		P_Temp+1,F
	rlf		P_Temp+1,F
	rlf		P_Temp+1,F
	rlf		P_Temp+1,F
	rlf		P_Temp+1,F
	movlw	1
	subwf	R_X,W
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F
	addwf	P_Temp+1,F						; 将当前模式值非0则-1乘以40得到初始时刻值
	goto	SetColor_Loop
SetColor_White:
	movlw	241
	movwf	P_Temp+1						; 当前模式值为0则赋白色光的初始时刻值
SetColor_Loop:
	movlw	Low(GRB_Tick)
	addwf	P_Temp,W						; 以P_Temp循环计数为偏移
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	P_Temp+1,W
	movwf	INDF0							; 将初值赋给对应GRB时刻表的内存

	incf	P_Temp,F
	movlw	33
	subwf	P_Temp,W
	btfss	C
	goto	SetColor_Loop					; 当计数计到33时，退出循环

	return



; 横滚模式根据当前是否渐变，每个8字的时刻间隔会改变
RGB_Horizon_Roll:
	clrf	P_Temp							; 初始化外层循环计数
	clrf	P_Temp+1						; 初始化内层循环计数
	clrf	P_Temp+2						; 初始化总偏移计数
	clrf	P_Temp+3						; 初始化初值
HorizonDigit_Loop:
	movlw	High(IncTable_HorizonRolling)
	movwf	PCLATH
	movf	P_Temp,W
	call	IncTable_HorizonRolling
	movwf	P_Temp+1						; 外层循环计数值作为偏移量查增量表，作为内层循环计数

HorizonSeg_Loop:
	movlw	High(RGBTable_HorizonRolling)
	movwf	PCLATH
	movf	P_Temp+2,W
	call	RGBTable_HorizonRolling
	movwf	R_X								; 将总循环计数作为偏移查横滚顺序表得出GRB时刻表的偏移

	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	P_Temp+3,W
	movwf	INDF0							; 将初值赋给对应GRB时刻表的内存

	incf	P_Temp+2,F						; 递增总偏移

	decfsz	P_Temp+1,F
	goto	HorizonSeg_Loop					; 递增内层循环

	btfss	SysEvent_Flag,3					; 每次内层循环结束说明8字改变，递增初值
	goto	Fixed_Mode
	movlw	40
	goto	InitialData_CHG
Fixed_Mode:
	movlw	60
InitialData_CHG:
	addwf	P_Temp+3						; 根据是固定模式还是渐变模式选择每个8字的初值增量

	incf	P_Temp,F
	movlw	4
	subwf	P_Temp,W
	btfss	C
	goto	HorizonDigit_Loop				; 递增外层循环

	return



; 垂直渐变
RGB_Vertical_Roll:
	clrf	P_Temp							; 初始化外层循环计数
	clrf	P_Temp+1						; 初始化内层循环计数
	clrf	P_Temp+2						; 初始化总偏移计数
	clrf	P_Temp+3						; 初始化初值
VerticalDigit_Loop:
	movlw	High(IncTable_VerticalRolling)
	movwf	PCLATH
	movf	P_Temp,W
	call	IncTable_VerticalRolling
	movwf	P_Temp+1						; 外层循环计数值作为偏移量查增量表，作为内层循环计数

VerticalSeg_Loop:
	movlw	High(RGBTable_VerticalRolling)
	movwf	PCLATH
	movf	P_Temp+2,W
	call	RGBTable_VerticalRolling
	movwf	R_X								; 将总循环计数作为偏移查横滚顺序表得出GRB时刻表的偏移

	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	P_Temp+3,W
	movwf	INDF0							; 将初值赋给对应GRB时刻表的内存

	incf	P_Temp+2,F						; 递增总偏移

	decfsz	P_Temp+1,F
	goto	VerticalSeg_Loop				; 递增内层循环

	btfss	SysEvent_Flag,3					; 每次内层循环结束说明8字改变，递增初值
	goto	VFixed_Mode
	movlw	40
	goto	VInitialData_CHG
VFixed_Mode:
	movlw	40
VInitialData_CHG:
	addwf	P_Temp+3						; 根据是固定模式还是渐变模式选择每个8字的初值增量

	incf	P_Temp,F
	movlw	5
	subwf	P_Temp,W
	btfss	C
	goto	VerticalDigit_Loop				; 递增外层循环

	return



; 乱彩模式，根据RGBTable_ChaosColor对33个时刻内存赋值
RGB_SetChaosColor:
	clrf	P_Temp							; 循环计数内存

SetChaosColor_Loop:
	movlw	High(RGBTable_ChaosColor)
	movwf	PCLATH
	movf	P_Temp,W
	call	RGBTable_ChaosColor
	movwf	P_Temp+1						; 根据循环计数值查询随机表

	movlw	Low(GRB_Tick)
	addwf	P_Temp,W						; 以P_Temp循环计数为偏移
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	P_Temp+1,W
	movwf	INDF0							; 将初值赋给对应GRB时刻表的内存

	incf	P_Temp,F
	movlw	33
	subwf	P_Temp,W
	btfss	C
	goto	SetChaosColor_Loop				; 当计数计到33时，退出循环

	return





SingleColor_White:
	bcf		SysEvent_Flag,3
	clrf	R_X
	call	RGB_SetColor
	return


SingleColor_Red:
	bcf		SysEvent_Flag,3
	movlw	1
	movwf	R_X
	call	RGB_SetColor
	return


SingleColor_Yellow:
	bcf		SysEvent_Flag,3
	movlw	2
	movwf	R_X
	call	RGB_SetColor
	return


SingleColor_Green:
	bcf		SysEvent_Flag,3
	movlw	3
	movwf	R_X
	call	RGB_SetColor
	return


SingleColor_Cyan:
	bcf		SysEvent_Flag,3
	movlw	4
	movwf	R_X
	call	RGB_SetColor
	return


SingleColor_Blue:
	bcf		SysEvent_Flag,3
	movlw	5
	movwf	R_X
	call	RGB_SetColor
	return


SingleColor_Purple:
	bcf		SysEvent_Flag,3
	movlw	6
	movwf	R_X
	call	RGB_SetColor
	return


DigitalAlter_Horizon_Fixed:
	bcf		SysEvent_Flag,3
	call	RGB_Horizon_Roll
	return


DigitalAlter_Vertical_Fixed:
	bcf		SysEvent_Flag,3
	call	RGB_Vertical_Roll
	return


ChaosColor_Fixed:
	bcf		SysEvent_Flag,3
	call	RGB_SetChaosColor
	return


SingleColor_Rolling:
	bsf		SysEvent_Flag,3
	movlw	1
	movwf	R_X
	call	RGB_SetColor
	return


ChaosColor_Rolling:
	bsf		SysEvent_Flag,3
	call	RGB_SetChaosColor
	return


DigitalAlter_Horizon_Rolling:
	bsf		SysEvent_Flag,3
	call	RGB_Horizon_Roll
	return


DigitalAlter_Vertical_Rolling:
	bsf		SysEvent_Flag,3
	call	RGB_Vertical_Roll
	return


SingleColor_Change:
	movlw	5
	movwf	ColorCHG_Counter				; 复位变色时间计数
	movlw	1
	movwf	Color_Num						; 复位颜色计数
	goto	SingleColor_Red					; 定时变色模式的初始状态为红色
