;======================================================================================

; 显示模式切换状态
SwitchState_DisplayMode:
	movlw	5
	subwf	Primary_Status,W
	btfsc	C
	goto	PrimarySta_Overflow
	incf	Primary_Status,F
	goto	SwitchState_DisplayMode_Exit
PrimarySta_Overflow:
	clrf	Primary_Status
SwitchState_DisplayMode_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return




; 设置模式切换状态
SwitchState_SetMode:
	movlw	3
	subwf	Primary_Status,W
	btfsc	C
	goto	Alarm_SetMode
	movlw	3							; 非闹钟有3个设置项
	goto	SecondarySta_Juge
Alarm_SetMode:
	movlw	2							; 闹钟2个设置项
SecondarySta_Juge:
	subwf	Secondary_Status,W			; 判断设置模式是否溢出
	btfsc	C
	goto	SecondarySta_Overflow
	incf	Secondary_Status,F
	goto	SwitchState_SetMode_Exit
SecondarySta_Overflow:
	clrf	Secondary_Status
SwitchState_SetMode_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return




; 切换到温度显示
SwitchState_TemperDis:
	movlw	2
	subwf	Primary_Status,W
	btfsc	Z
	goto	Switch_TemperMode			; 已经处于温度显示模式时切换温标
	movlw	2
	movwf	Primary_Status
SwitchState_TemperDis_Exit:
	call	SymbolTEMP_Display
	call	SymbolAL_Clear				; 温度模式灭AL点
	HALFSEC_DISPLAY						; 产生半S刷新
	return




; 切换到DP显示
SwitchState_DPDis:
	bsf		SysEvent_Flag,1				; 置位DP显示标志位
	movlw	3
	movwf	Return_Counter				; 返回计时设置为3S
	call	DPMode_Reload

	movlw	2
	subwf	DP_Mode,W
	btfsc	C
	goto	DPMode_Overflow
	incf	DP_Mode,f
	movlw	2
	subwf	DP_Mode,W
	btfss	Z
	goto	SwitchState_DPDis_Exit
	movlw	15
	movwf	DP_Counter					; 复位DP模式计数
	goto	SwitchState_DPDis_Exit
DPMode_Overflow:
	clrf	DP_Mode
SwitchState_DPDis_Exit:
	call	SymbolTEMP_Clear			; 切入DP显示要清除所有散点
	call	SymbolAL_Clear
	call	SymbolPM_Clear
	call	SymbolCOL_Clear
	HALFSEC_DISPLAY						; 产生半S刷新
	return


;======================================================================================


; 小时制切换
Time_Mode_SW:
	movlw	1
	xorwf	Clock_Flag,F				; 取反12/24h标志位

	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 时增加
Time_Hour_Add:
	movlw	23
	subwf	R_Time_Hour,W
	btfsc	C
	goto	TimeHour_AddOverflow
	incf	R_Time_Hour,F
	goto	TimeHour_Add_Exit
TimeHour_AddOverflow:
	clrf	R_Time_Hour
TimeHour_Add_Exit:
	call	TimeAddBoard_Detect			; 夜间模式的处理
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 时减少
Time_Hour_Sub:
	movf	R_Time_Hour,F
	btfsc	Z
	goto	TimeHour_SubOverflow
	decf	R_Time_Hour,F
	goto	TimeHour_Sub_Exit
TimeHour_SubOverflow:
	movlw	23
	movwf	R_Time_Hour
TimeHour_Sub_Exit:
	call	TimeSubBoard_Detect			; 夜间模式的处理
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 分增加
Time_Min_Add:
	clrf	R_Time_Sec					; 调分钟会清空秒
	movlw	59
	subwf	R_Time_Min,W
	btfsc	C
	goto	TimeMin_AddOverflow
	incf	R_Time_Min,F
	goto	TimeMin_Add_Exit
TimeMin_AddOverflow:
	clrf	R_Time_Min
TimeMin_Add_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 分减少
Time_Min_Sub:
	clrf	R_Time_Sec					; 调分钟会清空秒
	movf	R_Time_Min,F
	btfsc	Z
	goto	TimeMin_SubOverflow
	decf	R_Time_Min,F
	goto	TimeMin_Sub_Exit
TimeMin_SubOverflow:
	movlw	59
	movwf	R_Time_Min
TimeMin_Sub_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return


;======================================================================================


