;======================================================================================

; 切换时钟显示
SwitchState_ClockDis:
	movlw	B'00000001'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2CD						; 若当前非时钟显示则只切换到时钟显示

	incf	Secondary_Status,F			; 时钟显示模式的子模式切换
	movlw	4
	subwf	Secondary_Status,W
	btfsc	C
	clrf	Secondary_Status			; 超过最后一个子模式序号则清零回到第一个
	return

SW2CD:
	movlw	B'00000001'
	movwf	Primary_Status
	clrf	Secondary_Status			; 清空子模式序号
	goto	L_KeyHandle_Over			; 从别的模式切入会结束按键处理




; 切换到闹钟显示模式
SwitchState_AlarmDis:
	movlw	B'00000010'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2AD						; 若当前非闹钟显示则只切换到闹钟显示

	movlw	B'10000000'
	xorwf	Clock_Flag,F				; 开启或关闭闹钟
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理

SW2AD:
	movlw	B'00000010'
	movwf	Primary_Status
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理




; 切换到时钟设置模式
SwitchState_ClockSet:
	movlw	B'00000100'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2CS						; 若当前非时钟设置则只切换到时钟设置

	incf	Secondary_Status,F			; 时钟设置模式的子模式切换
	movlw	3
	subwf	Secondary_Status,W
	btfsc	C
	goto	SwitchState_ClockDis		; 子模式溢出后退出设置模式，回到时显模式

	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理

SW2CS:
	movlw	B'00000100'
	movwf	Primary_Status
	clrf	Secondary_Status
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理




; 切换到闹钟设置模式
SwitchState_AlarmSet:
	movlw	B'00001000'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2AS						; 若当前非闹钟设置则只切换到闹钟设置
	
	incf	Secondary_Status,F			; 闹钟设置模式的子模式切换
	movlw	4
	subwf	Secondary_Status,W
	btfsc	C
	goto	SwitchState_AlarmDis		; 闹钟设置子模式溢出后会回到闹钟显示模式

	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理

SW2AS:
	movlw	B'00001000'
	movwf	Primary_Status
	clrf	Secondary_Status
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理





; 切换到日期设置模式
SwitchState_DateSet:
	movlw	B'00010000'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2DS						; 若当前非日期设置则只切换到日期设置
	
	incf	Secondary_Status,F			; 日期设置模式的子模式切换
	movlw	3
	subwf	Secondary_Status,W
	btfsc	C
	goto	SwitchState_ClockDis		; 日期设置子模式溢出后会回到时钟显示

	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理

SW2DS:
	movlw	B'00010000'
	movwf	Primary_Status
	clrf	Secondary_Status
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理




; 切换到夜间时段设置模式
SwitchState_NightPeriodSet:
	btfss	NightPeriod_Flag,0
	goto	L_KeyHandle_Over			; 如果夜间模式没有开启则不会进夜间时段设置模式

	movlw	B'00100000'
	andwf	Primary_Status,W
	btfsc	Z
	goto	SW2NS						; 若当前非夜间时段设置则只切换到夜间时段设置

	incf	Secondary_Status,F			; 夜间时段设置模式的子模式切换
	movlw	4
	subwf	Secondary_Status,W
	btfsc	C
	goto	SwitchState_ClockDis		; 夜间时段设置子模式溢出后会回到时钟显示

	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理

SW2NS:
	movlw	B'00100000'
	movwf	Primary_Status
	clrf	Secondary_Status
	goto	L_KeyHandle_Over			; 状态切换的功能会结束按键处理




;======================================================================================

