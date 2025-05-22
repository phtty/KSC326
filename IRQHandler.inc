T0_IRQ_Handler:
	bcf		T0IF
	decf	T0_Counter,F

	bsf		Time_Flag,1				; 2Hz标志位

	movf	T0_Counter,F			; 递减T0中断次数
	btfsc	Z
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

	movlw	0x04
	xorwf	PORTA,F

T1_32Hz_Juge:
	bsf		Time_Flag,4				; 32Hz标志位

	movf	T1_Counter,W
	andlw	0x01					; 对2求余
	btfsc	Z
	bsf		Time_Flag,3				; 16Hz标志位

	movf	T1_Counter,W
	andlw	0x07					; 对8求余
	btfsc	Z
	bsf		Time_Flag,2				; 4Hz标志位

	movf	T1_Counter,F			; 判0
	btfsc	Z
	goto	T1_Counter_Reload
	goto	IRQ_EXIT

T1_Counter_Reload:
	movlw	8
	movwf	T1_Counter				; 重装载T1中断次数计数
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
	movf	PORTF,W
	bcf		RFIF
	andlw	0xe0
	movwf	I_Temp

	goto	Is_Battery

Battery_JugeOver:
	movlw	0xe0
	subwf	IOCF,W
	btfss	Z
	goto	IRQ_EXIT				; 若按键口中断没有开启，此中断只用于唤醒，并无实际功能

	btfsc	Key_Flag,0				; 若有扫键标志，则不判断按键
	goto	IRQ_EXIT

	movlw	0xe0
	subwf	I_Temp,W				; 判断松键
	btfsc	Z
	goto	IRQ_EXIT
	movlw	0x60
	subwf	I_Temp,W				; 判断Set键
	btfsc	Z
	goto	PortF_PressKey
	movlw	0xa0
	subwf	I_Temp,W				; 判断Up键
	btfsc	Z
	goto	PortF_PressKey
	movlw	0xc0
	subwf	I_Temp,W				; 判断Down键
	btfss	Z
	goto	PortF_IllegalPress

PortF_PressKey:
	bsf		Key_Flag,0				; 扫键标志位
	bsf		Key_Flag,2				; 首次触发标志位
	movlw	2
	movwf	Debounce_Counter		; 复位消抖计数值
	bsf		T1ON					; 开启Time1 32Hz计数
	clrf	IOCF					; 按键处理完成前关闭PF5~7电平变化中断
	clrf	LongPress_Counter
	movf	PORTF,W
	andlw	0xe0
	movwf	PORTF_BackUp			; 保存中断时的PF状态，在消抖后继续判断
	goto	IRQ_EXIT

PortF_IllegalPress:					; 非法操作
	call	Key_HandleOver
	goto	IRQ_EXIT


Is_Battery:
	btfsc	PORTF,0
	goto	DC5V
	LED_OFF
	goto	Battery_JugeOver
DC5V:
	;REFLASH_DISPLAY
	LED_ON
	goto	Battery_JugeOver
