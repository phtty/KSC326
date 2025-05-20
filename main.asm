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

	call	System_Init						; 上电初始化

	call	RGB_ModeSwitch
	;btfsc	PORTF,0							; 若是只有纽扣电池则不进行上电显示
	call	BootScreen_Display				; 上电显示
	HALFSEC_DISPLAY							; 半S更新以及上电提示音
	call	Key_Beep

MAIN:
	call	PowerSave_Juge					; 省电模式
	call	Display_Reflash					; 刷新显示
	call	PeriodicTask_32Hz				; 32Hz任务
	call	PeriodicTask_2Hz				; 2Hz任务

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

	btfss	T0IE							; 先判断中断是否开启
	goto	T1_IRQ_Juge
	btfsc	T0IF							; 再判断有无中断标志位
	goto	T0_IRQ_Handler					; Timer0 2Hz中断，用于走时和显示刷新

T1_IRQ_Juge:
	btfss	T1IE
	goto	T2_IRQ_Juge
	btfsc	T1IF
	goto	T1_IRQ_Handler					; Timer1 32Hz中断，用于按键、蜂鸣间隔、RGB变色等

T2_IRQ_Juge:
	btfss	T2IE
	goto	PA_IRQ_Juge
	btfsc	T2IF
	goto	T2_IRQ_Handler					; Timer2中断，未使用

PA_IRQ_Juge:
	btfss	RAIE
	goto	PB_IRQ_Juge
	btfsc	RAIF
	goto	PA_IRQ_Handler					; PA口电平变化中断，未使用

PB_IRQ_Juge:
	btfss	RBIE
	goto	PF_IRQ_Juge
	btfsc	RBIF
	goto	PB_IRQ_Handler					; PB口电平变化中断，未使用

PF_IRQ_Juge:
	btfss	RFIE
	goto	IRQ_EXIT
	btfsc	RFIF
	goto	PF_IRQ_Handler					; PF口电平变化中断，用于侦测按键和DC5V

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
