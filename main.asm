List	P = HC18P134L, R = DEC

include	HC18P133L.inc
include	Content.inc
include	Macro.inc
include	RAM_DEF.inc

	org	0000H
	goto	START
	org	0004H
	goto	IRQ

START:
	nop
	nop

	call	System_Init						; 上电初始化
	
	bcf		TRISF,5
	bcf		PORTF,5

	movlw	8
	movwf	P_Temp
SendLoop1:
	LED_CODE1
	decfsz	P_Temp,F
	goto	SendLoop1
	movlw	8
	movwf	P_Temp
SendLoop2:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop2
	movlw	8
	movwf	P_Temp
SendLoop3:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop3


	movlw	8
	movwf	P_Temp
SendLoop4:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop4
	movlw	8
	movwf	P_Temp
SendLoop5:
	LED_CODE1
	decfsz	P_Temp,F
	goto	SendLoop5
	movlw	8
	movwf	P_Temp
SendLoop6:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop6


	movlw	8
	movwf	P_Temp
SendLoop7:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop7
	movlw	8
	movwf	P_Temp
SendLoop8:
	LED_CODE0
	decfsz	P_Temp,F
	goto	SendLoop8
	movlw	8
	movwf	P_Temp
SendLoop9:
	LED_CODE1
	decfsz	P_Temp,F
	goto	SendLoop9


MAIN:
	call	PeriodicTask_32Hz				; 32Hz任务
	call	PeriodicTask_16Hz				; 16Hz任务
	call	PeriodicTask_2Hz				; 2Hz任务
	call	PeriodicTask_1Hz				; 1Hz任务
	goto	MAIN


F_Delay:									; 大约130us
	bcf		RP0

	movlw	0xff
	movwf	P_Temp

	decfsz	P_Temp,F
	goto	$-1
	return


IRQ:
	bcf		RP0
	movwf	W_BAK
	swapf	STATUS,W
	movwf	STATUS_BAK
	movf	PCLATH,W
	movwf	PCLATH_BAK
	movf	R_X,W
	movwf	R_X_BAK

	btfss	T0IE							; 先判断中断是否开启
	goto	T1_IRQ_Juge
	btfsc	T0IF							; 再判断有无中断标志位
	goto	T0_IRQ_Handler
T1_IRQ_Juge:

	btfss	T1IE
	goto	T2_IRQ_Juge
	btfsc	T1IF
	goto	T1_IRQ_Handler
T2_IRQ_Juge:

	btfss	T2IE
	goto	PA_IRQ_Juge
	btfsc	T2IF
	goto	T2_IRQ_Handler
PA_IRQ_Juge:

	btfss	RAIE
	goto	PB_IRQ_Juge
	btfsc	RAIF
	goto	PA_IRQ_Handler
PB_IRQ_Juge:

	btfss	RBIE
	goto	PF_IRQ_Juge
	btfsc	RBIF
	goto	PB_IRQ_Handler
PF_IRQ_Juge:

	btfss	RFIE
	goto	IRQ_EXIT
	btfsc	RFIF
	goto	PF_IRQ_Handler

IRQ_EXIT:
	movf	R_X_BAK,W
	movwf	R_X
	movf	PCLATH_BAK,W
	movwf	PCLATH
	swapf	STATUS_BAK,W
	movwf	STATUS
	swapf	W_BAK,F
	swapf	W_BAK,W

	retfie

include	Init.inc
include	IRQHandler.inc
include KeyHandler.inc
;include LedTable.inc
;include Dis.inc
;include Display.inc
include Temper.inc
include ADCTable.inc
include Beep.inc
include Time.inc
include Calendar.inc
include Alarm.inc
include NightMode.inc
include PowerManage.inc

end