; 年增加
Date_Year_Add:
	movlw	99
	subwf	R_Date_Year,W
	btfsc	C
	goto	DateYear_AddOverflow
	incf	R_Date_Year,F
	goto	DateYear_Add_Exit
DateYear_AddOverflow:
	clrf	R_Date_Year
DateYear_Add_Exit:
	call	DayOverflow_Juge
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 年减少
Date_Year_Sub:
	movf	R_Date_Year,F
	btfsc	Z
	goto	DateYear_SubOverflow
	decf	R_Date_Year,F
	goto	DateYear_Sub_Exit
DateYear_SubOverflow:
	movlw	99
	movwf	R_Date_Year
DateYear_Sub_Exit:
	call	DayOverflow_Juge
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 月增加
Date_Month_Add:
	movlw	12
	subwf	R_Date_Month,W
	btfsc	C
	goto	DateMonth_AddOverflow
	incf	R_Date_Month,F
	goto	DateMonth_Add_Exit
DateMonth_AddOverflow:
	movlw	1
	movwf	R_Date_Month
DateMonth_Add_Exit:
	call	DayOverflow_Juge
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 月减少
Date_Month_Sub:
	movlw	1
	subwf	R_Date_Month,W
	btfsc	Z
	goto	DateMonth_SubOverflow
	decf	R_Date_Month,F
	goto	DateMonth_Sub_Exit
DateMonth_SubOverflow:
	movlw	12
	movwf	R_Date_Month
DateMonth_Sub_Exit:
	call	DayOverflow_Juge
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 日增加
Date_Day_Add:
	incf	R_Date_Day,F
	call	DayAdd_Overflow
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 日减少
Date_Day_Sub:
	movlw	1
	subwf	R_Date_Day,W
	btfsc	Z
	goto	DateDay_SubOverflow
	decf	R_Date_Day,F
	goto	DateDay_Sub_Exit
DateDay_SubOverflow:					; 溢出时天数变为此月最大天数
	decf	R_Date_Month,W
	movwf	R_X
	btfsc	Calendar_Flag,0
	goto	DaySub_LeapYear_Handle		; 查闰年表
	movlw	High(Table_Month_Common)
	movwf	PMADRH
	movlw	Low(Table_Month_Common)
	goto	DaySub_SearchMaxDay
