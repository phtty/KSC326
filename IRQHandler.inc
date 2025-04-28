T0_IRQ_Handler:
	bcf		T0IF
	decf	T0_Counter

	bsf		Time_Flag,1				; 2Hz标志位

	movf	T0_Counter,F			; 递减T0中断次数
	btfss	Z
	goto	T0_Counter_Reload
	goto	IRQ_EXIT

T0_Counter_Reload:
	movlw	2
	movwf	T0_Counter				; 重装载T0中断次数计数
	bsf		Time_Flag,0				; 1Hz标志位
	goto	IRQ_EXIT


T1_IRQ_Handler:
	bcf		T1IF
	movlw	0xff-0x04
	movwf	T1H
	movlw	0xff
	movwf	T1L						; 重装载T1计数器
	decf	T1_Counter,F			; 递减T1中断次数

T1_32Hz_Juge:
	btfss	TimeFlag_SW,2			; 32Hz标志开关
	goto	T1_16Hz_Juge
	bsf		Time_Flag,4				; 32Hz标志位

T1_16Hz_Juge:
	btfss	TimeFlag_SW,1
	goto	T1_4Hz_Juge
	movf	T1_Counter,W
	andlw	0x01					; 对2求余
	btfsc	Z
	bsf		Time_Flag,3				; 16Hz标志位

T1_4Hz_Juge:
	btfss	TimeFlag_SW,0
	goto	T1_Zero_Juge
	movf	T1_Counter,W
	andlw	0x07					; 对8求余
	btfsc	Z
	bsf		Time_Flag,2				; 4Hz标志位

T1_Zero_Juge:
	movf	T1_Counter,F			; 判0
	btfsc	Z
	goto	T1_Counter_Reload
	goto	IRQ_EXIT

T1_Counter_Reload:
	movlw	8
	movwf	T0_Counter				; 重装载T1中断次数计数
	;movlw	0x02
	;xorwf	PORTF,F					; 翻转PF1用于测试
	bsf		Time_Flag,0
	goto	IRQ_EXIT


T2_IRQ_Handler:
	bcf		T0IF

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
	subwf	IOCF,W
	btfss	Z
	goto	IRQ_EXIT				; 若按键口中断没有开启，此中断只用于唤醒，并无实际功能

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
	bsf		T1ON					; 开启Time1 32Hz计数
	bsf		TimeFlag_SW,2
	movlw	0xe0
	andwf	PORTF,W
	movwf	PORTF_BackUp			; 更新历史状态
	clrf	IOCF					; 消抖完成前关闭PF5~7电平变化中断
	goto	IRQ_EXIT

PortF_ReleaseKey:
	bsf		Key_Flag,1				; 置位松键标志位
	goto	IRQ_EXIT
