T0_IRQ_Handler:
	bcf		T0IF
	decf	T0_Counter

L_32Hz_Juge:
	btfss	TimeFlag_SW,2			; 32Hz标志开关
	goto	L_16Hz_Juge
	bsf		Time_Flag,4				; 32Hz标志位

L_16Hz_Juge:
	btfss	TimeFlag_SW,1
	goto	L_4Hz_Juge
	movf	T0_Counter,W
	andlw	0x01					; 对2求余
	btfsc	Z
	bsf		Time_Flag,3				; 16Hz标志位

L_4Hz_Juge:
	btfss	TimeFlag_SW,0
	goto	L_2Hz_Juge
	movf	T0_Counter,W
	andlw	0x07					; 对8求余
	btfsc	Z
	bsf		Time_Flag,2				; 4Hz标志位

L_2Hz_Juge:
	movf	T0_Counter,W
	andlw	0x0f					; 对16求余
	btfsc	Z
	bsf		Time_Flag,1				; 2Hz标志位

	movf	T0_Counter,F			; 判0
	btfsc	Z
	goto	T0_Counter_Reload

	goto	IRQ_EXIT

T0_Counter_Reload:
	movlw	32
	movwf	T0_Counter				; 重装载T0计数

	bsf		Time_Flag,0

	goto	IRQ_EXIT




T2_IRQ_Handler:
	bcf		T0IF
	clrf	T2						; 重装载T2

	goto	IRQ_EXIT




T1_IRQ_Handler:
	bcf		T1IF
	movlw	0xff-0x40
	movwf	T1H
	movlw	0xff
	movwf	T1L						; 重装载T1计数器的计数值

	movlw	0x02
	xorwf	PORTF,F					; 翻转PF1用于测试

	goto	IRQ_EXIT




PA_IRQ_Handler:
	movf	PORTA,F
	bcf		RAIF

	
	goto	IRQ_EXIT




PB_IRQ_Handler:
	movf	PORTB,F
	bcf		RBIF
	
	goto	IRQ_EXIT




PF_IRQ_Handler:
	movf	PORTF,F
	bcf		RFIF

	movlw	0xe0
	andwf	PORTF,W
	subwf	PORTF_BackUp,W			; 用当前PF5~7与历史状态比较，若增大了则说明有按键压下
	btfss	C
	goto	PortF_PressKey

	movlw	0xe0
	andwf	PORTF,W
	movwf	PORTF_BackUp			; 更新历史状态
	btfsc	Key_Flag,0				; 只有在扫键标志存在时，才会判断有无松键
	goto	PortF_ReleaseKey
	goto	IRQ_EXIT
PortF_PressKey:
	bsf		Key_Flag,0				; 扫键标志位
	bsf		Key_Flag,2				; 首次触发标志位
	movlw	2
	movwf	Debounce_Counter		; 复位消抖计数值
	movlw	0xe0
	andwf	PORTF,W
	movwf	PORTF_BackUp			; 更新历史状态
	clrf	IOCF					; 消抖完成前关闭PF5~7电平变化中断
	goto	IRQ_EXIT

PortF_ReleaseKey:
	bsf		Key_Flag,1				; 置位松键标志位
	goto	IRQ_EXIT