DaySub_LeapYear_Handle:
	movlw	High(Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(Table_Month_Leap)
DaySub_SearchMaxDay:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W					; 查表得出当月最大天数
	movwf	R_Date_Day
DateDay_Sub_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return


;======================================================================================


; 切换闹钟开关
Alarm1SW:
	movlw	B'001'
	xorwf	Alarm_Switch,F
	goto	Alarm_SetSwitch_Exit
Alarm2SW:
	movlw	B'010'
	xorwf	Alarm_Switch,F
	goto	Alarm_SetSwitch_Exit
Alarm3SW:
	movlw	B'100'
	xorwf	Alarm_Switch,F
Alarm_SetSwitch_Exit:
	movf	Alarm_Switch,F				; 若无任何闹钟开启则灭AL点，有闹钟则亮AL点
	btfsc	Z
	goto	AL_Clr
	call	SymbolAL_Display
	goto	AlarmSW_Exit
AL_Clr:
	call	SymbolAL_Clear
AlarmSW_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 闹钟时增加
Alarm_Hour_Add:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp						; 闹钟组偏移
	addlw	Low(R_Alarm1_Hour)
	movwf	FSR0L
	movlw	High(R_Alarm1_Hour)
	movwf	FSR0H

	movlw	23
	subwf	INDF0,W
	btfsc	C
	goto	AlarmHour_AddOverflow
	incf	INDF0,F
	goto	AlarmHour_Add_Exit
AlarmHour_AddOverflow:
	clrf	INDF0
AlarmHour_Add_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 闹钟时减少
Alarm_Hour_Sub:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp						; 闹钟组偏移
	addlw	Low(R_Alarm1_Hour)
	movwf	FSR0L
	movlw	High(R_Alarm1_Hour)
	movwf	FSR0H

	movf	INDF0,F
	btfsc	Z
	goto	AlarmHour_SubOverflow
	decf	INDF0,F
	goto	AlarmHour_Sub_Exit
AlarmHour_SubOverflow:
	movlw	23
	movwf	INDF0
AlarmHour_Sub_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 闹钟分增加
Alarm_Min_Add:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp						; 闹钟组偏移
	addlw	Low(R_Alarm1_Min)
	movwf	FSR0L
	movlw	High(R_Alarm1_Min)
	movwf	FSR0H

	movlw	59
	subwf	INDF0,W
	btfsc	C
	goto	AlarmMin_AddOverflow
	incf	INDF0,F
	goto	AlarmMin_Add_Exit
AlarmMin_AddOverflow:
	clrf	INDF0
AlarmMin_Add_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return

; 闹钟分减少
Alarm_Min_Sub:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp						; 闹钟组偏移
	addlw	Low(R_Alarm1_Min)
	movwf	FSR0L
	movlw	High(R_Alarm1_Min)
	movwf	FSR0H

	movf	INDF0,F
	btfsc	Z
	goto	AlarmMin_SubOverflow
	decf	INDF0,F
	goto	AlarmMin_Sub_Exit
AlarmMin_SubOverflow:
	movlw	59
	movwf	INDF0
AlarmMin_Sub_Exit:
	HALFSEC_DISPLAY						; 产生半S刷新
	return


;======================================================================================


; 切换温度单位
Switch_TemperMode:
	movlw	2
	subwf	Primary_Status,W
	btfsc	Z
	goto	Switch_TemperUnit
	movlw	2
	movwf	Primary_Status				; 非温显模式切换至温显模式
	goto	Switch_TemperMode_Exit
Switch_TemperUnit:
	movlw	2
	xorwf	ADC_Flag,F					; 温显模式取反温度单位标志位，切换显示的温度单位
Switch_TemperMode_Exit:
	call	SymbolCOL_Clear
	call	SymbolTEMP_Display
	HALFSEC_DISPLAY						; 产生半S刷新
	return


; 切换2档亮度
Switch_LightLevel:
	movf	Light_Level,F
	btfsc	Z
	goto	HighLight
	clrf	Light_Level					; 低亮切高亮
	goto	Switch_LightLevel_Exit

HighLight:
	movlw	3
	movwf	Light_Level					; 高亮切低亮

Switch_LightLevel_Exit:
	return


; 切换RGB模式
Switch_RGBMode:
	movlw	14
	subwf	RGB_Mode,W
	btfsc	C
	goto	RGB_Mode_Overflow
	incf	RGB_Mode,F
	goto	Switch_RGBMode_Exit
RGB_Mode_Overflow:
	clrf	RGB_Mode
Switch_RGBMode_Exit:
	call	RGB_ModeSwitch				; 切换RGB模式
	HALFSEC_DISPLAY						; 产生半S刷新
	return


;======================================================================================


DayOverflow_Juge:
	bcf		RP0
	call	Is_Leap_Year				; 判断当前是平年还是闰年

	decf	R_Date_Month,W
	movwf	R_X
	btfsc	Calendar_Flag,0
	goto	DayOverflow_LeapYear		; 查闰年表
	movlw	High(Table_Month_Common)
	movwf	PMADRH
	movlw	Low(Table_Month_Common)
	goto	DayOverflow_SearchMaxDay
DayOverflow_LeapYear:
	movlw	High(Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(Table_Month_Leap)
DayOverflow_SearchMaxDay:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W					; 查表得出当月最大天数
	movwf	P_Temp
	subwf	R_Date_Day,W
	btfsc	C
	goto	Day_Overflow_Yes			; 当前日期天数溢出
	return
Day_Overflow_Yes:
	movf	P_Temp,W
	movwf	R_Date_Day					; 溢出后将天数设置为当月最大天数
	return




DayAdd_Overflow:
	bcf		RP0
	call	Is_Leap_Year				; 判断当前是平年还是闰年

	decf	R_Date_Month,W
	movwf	R_X
	btfsc	Calendar_Flag,0
	goto	DayAdd_LeapYear_Handle		; 查闰年表
	movlw	High(Table_Month_Common)
	movwf	PMADRH
	movlw	Low(Table_Month_Common)
	goto	DayAdd_SearchMaxDay
DayAdd_LeapYear_Handle:
	movlw	High(Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(Table_Month_Leap)
DayAdd_SearchMaxDay:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W					; 查表得出当月最大天数
	addlw	1
	subwf	R_Date_Day,W
	btfsc	C
	goto	Day_Overflow_Yes1			; 当前日期天数溢出
	return
Day_Overflow_Yes1:
	movlw	1
	movwf	R_Date_Day					; 溢出后天数清空
	return
