; 2Hz处理
PeriodicTask_2Hz:
	btfss	Time_Flag,1
	return
	bcf		Time_Flag,1						; 复位2Hz标志

; 一些2Hz刷新程序

	movlw	High(PeriodicTask_2HzJumpTable)
	movwf	PCLATH
	movf	Primary_Status,W
PeriodicTask_2HzJumpTable:
	addwf	PCL,F
	goto	TimeDis_HalfSec
	goto	DateDis_HalfSec
	goto	TemperDis_HalfSec
	goto	Alarm1Dis_HalfSec
	goto	Alarm2Dis_HalfSec
	goto	Alarm3Dis_HalfSec
	return
