; 32Hz里调用温度测量的处理并递减测温序列
TemperTest_Handler:
	call	Temperature_Get
	movf	Temper_Serial,f			; 测温序列递减到0则不继续递减
	btfss	Z
	decf	Temper_Serial,F
	return




; 初始化AD
TemperGet_Init:
	clrf	R_ADC_H
	clrf	R_ADC_L

	bsf		PORTF,1					; AD测温电路启动
	goto	ADC_Initial				; 初始化ADC




; AD测量
TemperGet_Start:
	bcf		GIE						; 测温时关闭中断

	bsf		ADON
	btfsc	ADON
	goto	$-1

	movf	ADRESH,W
	movwf	R_ADC_H
	movf	ADRESL,W
	movwf	R_ADC_L
	call	ADC_Deinitial			; 避免漏电去初始化ADC
	bcf		PORTF,1					; AD测温电路断开
	bsf		GIE						; 测温完成重新开启中断
	return




; 通过AD采样值计算温度
TemperGet_Calculate:
	swapf	R_ADC_L,F				; 采样值要右移4位
	movlw	0x0f
	andwf	R_ADC_H,W
	movwf	P_Temp
	swapf	P_Temp,W
	iorwf	R_ADC_L,F
	movlw	0xf0
	andwf	R_ADC_H,F
	swapf	R_ADC_H,F

	movlw	90
	movwf	R_Temperature			; 温度通过循环查表递减得出
	movlw	0x4a
	subwf	R_ADC_L,F
	btfsc	C
	goto	TemperTableHead_NoBorrow
	movlw	1
	subwf	R_ADC_H,F
	btfss	C
	goto	TemperLoop_Over			; 有借位则说明超过上限
TemperTableHead_NoBorrow:
	movlw	0x03
	subwf	R_ADC_H,F
	btfss	C						; 有借位则说明超过上限
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
	call	Compensation			; 温度补偿

	movlw	25
	subwf	R_Temperature,W
	btfss	C
	goto	Minus_Temper
	movwf	R_Temperature
	bcf		ADC_Flag,0				; 清掉负温度标志位
	goto	Temperature_Get_Exit
Minus_Temper:
	movf	R_Temperature,W
	sublw	25
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
	movlw	5
	movwf	Temper_Serial			; 开始测温序列
	return




TemperDis_HalfSec:
	call	Temper_Display
	goto	TemperSymbol_Display




; 根据时间进行温度补偿
Compensation:
	clrf	P_Temp

	movf	Light_Level,F
	btfsc	Z
	goto	HighLightCompen

LowLightLoop_Start:
;	movlw	High(LowLightCompen_Table)
;	movwf	PCLATH
;	movf	P_Temp,W
;	call	LowLightCompen_Table			; 用当前的补偿时间循环查表，得到补偿等级
;
;	subwf	CompTime_Count,W
;	btfss	C
;	goto	LowLightLoop_Over
;	incf	P_Temp,F
;	goto	LowLightLoop_Start
;LowLightLoop_Over:
	movf	CompTime_Count,F
	btfsc	Z
	goto	TemperOverflow_Juge		; 低亮时若补偿时间为0则不进行补偿
	movlw	5
	addwf	P_Temp,F				; 有补偿时间则固定补偿5度
	goto	TemperOverflow_Juge

HighLightCompen:
	movlw	High(HighLightCompen_Table)
	movwf	PCLATH
	movf	P_Temp,W
	call	HighLightCompen_Table	; 用当前的补偿时间循环查表，得到补偿等级

	subwf	CompTime_Count,W
	btfss	C
	goto	HighLightLoop_Over
	incf	P_Temp,F
	goto	HighLightCompen
HighLightLoop_Over:
	;movlw	5
	;addwf	P_Temp,F				; 高亮从5度开始补偿

TemperOverflow_Juge:
	movf	P_Temp,W
	subwf	R_Temperature,W			; 补偿值修正
	btfss	C
	movlw	15
	movwf	R_Temperature			; 补偿后值若小于最低温度值时，以补偿15度计算

	movlw	75
	subwf	R_Temperature,W			; 计算修正后的值是否有上下溢
	btfss	C
	goto	LowerLimit_Juge

	movlw	75
	movwf	R_Temperature			; 发生上溢将温度固定为+50
	return

LowerLimit_Juge:
	movlw	15
	subwf	R_Temperature,W
	btfsc	C
	return							; 未溢出

	movlw	15
	movwf	R_Temperature			; 发生下溢将温度固定为-10
	return




; 补偿时间递增/递减判断
CompenTime_Juge:
	btfsc	PORTF,0
	goto	CompenTime_Add

	movf	CompTime_Count,F
	btfss	Z
	decf	CompTime_Count,F		; 若关屏则递减到0
	return

CompenTime_Add:
	movlw	11
	subwf	CompTime_Count,W
	btfss	Z
	incf	CompTime_Count,F		; 若亮屏则递增到11
	return
