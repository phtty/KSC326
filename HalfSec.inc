HalfSec_Handle:
	btfsc	Time_Flag,1
	goto	HalfSec_Start					; 2Hz处理
	return
HalfSec_Start:
	bcf		Time_Flag,1						; 复位2Hz标志

	btfsc	Time_Flag,0
	goto	Sec_Start						; 1Hz处理

; 一些刷新程序

	movlw	High(Sys_HalfSec_Table)
	movwf	PCLATH
	movf	Primary_Status,W
Sys_HalfSec_Table:
	addwf	PCL,F
	goto	F_TimeDis_HalfSec
	goto	F_DateDis_HalfSec
	goto	F_TemperDis_HalfSec
	goto	F_AlarmDis_HalfSec
	goto	F_TimeSet_HalfSec
	goto	F_DateSet_HalfSec
	goto	F_AlarmSet_HalfSec
	return

Sec_Start:
	bcf		Time_Flag,0						; 复位1Hz标志

; 还有一些加时1S的程序
	call	F_Time_Run						; 走时
	call	LoudManage						; 响闹管理

	movlw	High(Sys_Sec_Table)
	movwf	PCLATH
	movf	Primary_Status,W
Sys_Sec_Table:
	addwf	PCL,F
	goto	F_TimeDis_Sec
	goto	F_DateDis_Sec
	goto	F_TemperDis_Sec
	goto	F_AlarmDis_Sec
	goto	F_TimeSet_Sec
	goto	F_DateSet_Sec
	goto	F_AlarmSet_Sec
	return



F_TimeDis_HalfSec:


