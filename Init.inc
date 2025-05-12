System_Init:
	bcf		RP0
	clrf	INTCON			; 清所有外设的中断使能
	clrf	PCON
	clrf	PORCTR			; 重置引脚配置寄存器
	clrf	ANSELL			; 设置IO口为数字IO模式
	clrf	ANSELH
	clrf	OPTION			; 重置功能选择寄存器
	clrf	TRISA			; 重置PORTA为输出
	clrf	TRISC			; 重置PORTC为输出
	clrf	TRISF			; 重置PORTF为输出

	bcf		SCS				; 选择高频系统时钟
	;movlw	0xb1
	;movwf	OPTION

	call	Ram_Initial
	call	Port_Initial
	call	PWM_Initial
	call	TIM_Initial

	LED_ON					; 打开RGB灯的供电
	movf	PORTF,F
	bcf		RFIF

	movlw	1				; 测温间隔设置为1，上电进行一次测温
	movwf	TemperTest_Counter

	bsf		PEIE			; 使能外设中断
	bsf		GIE				; 开启总中断

	return


; 上电初始化RAM
Ram_Initial:
	movlw	00H				; 清零0X100~0X1FF
	movwf	FSR0L
	movlw	0X01
	movwf	FSR0H			; FSR 指向100h 地址 
NEXTBYTE:
	clrf	INDF0			; 对FSR 指向的数据存储器清零
	incfsz	FSR0L,F			; FSR0L + 1,指向下一个地址。注意这里的边界值为欲操作RAM 最大地址+1
							; 利用间接寻址，注意意外指向特殊寄存器的情况
	goto	NEXTBYTE		; FSR0L 的值加一不溢出,循环清零下一个地址

	movlw	00H				; 清零0X300~0X3FF
	movwf	FSR1L			; FSR 指向300h 地址
	movlw	0X03
	movwf	FSR1H
NEXTBYTE_1:
	clrf	INDF1			; 对FSR 指向的数据存储器清零 
	incfsz	FSR1L,F			; FSR1L,指向下一个地址 
							; 注意这里的边界值为欲操作RAM 最大地址+1 
							; 利用间接寻址，注意意外指向特殊寄存器的情况 
	goto	NEXTBYTE_1		; FSR1L 的值加不溢出,循环清零下一个地址

; 用户变量初始化
	movlw	2
	movwf	T0_Counter		; 从2开始计数到0
	movlw	8
	movwf	T1_Counter		; 从8开始计数到0

	movlw	0
	movwf	Light_Level		; 上电默认高亮

	movlw	1
	movwf	DP_Mode			; DP2模式

	movlw	15
	movwf	Return_Counter	; 复位设置模式返回计时

	movlw	12
	movwf	R_Time_Hour		; 设置上电时间
	movlw	00
	movwf	R_Time_Min
	movlw	00
	movwf	R_Time_Sec
	bsf		Clock_Flag,0	; 上电为12H模式

	movlw	25
	movwf	R_Date_Year		; 设置上电日期
	movlw	1
	movwf	R_Date_Month
	movlw	1
	movwf	R_Date_Day

	clrf	R_Alarm1_Hour	; 设置上电闹钟
	clrf	R_Alarm2_Hour
	clrf	R_Alarm3_Hour
	clrf	R_Alarm1_Min
	clrf	R_Alarm2_Min
	clrf	R_Alarm3_Min

	return


; GPIO初始化
Port_Initial:
	movlw	0x80
	movwf	TRISA			; 配置PA7输入
	movlw	0xff
	movwf	WPDA
	movlw	0xff
	movwf	WPUA			; 无上下拉
	movlw	0x00
	movwf	IOCA			; PA无电平变化中断
	LED_OFF					; 初始化阶段关闭LED
	bcf		RAIE

	movlw	0x24
	movwf	TRISB			; PB0、5配置为输入悬空
	movlw	0xff
	movwf	WPDB
	movlw	0xff
	movwf	WPUB			; 无上下拉

	movlw	0xe1
	movwf	TRISF			; 配置PF0、5~7为输入
	movlw	0xff
	movwf	WPDF
	movlw	0x1f
	movwf	WPUF			; PF5~7启用上拉
	movlw	0xe0
	movwf	IOCF			; 启用PF5~7电平变化中断
	movf	PORTF,W
	bcf		RFIF
	bsf		RFIE			; 开启PF中断

	bsf		RBPUB

	clrf	PORTA
	clrf	PORTB
	clrf	PORTC
	clrf	PORTF

	return

; ADC初始化
ADC_Initial:
	bsf		TRISB,2			; PB2配置为输入
	bsf		ANSELH,3		; PB2设为模拟IO

	movlw	0x30
	movwf	ADCON1			; 数据格式左对齐12位ADC，时钟源为Fsys8M/2T

	movlw	0x01
	movwf	ADCLK

	movlw	0xec
	movwf	ADCON0			; 启用模拟通道11，参考电压内部VDD

	bsf		ADEN
	call	Delay_130us		; 等待电路稳定

	return


; ADC去初始化
ADC_Deinitial:
	bcf		ANSELH,3		; PB2设为IO口输出
	bcf		TRISB,2			; PB2配置为输出0
	bcf		PORTB,2

	clrf	ADCON1			; 清空ADC相关寄存器
	clrf	ADCLK
	clrf	ADCON0

	return


; PWM初始化
PWM_Initial:
	bsf		PORCTR,4		; 配置CCP2为PA7，输出PWM

	movlw	0x1c
	movwf	T2CON			; Fcpu预分频为8作为时钟源

	movlw	0xf9
	movwf	PR2				; 250个脉冲计数为1个周期（2KHz）

	movlw	0x0f
	movwf	CCP2CON			; 配置CCP模块为PWM模式

	movlw	0x7d
	movwf	CCPR2L			; 占空比寄存器的高8位，低2位在CCP2CON的[4:5]位

	bcf		T2ON			; 关闭TIM2

	return

; 定时器初始化
TIM_Initial:
	; Tim0 2Hz走时定时器
	movlw	0x25
	iorwf	OPTION,F
	movlw	0xe5
	andwf	OPTION,F		; 设置64分频，PSA=0，T0CS=1,T0SE=0
	bsf		T0OSCEN			; T0OSCEN = 1

	clrf	T0				; T0从0计到256,2Hz溢出一次

	bcf		T0IF			; 开启TIM0中断并清中断标志位
	bsf		T0IE

	; Tim1 32Hz功能定时器
	movlw	0x8d
	movwf	T1CON			; Tim1配置为异步定时器唤醒模式，预分频系数设为不分频

	clrf	T1L
	clrf	T1H				; 清空Tim1计数
	movlw	0xff-0x04
	movwf	T1H
	movlw	0xff
	movwf	T1L				; 计数1024，32Hz中断1次

	bcf		T1IF			; 开启TIM1中断并清中断标志位
	bsf		T1IE

	return


Delay_130us:				; 大约130us
	movlw	0xff
	movwf	P_Temp

	decfsz	P_Temp,F
	goto	$-1
	return
