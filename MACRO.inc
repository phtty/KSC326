; 发送1码
SEND_CODE1:		MACRO	
	bsf		PORTF,2
	bsf		PORTF,2
	bsf		PORTF,2
	bcf		PORTF,2
	ENDM

; 发送0码
SEND_CODE0:		MACRO
	bsf		PORTF,2
	bcf		PORTF,2
	bcf		PORTF,2
	ENDM

; RGB灯供电开启
LED_ON:			MACRO	
	bcf		PORTA,1
	ENDM

; RGB灯供电关闭
LED_OFF:		MACRO
	bsf		PORTA,1
	ENDM

; 刷新显示
REFLASH_DISPLAY:		MACRO
	bsf		SysEvent_Flag,0
	ENDM

; 刷新2Hz显示更新
HALFSEC_DISPLAY:		MACRO
	bsf		Time_Flag,1
	ENDM

; 根据GRB_Tick的值查三通道时刻表得到该通道的灰度值，并送入发送缓冲区
Search_TickTable:		MACRO
	movlw	Low(GRB_Tick)
	addwf	P_Temp+1,W
	movwf	FSR0L
	movlw	High(GRB_Tick)
	movwf	FSR0H
	movf	INDF0,W

	movlw	Low(G_TickTable)
	addwf	INDF0,W
	movlw	High(G_TickTable)				; 查G通道的灰度值
	btfsc	C
	addlw	1
	movwf	PCLATH
	movf	INDF0,W
	call	G_TickTable
	movwf	SendBuff_G

	movlw	Low(R_TickTable)
	addwf	INDF0,W
	movlw	High(R_TickTable)				; 查R通道的灰度值
	btfsc	C
	addlw	1
	movwf	PCLATH
	movf	INDF0,W
	call	R_TickTable
	movwf	SendBuff_R

	movlw	Low(R_TickTable)
	addwf	INDF0,W
	movlw	High(B_TickTable)				; 查B通道的灰度值
	btfsc	C
	addlw	1
	movwf	PCLATH
	movf	INDF0,W
	call	B_TickTable
	movwf	SendBuff_B
	ENDM



; 发送GRB缓冲区的数据，MSB
Send_RGB:				MACRO
	movf	Light_Level,W
	movwf	R_X
	movf	SendBuff_G,W
	W_RightShift_XBit						; 将缓冲区数据右移X位来调整亮度
	movwf	SendBuff_G

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

	movf	Light_Level,W
	movwf	R_X
	movf	SendBuff_R,W
	W_RightShift_XBit						; 将缓冲区数据右移X位来调整亮度
	movwf	SendBuff_R

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

	movf	Light_Level,W
	movwf	R_X
	movf	SendBuff_B,W
	W_RightShift_XBit						; 将缓冲区数据右移X位来调整亮度
	movwf	SendBuff_B

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




; 将W右移R_X位
W_RightShift_XBit:		MACRO
	movwf	P_Temp
RightShift_Loop:
	movf	R_X,F
	btfsc	Z
	goto	RightShift_LoopOver
	decf	R_X,F
	bcf		C
	rrf		P_Temp,F
	goto	RightShift_Loop
RightShift_LoopOver:
	movf	P_Temp,W
	ENDM
