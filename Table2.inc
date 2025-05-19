	org		0b00H
DPMode_1:
	movlw	3
	subwf	Primary_Status,W
	btfsc	C
	goto	DP1_OtherDis						; 非时间、日期、温度模式

	movlw	High(DP1_JumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
DP1_JumpTable:
	addwf	PCL,F
	goto	DP1_TimeDis
	goto	DP1_DateDis
	goto	DP1_TemperDis


;======================================================================================


; 闹钟开关
Alarm_SetSwitch:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp

	movlw	High(AlarmSW_JumpTable)
	movwf	PCLATH
	movf	P_Temp,W							; 根据当前闹钟组开关闹钟
AlarmSW_JumpTable:
	addwf	PCL,F
	goto	Alarm1SW
	goto	Alarm2SW
	goto	Alarm3SW


;======================================================================================

; 时间模式半S程序
TimeDis_HalfSec:
	movlw	High(TimeMode_HS_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
TimeMode_HS_JumpTable:
	addwf	PCL,F
	goto	Time_HS_Display
	goto	TimeMode_HS_Display
	goto	Time_HS_Display
	goto	Time_HS_Display



; 时间模式1S程序
TimeDis_Sec:
	movlw	High(TimeMode_JumpTable)			; 1S灭
	movwf	PCLATH
	movf	Secondary_Status,W
TimeMode_JumpTable:
	addwf	PCL,F
	goto	Time_Sec_Display
	goto	TimeMode_Sec_Display
	goto	TimeHour_HS_Display
	goto	TimeMin_HS_Display


;======================================================================================


HalfSec_Jump:
	movlw	High(HalfSec_2HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
HalfSec_2HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_HalfSec
	goto	DateDis_HalfSec
	goto	TemperDis_HalfSec
	goto	AlarmDis_HalfSec
	goto	AlarmDis_HalfSec
	goto	AlarmDis_HalfSec


Sec_Jump:
	movlw	High(Sec_2HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
Sec_2HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_Sec
	goto	DateDis_Sec
	return
	goto	AlarmDis_Sec
	goto	AlarmDis_Sec
	goto	AlarmDis_Sec


;======================================================================================


; 温度测量
Temperature_Get:
	movf	Temper_Serial,W
	sublw	5
	movwf	P_Temp

	movlw	High(TemperTest_JumpTable)
	movwf	PCLATH
	movf	P_Temp,W
TemperTest_JumpTable:
	addwf	PCL,F
	goto	TemperGet_Init
	return
	goto	TemperGet_Start
	return
	goto	TemperGet_Calculate
	return


;======================================================================================


; RGB模式切换
RGB_ModeSwitch:
	movlw	High(RGB_ModeJumpTable)
	movwf	PCLATH
	movf	RGB_Mode,W
RGB_ModeJumpTable:
	addwf	PCL,F
	goto	SingleColor_White
	goto	SingleColor_Red
	goto	SingleColor_Yellow
	goto	SingleColor_Green
	goto	SingleColor_Cyan
	goto	SingleColor_Blue
	goto	SingleColor_Purple
	goto	DigitalAlter_Horizon_Fixed
	goto	DigitalAlter_Vertical_Fixed
	goto	ChaosColor_Fixed
	goto	SingleColor_Rolling
	goto	ChaosColor_Rolling
	goto	DigitalAlter_Horizon_Rolling
	goto	DigitalAlter_Vertical_Rolling
	goto	SingleColor_Change




; 8字表，用于横向滚动变色
RGBTable_HorizonRolling:
	addwf	PCL,F
	retlw	27
	retlw	28
	retlw	22
	retlw	23
	retlw	24
	retlw	26
	retlw	25
	retlw	30
	retlw	31
	retlw	19
	retlw	20
	retlw	21
	retlw	29
	retlw	32
	retlw	0
	retlw	1
	retlw	18
	retlw	4
	retlw	5
	retlw	14
	retlw	15
	retlw	16
	retlw	3
	retlw	13
	retlw	2
	retlw	17
	retlw	7
	retlw	8
	retlw	10
	retlw	11
	retlw	12
	retlw	6
	retlw	9


IncTable_HorizonRolling:
	addwf	PCL,F
	retlw	7
	retlw	10
	retlw	9
	retlw	7


RGBTable_VerticalRolling:
	addwf	PCL,F
	retlw	27
	retlw	30
	retlw	4
	retlw	7
	retlw	26
	retlw	28
	retlw	29
	retlw	31
	retlw	0
	retlw	1
	retlw	2
	retlw	3
	retlw	5
	retlw	6
	retlw	8
	retlw	25
	retlw	32
	retlw	13
	retlw	9
	retlw	24
	retlw	22
	retlw	21
	retlw	19
	retlw	18
	retlw	17
	retlw	16
	retlw	14
	retlw	12
	retlw	10
	retlw	23
	retlw	20
	retlw	15
	retlw	11



IncTable_VerticalRolling:
	addwf	PCL,F
	retlw	4
	retlw	11
	retlw	4
	retlw	10
	retlw	4



; 乱彩颜色时刻表
RGBTable_ChaosColor:
	addwf	PCL,F
	retlw	0	; PM
	retlw	40	; COL1
	retlw	80	; AL
	retlw	200	; 3f
	retlw	80	; 3a
	retlw	241	; 3b
	retlw	120	; 4f
	retlw	160	; 4a
	retlw	200	; 4b
	retlw	80	;4gf
	retlw	0	; 4c
	retlw	40	; 4d
	retlw	241	; 4e
	retlw	120	; 3g
	retlw	160	; 3c
	retlw	0	; 3d
	retlw	40	; 3e
	retlw	160	; TEMP
	retlw	120	; COL2
	retlw	0	; 2c
	retlw	40	; 2d
	retlw	200	; 2e
	retlw	80	; 1c
	retlw	120	; 1d
	retlw	160	; 1e
	retlw	241	; 1g
	retlw	200	; 1f
	retlw	0	; 1a
	retlw	40	; 1b
	retlw	80	; 2f
	retlw	241	; 2a
	retlw	160	; 2b
	retlw	120 ; 2g

; 平年的每月份天数表
Table_Month_Common:
	db	31	; January
	db	28	; February
	db	31	; March
	db	30	; April
	db	31	; May
	db	30	; June
	db	31	; July
	db	31	; August
	db	30	; September
	db	31	; October
	db	30	; November
	db	31	; December

; 闰年的每月份天数表
Table_Month_Leap:
	db	31	; January
	db	29	; February
	db	31	; March
	db	30	; April
	db	31	; May
	db	30	; June
	db	31	; July
	db	31	; August
	db	30	; September
	db	31	; October
	db	30	; November
	db	31	; December
