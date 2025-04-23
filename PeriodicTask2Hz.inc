; 2Hz处理
PeriodicTask_2Hz:
	btfss	Time_Flag,1
	return
	bcf		Time_Flag,1						; 复位2Hz标志

; 一些2Hz刷新程序

	movlw	High(Sys_HalfSec_Table)
	movwf	PCLATH
	movf	Primary_Status,W
Sys_HalfSec_Table:
	addwf	PCL,F
	goto	F_TimeDis_HalfSec
	goto	F_DateDis_HalfSec
	goto	F_TemperDis_HalfSec
	goto	F_Alarm1Dis_HalfSec
	goto	F_Alarm2Dis_HalfSec
	goto	F_Alarm3Dis_HalfSec
	return
