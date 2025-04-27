SEND_CODE1:		MACRO	
	bsf	PORTF,5
	bsf	PORTF,5
	bcf	PORTF,5
	ENDM

SEND_CODE0:		MACRO
	bsf	PORTF,5
	bcf	PORTF,5
	bcf	PORTF,5
	ENDM

LED_ON:			MACRO	
	bsf	PORTA,1
	ENDM

LED_OFF:		MACRO
	bcf	PORTA,1
	ENDM

; 根据GRB_Tick的值查三通道时刻表，并将查表值送入发送缓冲区
Search_TickTable:			MACRO
	movlw	Low(GRB_Tick)
	addwf	R_X,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	INDF0,W
	movwf	P_Temp
	
	movlw	High(G_TickTable)				; 查G通道的灰度值
	movwf	PCLATH
	movf	P_Temp,W
	call	Digit_NumTable
	movwf	SendBuff_G

	movlw	High(R_TickTable)				; 查R通道的灰度值
	movwf	PCLATH
	movf	P_Temp,W
	call	Digit_NumTable
	movwf	SendBuff_R

	movlw	High(B_TickTable)				; 查B通道的灰度值
	movwf	PCLATH
	movf	P_Temp,W
	call	Digit_NumTable
	movwf	SendBuff_B
	ENDM



; 发送GRB缓冲区的数据，MSB
Send_RGB:					MACRO
	movlw	8
	movwf	P_Temp
	bcf		C
Send_G_Loop:								; 发送G通道的8bit
	rlf		SendBuff_G,F
	btfss	C
	goto	GChannel_Send0
	SEND_CODE1
	goto	GChannel_Juge
GChannel_Send0:
	SEND_CODE0
GChannel_Juge:
	decfsz	P_Temp,F
	goto	Send_G_Loop

	movlw	8
	movwf	P_Temp
	bcf		C
Send_R_Loop:								; 发送R通道的8bit
	rlf		SendBuff_R,F
	btfss	C
	goto	RChannel_Send0
	SEND_CODE1
	goto	RChannel_Juge
RChannel_Send0:
	SEND_CODE0
RChannel_Juge:
	decfsz	P_Temp,F
	goto	Send_R_Loop

	movlw	8
	movwf	P_Temp
	bcf		C
Send_B_Loop:								; 发送B通道的8bit
	rlf		SendBuff_B,F
	btfss	C
	goto	BChannel_Send0
	SEND_CODE1
	goto	BChannel_Juge
BChannel_Send0:
	SEND_CODE0
BChannel_Juge:
	decfsz	P_Temp,F
	goto	Send_B_Loop

	ENDM
