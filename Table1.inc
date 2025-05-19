	org		0c00H
DigitSet_SegA:
	movlw	High(SegA_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegA_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegA
	goto	Digit1Set_SegA
	goto	Digit2Set_SegA
	goto	Digit3Set_SegA


DigitSet_SegB:
	movlw	High(SegB_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegB_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegB
	goto	Digit1Set_SegB
	goto	Digit2Set_SegB
	goto	Digit3Set_SegB


DigitSet_SegC:
	movlw	High(SegC_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegC_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegC
	goto	Digit1Set_SegC
	goto	Digit2Set_SegC
	goto	Digit3Set_SegC


DigitSet_SegD:
	movlw	High(SegD_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegD_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegD
	goto	Digit1Set_SegD
	goto	Digit2Set_SegD
	goto	Digit3Set_SegD


DigitSet_SegE:
	movlw	High(SegE_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegE_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegE
	goto	Digit1Set_SegE
	goto	Digit2Set_SegE
	goto	Digit3Set_SegE


DigitSet_SegF:
	movlw	High(SegF_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegF_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegF
	goto	Digit1Set_SegF
	goto	Digit2Set_SegF
	goto	Digit3Set_SegF


DigitSet_SegG:
	movlw	High(SegG_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegG_JumpTable:
	addwf	PCL,F
	goto	Digit0Set_SegG
	goto	Digit1Set_SegG
	goto	Digit2Set_SegG
	goto	Digit3Set_SegG



DigitClr_SegA:
	movlw	High(SegAClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegAClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegA
	goto	Digit1Clr_SegA
	goto	Digit2Clr_SegA
	goto	Digit3Clr_SegA


DigitClr_SegB:
	movlw	High(SegBClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegBClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegB
	goto	Digit1Clr_SegB
	goto	Digit2Clr_SegB
	goto	Digit3Clr_SegB


DigitClr_SegC:
	movlw	High(SegCClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegCClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegC
	goto	Digit1Clr_SegC
	goto	Digit2Clr_SegC
	goto	Digit3Clr_SegC


DigitClr_SegD:
	movlw	High(SegDClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegDClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegD
	goto	Digit1Clr_SegD
	goto	Digit2Clr_SegD
	goto	Digit3Clr_SegD


DigitClr_SegE:
	movlw	High(SegEClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegEClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegE
	goto	Digit1Clr_SegE
	goto	Digit2Clr_SegE
	goto	Digit3Clr_SegE


DigitClr_SegF:
	movlw	High(SegFClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegFClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegF
	goto	Digit1Clr_SegF
	goto	Digit2Clr_SegF
	goto	Digit3Clr_SegF


DigitClr_SegG:
	movlw	High(SegGClr_JumpTable)
	movwf	PCLATH
	movf	R_X,W
SegGClr_JumpTable:
	addwf	PCL,F
	goto	Digit0Clr_SegG
	goto	Digit1Clr_SegG
	goto	Digit2Clr_SegG
	goto	Digit3Clr_SegG


;======================================================================================


Digit_NumTable:
	addwf	PCL,F
	retlw	D_Char_0
	retlw	D_Char_1
	retlw	D_Char_2
	retlw	D_Char_3
	retlw	D_Char_4
	retlw	D_Char_5
	retlw	D_Char_6
	retlw	D_Char_7
	retlw	D_Char_8
	retlw	D_Char_9
	retlw	D_Char_Null


Word_NumTable:
	addwf	PCL,F
	retlw	D_Char_C
	retlw	D_Char_F
	retlw	D_Char_A
	retlw	D_Char_o
	retlw	D_Char_n
	retlw	D_Char_Neg
	retlw	D_Char_d
	retlw	D_Char_p
	retlw	D_Char_H
	retlw	D_Char_r


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


DateDis_HalfSec:
	movlw	High(DateMode_HS_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DateMode_HS_JumpTable:
	addwf	PCL,F
	goto	Date_HS_Display
	goto	Year_HS_Display
	goto	Date_HS_Display
	goto	Date_HS_Display
	return


DateDis_Sec:
	movlw	High(DateMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
DateMode_JumpTable:
	addwf	PCL,F
	goto	Date_Sec_Display
	goto	Year_Sec_Display
	goto	DateMonth_HS_Display
	goto	DateDay_HS_Display
	return


;======================================================================================


AlarmDis_HalfSec:
	movlw	High(AlarmMode_HS_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AlarmMode_HS_JumpTable:
	addwf	PCL,F
	goto	AlarmSW_HS_Display
	goto	Alarm_HS_Display
	goto	Alarm_HS_Display


AlarmDis_Sec:
	movlw	High(AlarmMode_JumpTable)
	movwf	PCLATH
	movf	Secondary_Status,W
AlarmMode_JumpTable:
	addwf	PCL,F
	goto	AlarmSW_Sec_Display
	goto	AlarmHour_Sec_Display
	goto	AlarmMin_Sec_Display


;======================================================================================


DPMode_Mangage:
	movf	Secondary_Status,F				; 设置模式下不进行DP切换
	btfss	Z
	return

	btfsc	SysEvent_Flag,1					; 存在DP显示时，不递增DP计数
	return

	movlw	High(DPMode_JumpTable)
	movwf	PCLATH
	movf	DP_Mode,W
DPMode_JumpTable:
	addwf	PCL,F
	goto	DPMode_1
	goto	DPMode_2
	goto	DPMode_3
