List	P = HC18P134L, R = DEC

include	HC18P133L.inc
include	Content.inc
include	MACRO.inc
include	RAM_DEF.inc

	org	0000H
	goto	START
	org	0004H
	goto	IRQ

START:
	nop
	nop

	call	System_Init						; �ϵ��ʼ��

MAIN:
	call	PeriodicTask_32Hz				; 32Hz����
	call	PeriodicTask_16Hz				; 16Hz����
	call	PeriodicTask_2Hz				; 2Hz����
	call	PeriodicTask_1Hz				; 1Hz����
	goto	MAIN


F_Delay:									; ��Լ130us
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

	btfss	T0IE							; ���ж��ж��Ƿ���
	goto	T1_IRQ_Juge
	btfsc	T0IF							; ���ж������жϱ�־λ
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
include KeyFunction.inc
include RGBTable.inc
include	RGBManage.inc
include DisplayDriver.inc
include Display.inc
include	PeriodicTask16Hz.inc
include	PeriodicTask2Hz.inc
include	PeriodicTask1Hz.inc
include Temper.inc
include ADCTable.inc
include Beep.inc
include Time.inc
include Calendar.inc
include Alarm.inc
include NightMode.inc
include PowerManage.inc
include	DPMode.inc


end
