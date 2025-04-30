	org		0bc5H
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


;======================================================================================

; 时间模式半S程序
TimeDis_HalfSec:
	btfsc	SysEvent_Flag,1						; 显示模式下，有DP标志时不显示时间
	return

	movlw	High(TimeMode_HS_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
TimeMode_HS_JumpTable:
	addwf	PCL,F
	goto	Time_Display
	goto	TimeMode_Display
	goto	Time_Display
	goto	Time_Display



; 时间模式1S程序
TimeDis_Sec:
	movlw	High(TimeMode_JumpTable)			; 1S灭
	movwf	PCLATH
	movf	Secondary_Status,W
TimeMode_JumpTable:
	addwf	PCL,F
	return
	goto	Undisplay_All
	goto	Undisplay_01
	goto	Undisplay_23


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
