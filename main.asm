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

	call	RGB_ModeSwitch
	;btfsc	PORTF,0							; ����ֻ��Ŧ�۵���򲻽����ϵ���ʾ
	call	BootScreen_Display				; �ϵ���ʾ
	HALFSEC_DISPLAY							; ��S�����Լ��ϵ���ʾ��
	call	Key_Beep

MAIN:
	call	PowerSave_Juge					; ʡ��ģʽ
	call	Display_Reflash					; ˢ����ʾ
	call	PeriodicTask_32Hz				; 32Hz����
	call	PeriodicTask_2Hz				; 2Hz����

	goto	MAIN


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
	goto	T0_IRQ_Handler					; Timer0 2Hz�жϣ�������ʱ����ʾˢ��

T1_IRQ_Juge:
	btfss	T1IE
	goto	T2_IRQ_Juge
	btfsc	T1IF
	goto	T1_IRQ_Handler					; Timer1 32Hz�жϣ����ڰ��������������RGB��ɫ��

T2_IRQ_Juge:
	btfss	T2IE
	goto	PA_IRQ_Juge
	btfsc	T2IF
	goto	T2_IRQ_Handler					; Timer2�жϣ�δʹ��

PA_IRQ_Juge:
	btfss	RAIE
	goto	PB_IRQ_Juge
	btfsc	RAIF
	goto	PA_IRQ_Handler					; PA�ڵ�ƽ�仯�жϣ�δʹ��

PB_IRQ_Juge:
	btfss	RBIE
	goto	PF_IRQ_Juge
	btfsc	RBIF
	goto	PB_IRQ_Handler					; PB�ڵ�ƽ�仯�жϣ�δʹ��

PF_IRQ_Juge:
	btfss	RFIE
	goto	IRQ_EXIT
	btfsc	RFIF
	goto	PF_IRQ_Handler					; PF�ڵ�ƽ�仯�жϣ�������ⰴ����DC5V

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
include	RGBManage.inc
include DisplayDriver.inc
include Display.inc
include	PeriodicTask32Hz.inc
include	PeriodicTask2Hz.inc
include Temper.inc
include Beep.inc
include Time.inc
include Calendar.inc
include Alarm.inc
include PowerSaving.inc
include	DPMode.inc
include BootScreen.inc
include ADCTable.inc
include	Table2.inc
include	Table1.inc
include RGBTable.inc
	end
