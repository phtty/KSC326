;32Hz任务，主要用于按键处理
PeriodicTask_32Hz:
	btfss	Time_Flag,4
	return
	bcf		Time_Flag,4

	call	Key_Handler
	call	TemperTest_Handler

	call	PeriodicTask_16Hz
	return



; 16Hz任务，用于蜂鸣间隔和RGB变色刷新以及更新显示
PeriodicTask_16Hz:
	btfss	Time_Flag,3
	return
	bcf		Time_Flag,3

	call	BeepManage							; 蜂鸣管理
	call	RGB_TickReflash						; RGB渐变刷新
	call	Send_RGB_Data
	return
