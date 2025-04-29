; 若有刷新显示标志位，则执行一次显示刷新
Display_Reflash:
	btfss	SysEvent_Flag,0
	return
	bcf		SysEvent_Flag,0

	call	Send_RGB_Data
	return




; 时间显示
Time_Display:
	call	TimeMin_Display
	goto	TimeHour_Display

; 分钟显示
TimeMin_Display:
	movf	R_Time_Min,W
	call	Hex2BCD
	
	movlw	3
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示分钟个位

	movlw	2
	movwf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display				; 显示分钟个位
	REFLASH_DISPLAY
	return

; 小时显示
TimeHour_Display:
	btfss	Clock_Flag,0
	goto	Time_24hMode

	movlw	12
	subwf	R_Time_Hour,W					; 判断当前时间是上午或下午
	btfsc	C
	goto	Time12h_PM

	call	SymbolPM_Clear					; 上午需灭PM点

	movf	R_Time_Hour,W					; 再判断是否在0点
	btfsc	Z
	goto	Time_0Hour
	goto	Start_DisTime_Hour
Time12h_PM:
	call	SymbolPM_Display				; 下午亮PM点
	movlw	12
	subwf	R_Time_Hour,W					; 调整为12h模式显示
	btfss	Z
	goto	Start_DisTime_Hour
Time_0Hour:
	movlw	12								; 0点在12h模式里是12点
	goto	Start_DisTime_Hour

Time_24hMode:
	call	SymbolPM_Clear					; 24h模式下没有PM点
	movf	R_Time_Hour,W
Start_DisTime_Hour:
	call	Hex2BCD							; 显示最终的结果
	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	clrf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display
	REFLASH_DISPLAY
	return




; 小时制显示
TimeMode_Display:
	btfsc	Clock_Flag,0
	goto	TimeMode_12H

	movlw	1
	movwf	R_X
	movlw	4
	call	Digit_Num_Display

	clrf	R_X
	movlw	2
	call	Digit_Num_Display				; 24小时制显示24
	goto	Hr_Display
TimeMode_12H:
	movlw	1
	movwf	R_X
	movlw	2
	call	Digit_Num_Display

	clrf	R_X
	movlw	1
	call	Digit_Num_Display				; 12小时制显示12
Hr_Display:
	movlw	3
	movwf	R_X
	movlw	Word_H
	call	Digit_Word_Display

	movlw	2
	movwf	R_X
	movlw	Word_r
	call	Digit_Word_Display				; 显示Hr
	REFLASH_DISPLAY
	return




; 日期显示
Date_Display:
	call	Day_Display
	goto	Month_Display

; 日显示
Day_Display:
	movf	R_Date_Day,W
	call	Hex2BCD
	movlw	3
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	movlw	2
	movwf	R_X
	movf	P_Temp6,W
	btfss	Z
	goto	Day_Tens_NoZero
	movlw	10
Day_Tens_NoZero:
	call	Digit_Num_Display
	REFLASH_DISPLAY
	return

; 月显示
Month_Display:
	movf	R_Date_Month,W
	call	Hex2BCD
	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	clrf	R_X
	movf	P_Temp6,W
	btfss	Z
	goto	Month_Tens_NoZero
	movlw	10
Month_Tens_NoZero:
	call	Digit_Num_Display
	REFLASH_DISPLAY
	return



; 年份显示
Year_Display:
	movf	R_Date_Year,W
	call	Hex2BCD
	movlw	3
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	movlw	2
	movwf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display

	movlw	1
	movwf	R_X
	movlw	0
	call	Digit_Num_Display

	clrf	R_X
	movlw	2
	call	Digit_Num_Display
	REFLASH_DISPLAY
	return


Temper_Display:
	call	SymbolTEMP_Display				; 显示温度符号

	btfss	ADC_Flag,1
	goto	Display_CelsiusDegree
	goto	Display_FahrenheitDegree

Display_CelsiusDegree:
	btfsc	ADC_Flag,0
	goto	CelsiusDegree_Minus				; 摄氏度存在负号时使用另一套显示逻辑

	movf	R_Temperature,W
	call	Hex2BCD
	clrf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display				; 显示温度十位

	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示温度个位

	movlw	2
	movwf	R_X
	movlw	Word_C
	call	Digit_Word_Display				; D2显示温标C
	REFLASH_DISPLAY
	return

CelsiusDegree_Minus:
	clrf	R_X
	movlw	Word_Neg
	call	Digit_Word_Display				; D0显示负号

	movf	R_Temperature,W
	call	Hex2BCD
	movlw	1
	movwf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display				; 显示温度十位

	movlw	2
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示温度个位

	movlw	3
	movwf	R_X
	movlw	Word_C
	call	Digit_Word_Display				; D3显示温标C
	REFLASH_DISPLAY
	return


Display_FahrenheitDegree:
	movf	R_Temperature_F,W
	call	Hex2BCD
	movf	P_Temp7
	btfss	Z
	goto	CelsiusDegree_3Digit			; 华氏度存在百位数时使用另一套显示逻辑

	clrf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display				; 显示温度十位

	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示温度个位

	movlw	2
	movwf	R_X
	movlw	Word_C
	call	Digit_Word_Display				; D2显示温标F
	REFLASH_DISPLAY
	return

CelsiusDegree_3Digit:
	clrf	R_X
	movf	P_Temp7,W
	call	Digit_Num_Display				; 显示温度百位

	movlw	1
	movwf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display				; 显示温度十位

	movlw	2
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示温度个位

	movlw	3
	movwf	R_X
	movlw	Word_C
	call	Digit_Word_Display				; D3显示温标F
	REFLASH_DISPLAY
	return




