Temperature_Get:
	clrf	R_ADC_H
	clrf	R_ADC_L

	bcf		GIE
	bsf		PORTF,1					; AD测温电路启动
	call	ADC_Initial				; 初始化ADC
	nop
	nop
	bsf		ADON
ADC_Wait:
	btfsc	ADON
	goto	ADC_Wait

	movf	ADRESH,W
	movwf	R_ADC_H
	movf	ADRESL,W
	movwf	R_ADC_L
	call	ADC_Deinitial			; 避免漏电去初始化ADC
	bcf		GIE
	bcf		PORTF,1					; AD测温电路断开

	swapf	R_ADC_L,F				; 采样值要右移4位
	movlw	0x0f
	andwf	R_ADC_H,W
	movwf	P_Temp
	swapf	P_Temp,W
	iorwf	R_ADC_L,F
	movlw	0xf0
	andwf	R_ADC_H,F
	swapf	R_ADC_H,F

TEST_LABEL:
	movlw	59
	movwf	R_Temperature			; 温度通过循环查表递减得出
	movlw	0xb0
	subwf	R_ADC_L,F
	btfsc	C
	goto	TemperTableHead_NoBorrow
	movlw	1
	subwf	R_ADC_H,F
	btfss	C
	goto	TemperLoop_Over			; 有借位则说明超过下限
TemperTableHead_NoBorrow:
	movlw	0x04
	subwf	R_ADC_H,F
	btfss	C						; 有借位则说明超过下限
	goto	TemperLoop_Over

TemperLoop_Start:
	movlw	High(Temper_Table)		; 循环查表得出当前温度值
	movwf	PMADRH
	movlw	Low(Temper_Table)
	addwf	R_Temperature,W
	movwf	PMADRL
	bsf		RDON
	nop
	nop

	movf	PMDATH,W
	subwf	R_ADC_L,F
	btfsc	C
	goto	ADCL_NoBorrow
	movlw	1
	subwf	R_ADC_H,F				; 低8位借位，高8位-1
	btfss	C
	goto	TemperLoop_Over			; 高8位出现借位，则说明温度值已经取完了
ADCL_NoBorrow:
	decfsz	R_Temperature,F			; 下限检测，超过下限则不继续处理	
	goto	TemperLoop_Start

TemperLoop_Over:
	movlw	9
	subwf	R_Temperature,W
	btfss	C
	goto	Minus_Temper
	movwf	R_Temperature
	bcf		ADC_Flag,0				; 清掉负温度标志位
	goto	Temperature_Get_Exit
Minus_Temper:
	movf	R_Temperature,W
	sublw	10
	movwf	R_Temperature
	bsf		ADC_Flag,0
Temperature_Get_Exit:
	call	Cel2Fah
	return




; 通过摄氏度计算出华氏度
Cel2Fah:
	bcf		RP0
	clrf	R_Temperature_F
	clrf	P_Temp
	clrf	P_Temp1					; 变量初始化
	movf	R_Temperature,W
	movwf	P_Temp

	bcf		C
	rlf		P_Temp,F
	rlf		P_Temp1,F
	bcf		C
	rlf		P_Temp,F
	rlf		P_Temp1,F
	bcf		C
	rlf		P_Temp,F
	rlf		P_Temp1,F				; 左移三次乘8

	movf	R_Temperature,W
	addwf	P_Temp,F
	btfss	C
	goto	Div5_Loop				; 无进位
	movlw	1
	addwf	P_Temp1,F				; 再加上自身完成x9操作

Div5_Loop:
	movlw	5
	subwf	P_Temp,F				; 循环-5
	btfsc	C
	goto	Div5_NoBorrow
	movlw	1
	subwf	P_Temp1,F				; 低8位借位，高8位-1
	btfss	C
	goto	Div5_Loop_Over			; 高8位借位则是不够减了，退出循环
Div5_NoBorrow:
	incf	R_Temperature_F,F
	goto	Div5_Loop
Div5_Loop_Over:
	btfsc	ADC_Flag,0
	goto	MinusTemper2Fah
	movlw	32
	addwf	R_Temperature_F,F
	return
MinusTemper2Fah:
	movf	R_Temperature_F,W
	sublw	32
	movwf	R_Temperature_F
	return




; 15S间隔测温
Temperature_Test:
	decfsz	TemperTest_Counter,F
	return

	movlw	15
	movwf	TemperTest_Counter		; 复位15S测温间隔计时
	call	Temperature_Get
	return




TemperDis_HalfSec:
	goto	Temper_Display
