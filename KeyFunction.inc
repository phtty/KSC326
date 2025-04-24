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
	nop									; 更新显示
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
	movlw	15
	movwf	Return_Counter				; 重置15S返回计时
	goto	SwitchState_SetMode_Exit
SecondarySta_Overflow:
	clrf	Secondary_Status
SwitchState_SetMode_Exit:
	nop									; 更新显示
	return




; 切换到温度显示
SwitchState_TemperDis:
	movlw	2
	subwf	Primary_Status,W
	btfsc	Z
	goto	TemperMode_Change			; 已经处于温度显示模式时切换温标
	movlw	2
	movwf	Primary_Status
SwitchState_TemperDis_Exit:
	nop									; 更新显示
	return




; 切换到DP显示
SwitchState_DPDis:
	bsf		SysEvent_Flag,1				; 置位DP显示标志位
	movlw	3
	movwf	Return_Counter				; 返回计时设置为3S
SwitchState_DPDis_Exit:
	nop									; 更新显示
	return


;======================================================================================


; 时设模式增数
AddNum_CS:
	movlw	High(CS_AddNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
CS_AddNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Time_Mode_SW
	goto	Time_Hour_Add
	goto	Time_Min_Add
	goto	Key_HandleOver

; 闹设模式增数
AddNum_AS:
	movlw	High(AS_AddNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AS_AddNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Alarm_Hour_Add				; 闹钟小时设置
	goto	Alarm_Min_Add				; 闹钟分钟设置
	goto	Key_HandleOver

; 日设模式增数
AddNum_DS:
	movlw	High(DS_AddNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DS_AddNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Date_Year_Add				; 年份设置
	goto	Date_Month_Add				; 月份设置
	goto	Date_Day_Add				; 日期设置
	goto	Key_HandleOver




; 时设模式减数
SubNum_CS:
	movlw	High(CS_SubNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
CS_SubNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Time_Mode_SW
	goto	Time_Hour_Sub
	goto	Time_Min_Sub
	goto	Key_HandleOver

; 闹设模式减数
SubNum_AS:
	movlw	High(AS_SubNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AS_SubNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Alarm_Hour_Sub				; 闹钟小时设置
	goto	Alarm_Min_Sub				; 闹钟分钟设置
	goto	Key_HandleOver

; 日设模式减数
SubNum_DS:
	movlw	High(DS_SubNum_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DS_SubNum_JumpTable:
	addwf	PCL,F
	return								; 显示模式，占位用
	goto	Date_Year_Sub				; 年份设置
	goto	Date_Month_Sub				; 月份设置
	goto	Date_Day_Sub				; 日期设置
	goto	Key_HandleOver


;======================================================================================

; 小时制切换
Time_Mode_SW:
	movlw	1
	xorwf	Clock_Flag,F				; 取反12/24h标志位

	nop									; 刷新显示
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
	call	TimeRange_Detect			; 夜间模式的处理
	nop									; 刷新显示
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
	call	TimeRange_Detect			; 夜间模式的处理
	nop									; 刷新显示
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
	call	TimeRange_Detect
	nop									; 刷新显示
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
	call	TimeRange_Detect
	nop									; 刷新显示
	return


;======================================================================================

; 年增加
Date_Year_Add:
	movlw	99
	subwf	R_Date_Year,W
	btfss	C
	goto	DateYear_AddOverflow
	incf	R_Date_Year,F
	goto	DateYear_Add_Exit
DateYear_AddOverflow:
	clrf	R_Date_Year
DateYear_Add_Exit:
	call	L_DayOverflow_Juge
	call	Is_Leap_Year
	nop									; 刷新显示
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
	call	L_DayOverflow_Juge
	call	Is_Leap_Year
	nop									; 刷新显示
	return

; 月增加
Date_Month_Add:
	movlw	12
	subwf	R_Date_Month,W
	btfss	C
	goto	DateMonth_AddOverflow
	incf	R_Date_Month,F
	goto	DateMonth_Add_Exit
DateMonth_AddOverflow:
	movlw	1
	movwf	R_Date_Month
DateMonth_Add_Exit:
	nop									; 刷新显示
	return

; 月减少
Date_Month_Sub:
	movlw	1
	movf	R_Date_Month,W
	btfss	C
	goto	DateMonth_SubOverflow
	decf	R_Date_Month,F
	goto	DateMonth_Sub_Exit
DateMonth_SubOverflow:
	movlw	12
	movwf	R_Date_Month
DateMonth_Sub_Exit:
	nop									; 刷新显示
	return

; 日增加
Date_Day_Add:
	incf	R_Date_Day,F
	call	L_DayOverflow_To_1
	nop									; 刷新显示
	return

; 日减少
Date_Day_Sub:
	movlw	1
	movf	R_Date_Day,W
	btfss	C
	goto	DateDay_SubOverflow
	decf	R_Date_Day,F
	goto	DateDay_Sub_Exit
DateDay_SubOverflow:					; 溢出时天数变为此月最大天数
	decf	R_Date_Month,W
	movf	R_X
	btfsc	Calendar_Flag,0
	goto	L_LeapYear_Handle3			; 查闰年表
	movlw	High(L_Table_Month_Common)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Common)
	goto	L_Day_Juge2
L_LeapYear_Handle3:
	movlw	High(L_Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Leap)
L_Day_Juge2:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W					; 查表得出当月最大天数
	movwf	R_Date_Day
DateDay_Sub_Exit:
	nop									; 刷新显示
	return


;======================================================================================


; 闹钟开关
Alarm_SetSwitch:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp

	movlw	High(AlarmSW_JumpTable)
	movwf	PCLATH
	movf	P_Temp,W					; 根据当前闹钟组开关闹钟
AlarmSW_JumpTable:
	addwf	PCL,F
	goto	Alarm1SW
	goto	Alarm2SW
	goto	Alarm3SW
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
	nop									; 刷新显示
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
	nop									; 刷新显示
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
	nop									; 刷新显示
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
	nop									; 刷新显示
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
	nop									; 刷新显示
	return


;======================================================================================


; 切换温度单位
TemperMode_Change:
	movlw	2
	xorwf	ADC_Flag,F					; 取反温度单位标志位，切换显示的温度单位
	nop									; 刷新显示
	return


; 调整亮度
BackLight_Level_CHG:
	nop									; 调整亮度除数，以调整亮度
	return


; RGB模式切换
RGB_MODE_CHG:
	nop									; 调整显存值的上限来调整亮度
	return


;======================================================================================


L_DayOverflow_Juge:
	bcf		RP0
	call	Is_Leap_Year				; 判断当前是平年还是闰年
	
	decf	R_Date_Month,W
	movf	R_X
	btfsc	Calendar_Flag,0
	goto	L_LeapYear_Handle			; 查闰年表
	movlw	High(L_Table_Month_Common)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Common)
	goto	L_Day_Juge1
L_LeapYear_Handle:
	movlw	High(L_Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Leap)
L_Day_Juge1:
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
	movwf	R_Date_Day					; 溢出后不再增加天数
	return




L_DayOverflow_To_1:
	bcf		RP0
	call	Is_Leap_Year				; 判断当前是平年还是闰年
	
	decf	R_Date_Month,W
	movf	R_X
	btfsc	Calendar_Flag,0
	goto	L_LeapYear_Handle			; 查闰年表
	movlw	High(L_Table_Month_Common)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Common)
	goto	L_Day_Juge1
L_LeapYear_Handle1:
	movlw	High(L_Table_Month_Leap)
	movwf	PMADRH
	movlw	Low(L_Table_Month_Leap)
L_Day_Juge3:
	addwf	R_X,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop
	movf	PMDATH,W					; 查表得出当月最大天数
	movwf	P_Temp
	subwf	R_Date_Day,W
	btfsc	C
	goto	Day_Overflow_Yes1			; 当前日期天数溢出
	return
Day_Overflow_Yes1:
	movlw	1
	movwf	R_Date_Day					; 溢出后天数清空
	return