; 根据当前显示模式，显示闹钟组n的开关
AlarmSW_Dipalay:
	movlw	3
	subwf	Primary_Status,W
	movwf	R_X
	movlw	1
	call	W_LeftShift_XBit				; 将当前的显示状态变为操作的闹钟组
	andwf	Alarm_Switch,W
	btfss	Z
	goto	AlarmSW_DisON

	clrf	R_X
	movlw	Word_Neg
	call	Digit_Word_Display				; 闹钟关闭显示-

	movlw	1
	movwf	R_X
	movf	Word_Neg
	call	Digit_Word_Display				; 闹钟关闭显示-
	goto	Display_AlarmNumber
AlarmSW_DisON:
	clrf	R_X
	movlw	Word_o
	call	Digit_Word_Display				; 闹钟开启显示o

	movlw	1
	movwf	R_X
	movf	Word_n
	call	Digit_Word_Display				; 闹钟开启显示n

Display_AlarmNumber:
	movlw	2
	movwf	R_X
	movf	Word_A
	call	Digit_Word_Display				; 显示A

	movlw	3
	movwf	R_X
	movlw	3
	subwf	Primary_Status,W
	call	Digit_Num_Display				; 显示闹钟序号
	REFLASH_DISPLAY
	return



; 显示闹钟
Alarm_Dipalay:
	call	AlarmMin_Display
	goto	AlarmHour_Display

; 闹钟分钟显示
AlarmMin_Display:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp							; 计算出闹钟组
	movlw	Low(R_Alarm1_Min)
	addwf	P_Temp,W
	movwf	FSR0L
	movlw	High(R_Alarm1_Min)
	movwf	FSR0H

	movf	INDF0,W
	call	Hex2BCD

	movlw	2
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display				; 显示分钟十位

	movf	P_Temp6,W
	movwf	P_Temp
	movlw	3
	movwf	R_X
	call	Digit_Num_Display				; 显示分钟个位
	return

; 闹钟小时显示
AlarmHour_Display:
	movlw	3
	subwf	Primary_Status,W
	movwf	P_Temp							; 计算出闹钟组
	movlw	Low(R_Alarm1_Min)
	addwf	P_Temp,W
	movwf	FSR0L
	movlw	High(R_Alarm1_Min)
	movwf	FSR0H

	btfss	Clock_Flag,0
	goto	Alarm_24hMode

	movlw	12
	subwf	INDF0,W							; 判断闹钟时间是上午或下午
	btfsc	C
	goto	Alarm12h_PM

	call	SymbolPM_Clear					; 上午需灭PM点

	movf	INDF0,W							; 再判断是否在0点
	btfsc	Z
	goto	Alarm_0Hour
	goto	Start_DisAlarm_Hour
Alarm12h_PM:
	call	SymbolPM_Display				; 下午亮PM点
	movlw	12
	subwf	INDF0,W							; 调整为12h模式显示
	btfss	Z
	goto	Start_DisAlarm_Hour
Alarm_0Hour:
	movlw	12								; 0点在12h模式里是12点
	goto	Start_DisAlarm_Hour

Alarm_24hMode:
	call	SymbolPM_Clear					; 24h模式下没有PM点
	movf	INDF0,W
Start_DisAlarm_Hour:
	call	Hex2BCD							; 显示闹钟小时
	movlw	1
	movwf	R_X
	movf	P_Temp5,W
	call	Digit_Num_Display

	clrf	R_X
	movf	P_Temp6,W
	call	Digit_Num_Display
	REFLASH_DISPLAY
	return




; 取消显示
Undisplay_All:
	call	Undisplay_01
	goto	Undisplay_23

; 取消显示Digit0、1
Undisplay_01:
	movlw	0
	movwf	R_X
	movlw	10
	call	Digit_Num_Display				; 取消显示Digit0

	movlw	1
	movwf	R_X
	movlw	10
	call	Digit_Num_Display				; 取消显示Digit1
	REFLASH_DISPLAY
	return

; 取消显示Digit2、3
Undisplay_23:
	movlw	2
	movwf	R_X
	movlw	10
	call	Digit_Num_Display				; 取消显示Digit2

	movlw	3
	movwf	R_X
	movlw	10
	call	Digit_Num_Display				; 取消显示Digit3
	REFLASH_DISPLAY
	return




; W==要转化的值
; P_Temp5、6、7为个、十、百位
Hex2BCD:
	movwf	P_Temp
	clrf	P_Temp+5
	clrf	P_Temp+6
	clrf	P_Temp+7
LOOP_HUND:
	movlw	D'100'
	subwf	P_Temp,W					; W = BIN - 100
	btfss	C
	goto	LOOP_TEN					; 不够减，退出循环
	movwf	P_Temp						; 保存减法结果到BIN
	incf	P_Temp7,F					; 百位加1
	goto	LOOP_HUND
LOOP_TEN:								; 计算十位
	movlw	D'10'
	subwf	P_Temp,W					; W = BIN - 10
	btfss	C
	goto	END_TEN
	movwf	P_Temp						; 保存减法结果到BIN
	incf 	P_Temp6,F					; 十位加1
	goto 	LOOP_TEN
END_TEN:
	movf	P_Temp,W					; 剩余值即为个位
	movwf	P_Temp5
	return



; 将W左移R_X位
W_LeftShift_XBit:
	movwf	P_Temp
LeftShift_Loop:
	movf	R_X,F
	btfsc	Z
	goto	LeftShift_LoopOver
	decf	R_X,F
	bcf		C
	rlf		P_Temp,F
	goto	LeftShift_Loop
LeftShift_LoopOver:
	movf	P_Temp,W
	return