; 根据时显的子模式选择切换至对应的设置模式
CD_SetMode_CHG:
	movlw	High(CD_Secondary_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
CD_Secondary_JumpTable:
	addwf	PCL,F
	goto	SwitchState_ClockSet		; 子模式为0，进时间设置
	goto	SwitchState_DateSet			; 子模式为1，进日期设置
	goto	L_KeyHandle_Over			; 子模式为2，没有状态切换直接退出按键处理
	goto	SwitchState_NightPeriodSet	; 子模式为3，进夜间时段设置
	goto	L_KeyHandle_Over			; 非法子模式值，退出按键处理


; 根据当前设置模式的状态选择切换哪个设置模式的子模式
SetMode_CHG:
	btfsc	Primary_Status,2
	goto	SwitchState_ClockSet		; 切换时间设置下的子模式
	btfsc	Primary_Status,4
	goto	SwitchState_DateSet			; 切换日期设置下的子模式
	btfsc	Primary_Status,5
	goto	SwitchState_NightPeriodSet	; 切换夜间时段设置下的子模式
	goto	L_KeyHandle_Over			; 非法值，退出按键处理


;======================================================================================

; 时设模式增数
AddNum_CS:
	movlw	High(CS_AddNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
CS_AddNumMode_JumpTable:
	addwf	PCL,F
	goto	Time_Mode_SW
	goto	Time_Hour_Add
	goto	Time_Min_Add
	goto	L_KeyHandle_Over

; 闹设模式增数
AddNum_AS:
	movlw	High(AS_AddNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AS_AddNumMode_JumpTable:
	addwf	PCL,F
	goto	Alarm_Hour_Add				; 闹钟小时设置
	goto	Alarm_Min_Add				; 闹钟分钟设置
	goto	Alarm_SNZ_Time_Add			; 闹钟贪睡时间设置
	goto	Alarm_WorkDay_CHG			; 工作日闹钟设置
	goto	L_KeyHandle_Over

; 日设模式增数
AddNum_DS:
	movlw	High(DS_AddNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DS_AddNumMode_JumpTable:
	addwf	PCL,F
	goto	Date_Year_Add				; 年份设置
	goto	Date_Month_Add				; 月份设置
	goto	Date_Day_Add				; 日期设置
	goto	L_KeyHandle_Over

; 夜设模式增数
AddNum_NS:
	movlw	High(NS_AddNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
NS_AddNumMode_JumpTable:
	addwf	PCL,F
	goto	NMode_Start_Hour_Add		; 夜间模式开始小时
	goto	NMode_Start_Min_Add			; 夜间模式开始分钟
	goto	NMode_End_Hour_Add			; 夜间模式结束小时
	goto	NMode_End_Min_Add			; 夜间模式结束分钟
	goto	L_KeyHandle_Over


; 时设模式减数
SubNum_CS:
	movlw	High(CS_SubNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
CS_SubNumMode_JumpTable:
	addwf	PCL,F
	goto	Time_Mode_SW
	goto	Time_Hour_Sub
	goto	Time_Min_Sub
	goto	L_KeyHandle_Over

; 闹设模式减数
SubNum_AS:
	movlw	High(AS_SubNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AS_SubNumMode_JumpTable:
	addwf	PCL,F
	goto	Alarm_Hour_Sub				; 闹钟小时设置
	goto	Alarm_Min_Sub				; 闹钟分钟设置
	goto	Alarm_SNZ_Time_Sub			; 闹钟贪睡时间设置
	goto	Alarm_WorkDay_CHG			; 工作日闹钟设置
	goto	L_KeyHandle_Over

; 日设模式减数
SubNum_DS:
	movlw	High(DS_SubNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DS_SubNumMode_JumpTable:
	addwf	PCL,F
	goto	Date_Year_Sub				; 年份设置
	goto	Date_Month_Sub				; 月份设置
	goto	Date_Day_Sub				; 日期设置
	goto	L_KeyHandle_Over

; 夜设模式减数
SubNum_NS:
	movlw	High(NS_SubNumMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
NS_SubNumMode_JumpTable:
	addwf	PCL,F
	goto	NMode_Start_Hour_Sub		; 夜间模式开始小时
	goto	NMode_Start_Min_Sub			; 夜间模式开始分钟
	goto	NMode_End_Hour_Sub			; 夜间模式结束小时
	goto	NMode_End_Min_Sub			; 夜间模式结束分钟
	goto	L_KeyHandle_Over

;======================================================================================

; 小时制切换
Time_Mode_SW:
	movlw	1
	xorwf	Clock_Flag,F				; 取反12/24h标志位

	call	F_Display_Time				; 刷新显示的时间
	goto	L_KeyHandle_Over			; 切换时间显示模式为压键生效

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
	call	F_Display_Time
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
	call	F_Display_Time
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
	call	F_Display_Time
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
	call	F_Display_Time
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
	call	L_DisDate_Year
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
	call	L_DisDate_Year
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
	call	F_Date_Display
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
	call	F_Date_Display
	return

; 日增加
Date_Day_Add:
	incf	R_Date_Day,F
	call	L_DayOverflow_To_1
	call	F_Date_Display
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
	call	F_Date_Display
	return


;======================================================================================

; 夜间模式开关
NMode_Swtich:
	movlw	1
	xorwf	NightPeriod_Flag

	btfsc	NightPeriod_Flag,0
	goto	Nmode_ON

	call	SetDayMode					; 关闭夜间模式时直接高亮
	goto	NMode_Swtich_Exit

Nmode_ON:
	call	TimeRange_Detect			; 根据当前夜间时段调整亮度

NMode_Swtich_Exit:
	call	F_Display_NightPeriod_SW
	goto	L_KeyHandle_Over			; 切换夜间模式开关压键生效


; 夜间模式开始时增加
NMode_Start_Hour_Add:
	movlw	23
	subwf	R_Night_StartHour,W
	btfsc	C
	goto	NMode_StartHour_AddOverflow
	incf	R_Night_StartHour,F
	goto	NMode_StartHour_Add_Exit
NMode_StartHour_AddOverflow:
	clrf	R_Night_StartHour
NMode_StartHour_Add_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_StartTime
	return

; 夜间模式开始时减少
NMode_Start_Hour_Sub:
	movf	R_Night_StartHour,F
	btfsc	Z
	goto	NMode_StartHour_SubOverflow
	decf	R_Night_StartHour,F
	goto	NMode_StartHour_Sub_Exit
NMode_StartHour_SubOverflow:
	movlw	23
	movwf	R_Night_StartHour
NMode_StartHour_Sub_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_StartTime
	return

; 夜间模式开始分增加
NMode_Start_Min_Add:
	movlw	59
	subwf	R_Night_StartMin,W
	btfsc	C
	goto	NMode_StartMin_AddOverflow
	incf	R_Night_StartMin,F
	goto	NMode_StartMin_Add_Exit
NMode_StartMin_AddOverflow:
	clrf	R_Night_StartMin
NMode_StartMin_Add_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_StartTime
	return

; 夜间模式开始分减少
NMode_Start_Min_Sub:
	movf	R_Night_StartMin,F
	btfsc	Z
	goto	NMode_StartMin_SubOverflow
	decf	R_Night_StartMin,F
	goto	NMode_StartMin_Sub_Exit
NMode_StartMin_SubOverflow:
	movlw	59
	movwf	R_Night_StartMin
NMode_StartMin_Sub_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_StartTime
	return


; 夜间模式结束时增加
NMode_End_Hour_Add:
	movlw	23
	subwf	R_Night_EndHour,W
	btfsc	C
	goto	NMode_EndHour_AddOverflow
	incf	R_Night_EndHour,F
	goto	NMode_EndHour_Add_Exit
NMode_EndHour_AddOverflow:
	clrf	R_Night_EndHour
NMode_EndHour_Add_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_EndTime
	return

; 夜间模式结束时减少
NMode_End_Hour_Sub:
	movf	R_Night_EndHour,F
	btfsc	Z
	goto	NMode_EndHour_SubOverflow
	decf	R_Night_EndHour,F
	goto	NMode_EndHour_Sub_Exit
NMode_EndHour_SubOverflow:
	movlw	23
	movwf	R_Night_EndHour
NMode_EndHour_Sub_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_EndTime
	return

; 夜间模式结束分增加
NMode_End_Min_Add:
	movlw	59
	subwf	R_Night_EndMin,W
	btfsc	C
	goto	NMode_EndMin_AddOverflow
	incf	R_Night_EndMin,F
	goto	NMode_EndMin_Add_Exit
NMode_EndMin_AddOverflow:
	clrf	R_Night_EndMin
NMode_EndMin_Add_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_EndTime
	return

; 夜间模式结束分减少
NMode_End_Min_Sub:
	movf	R_Night_EndMin,F
	btfsc	Z
	goto	NMode_EndMin_SubOverflow
	decf	R_Night_EndMin,F
	goto	NMode_EndMin_Sub_Exit
NMode_EndMin_SubOverflow:
	movlw	59
	movwf	R_Night_EndMin
NMode_EndMin_Sub_Exit:
	call	TimeRange_Detect
	call	F_Display_NMode_EndTime
	return


;======================================================================================

; 闹钟开关
Alarm_Switch:
	movlw	0x80
	xorwf	Clock_Flag,F				; 取反闹钟开关标志位

	movlw	led_al
	movwf	R_X
	btfsc	Clock_Flag,7
	goto	AlarmSwitch_OFF
	call	F_DisSymbol					; 亮ALARM点
	goto	L_KeyHandle_Over			; 闹钟开关是压键生效
AlarmSwitch_OFF:
	call	F_ClrSymbol					; 灭ALARM点
	goto	L_KeyHandle_Over			; 闹钟开关是压键生效

; 闹钟时增加
Alarm_Hour_Add:
	movlw	23
	subwf	R_Alarm_Hour,W
	btfsc	C
	goto	AlarmHour_AddOverflow
	incf	R_Alarm_Hour,F
	goto	AlarmHour_Add_Exit
AlarmHour_AddOverflow:
	clrf	R_Alarm_Hour
AlarmHour_Add_Exit:
	nop									; 夜间模式的处理
	call	F_Display_Alarm
	return

; 闹钟时减少
Alarm_Hour_Sub:
	movf	R_Alarm_Hour,F
	btfsc	Z
	goto	AlarmHour_SubOverflow
	decf	R_Alarm_Hour,F
	goto	AlarmHour_Sub_Exit
AlarmHour_SubOverflow:
	movlw	23
	movwf	R_Alarm_Hour
AlarmHour_Sub_Exit:
	nop									; 夜间模式的处理
	call	F_Display_Alarm
	return

; 闹钟分增加
Alarm_Min_Add:
	movlw	59
	subwf	R_Alarm_Min,W
	btfsc	C
	goto	AlarmMin_AddOverflow
	incf	R_Alarm_Min,F
	goto	AlarmMin_Add_Exit
AlarmMin_AddOverflow:
	clrf	R_Alarm_Min
AlarmMin_Add_Exit:
	call	F_Display_Alarm
	return

; 闹钟分减少
Alarm_Min_Sub:
	movf	R_Alarm_Min,F
	btfsc	Z
	goto	AlarmMin_SubOverflow
	decf	R_Alarm_Min,F
	goto	AlarmMin_Sub_Exit
AlarmMin_SubOverflow:
	movlw	59
	movwf	R_Alarm_Min
AlarmMin_Sub_Exit:
	call	F_Display_Alarm
	return

; 贪睡时间增加
Alarm_SNZ_Time_Add:
	movlw	60
	subwf	R_Snooze_Time,W
	btfsc	C
	goto	AlarmSNZ_AddOverflow
	incf	R_Snooze_Time,F
	goto	AlarmSNZ_Add_Exit
AlarmSNZ_AddOverflow:
	clrf	R_Snooze_Time
AlarmSNZ_Add_Exit:
	call	F_Display_SNZ
	return

; 贪睡时间减少
Alarm_SNZ_Time_Sub:
	movf	R_Snooze_Time,F
	btfsc	Z
	goto	AlarmSNZ_SubOverflow
	decf	R_Snooze_Time,F
	goto	AlarmSNZ_Sub_Exit
AlarmSNZ_SubOverflow:
	movlw	60
	movwf	R_Snooze_Time
AlarmSNZ_Sub_Exit:
	call	F_Display_SNZ
	return

; 变更工作日闹钟
Alarm_WorkDay_CHG:
	movf	8
	subwf	WorkDay_Alarm
	btfsc	C
	goto	WorkDay_Overflow

	bcf		C
	rlf		WorkDay_Alarm,F
	goto	Alarm_WorkDay_CHG_Exit
WorkDay_Overflow:
	movlw	1
	movwf	WorkDay_Alarm
Alarm_WorkDay_CHG_Exit:
	call	F_AlarmWorkDay_Dis
	goto	L_KeyHandle_Over


;======================================================================================

; 切换温度单位
TemperMode_Change:
	movlw	2
	xorwf	ADC_Flag					; 取反温度单位标志位，切换显示的温度单位
	goto	L_KeyHandle_Over


; 调整亮度
BackLight_Level_CHG:
	movlw	High(BackLight_Level_JumpTable)
	movwf	PCLATH
	movf	BackLight_Level,W			; 通过BackLight Level值来跳转
BackLight_Level_JumpTable:
	addwf	PCL,F
	goto	BackLight_Level0
	goto	BackLight_Level1
	goto	BackLight_Level2
BackLight_Level0:						; 当前亮度为低亮，下一亮度为高亮
	movlw	2
	movwf	BackLight_Level
	movlw	0xe8
	movwf	PWM0DL						; 调整占空比，先设置低位
	movlw	0x03
	movwf	PWM0DH
	goto	L_KeyHandle_Over
BackLight_Level1:						; 当前亮度为半亮，下一亮度为低亮
	movlw	0
	movwf	BackLight_Level
	movlw	0x4e
	movwf	PWM0DL						; 调整占空比，先设置低位
	movlw	0x01
	movwf	PWM0DH
	goto	L_KeyHandle_Over
BackLight_Level2:						; 当前亮度为高亮，下一亮度为半亮
	movlw	1
	movwf	BackLight_Level
	movlw	0x9b
	movwf	PWM0DL						; 调整占空比，先设置低位
	movlw	0x02
	movwf	PWM0DH
	goto	L_KeyHandle_Over


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
