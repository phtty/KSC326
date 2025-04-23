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

	bcf		SCS
	movlw	0xb1
	movwf	OPTION

	call	RamInitial

	call	Port_Initial

	call	ADC_Initial

	call	PWM_Initial

	call	TIM_Initial

	bsf		GIE				; 开启总中断
	bsf		PEIE			; 使能外设中断

	return


; 上电初始化RAM
RamInitial:
	bcf		RP0
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
	movlw	32
	movwf	T0_Counter		; 从32开始计数到0
	movlw	5
	movwf	R_Snooze_Time	; 贪睡时间默认为5分钟

	movlw	2
	movwf	BackLight_Level	; 上电默认高亮

	movlw	12
	movwf	R_Time_Hour		; 设置上电时间
	movlw	0
	movwf	R_Time_Min
	movlw	0
	movwf	R_Time_Sec

	movlw	25
	movwf	R_Date_Year		; 设置上电日期
	movlw	1
	movwf	R_Date_Month
	movlw	1
	movwf	R_Date_Day

	movlw	6
	movwf	R_Alarm_Hour	; 设置上电闹钟
	movlw	0
	movwf	R_Alarm_Min

	return


; GPIO初始化
Port_Initial:
	bcf		RP0
	movlw	0x82
	movwf	TRISA			; 配置PA0、7输入
	movlw	0xff
	movwf	WPDA
	movlw	0xff
	movwf	WPUA			; 无上下拉
	movlw	0x00
	movwf	IOCA			; PA无电平变化中断
	bcf		RAIE

	movlw	0x24
	movwf	TRISB			; PB0、5配置为输入悬空
	movlw	0xff
	movwf	WPDB
	movlw	0xff
	movwf	WPUB			; 无上下拉

	movlw	0xe9
	movwf	TRISF			; 配置PF0、5~7为输入
	movlw	0xff
	movwf	WPDF
	movlw	0x1f
	movwf	WPUF			; PF5~7启用上拉
	movlw	0xe0
	movwf	IOCF			; 启用PF5~7电平变化中断
	;bsf		RFIE

	bsf		RBPUB

	clrf	PORTA
	clrf	PORTB
	clrf	PORTC
	clrf	PORTF

	movlw	15
	movwf	Return_Counter	; 设置模式返回计时复位

	movlw	PORTF
	andlw	0xe0
	movwf	PORTF_BackUp	; 按键IO口初始状态保存

	return

; ADC初始化
ADC_Initial:
	bcf		RP0

	bsf		TRISB,2			; PB2配置为输入
	bsf		ANSELH,3		; PB2设为模拟IO

	movlw	0x20
	movwf	ADCON1			; 数据格式左对齐12位ADC，时钟源为Fsys8M/4T

	movlw	0x02
	movwf	ADCLK

	movlw	0xec
	movwf	ADCON0			; 启用模拟通道11，参考电压内部VDD

	bsf		ADEN
	call	F_Delay			; 等待电路稳定

	return

; PWM初始化
PWM_Initial:
	bcf		RP0
	bsf		PORCTR,4		; 配置CCP2为PA7，输出PWM
	movlw	0xfc
	andwf	TRISA

	bsf		RP0
	clrf	PWMEN			; PWM模块使能寄存器

	movlw	0xd0
	movwf	PWM0PL
	movlw	0x07
	movwf	PWM0PH			; 周期寄存器

	movlw	0xd0
	movwf	PWM0DL			; 先设置低位
	movlw	0x07
	movwf	PWM0DH			; 占空比寄存器

	movlw	0x0b
	movwf	PWM0C			; PWM模块控制寄存器

	bcf		RP0
	bsf		TRISA,7			; PA7先关闭输出

Beep_Initial:
	movlw	0x14
	movwf	T2CON			; Fcpu预分频为4作为时钟源

	movlw	0xf9
	movwf	PR2				; 250个脉冲为1个周期（2KHz）

	movlw	0x0f
	movwf	CCP2CON			; 配置CCP模块为PWM模式

	movlw	0x7d
	movwf	CCPR2L			; 占空比寄存器的高8位，低2位在CCP2CON的[6:5]位

	bsf		PORTA,0
	bsf		PORTA,1
	bsf		T2ON			; 开启TIM2
	;bcf		TRISA,7			; PA7输出启用

	return

; 定时器初始化
TIM_Initial:
	bcf		RP0
	; Tim0
	movlw	0x31
	iorwf	OPTION,F
	movlw	0xf1
	andwf	OPTION,F		; 设置8分频，PSA=0，T0CS=1,T0SE=1
	bsf		T0OSCEN			; T0OSCEN = 1

	clrf	T0				; T0从0计到256,32Hz溢出一次
	bsf		T0IE
	bcf		T0IF			; 开启TIM0中断并清中断标志位

	; Tim1
	movlw	0x8d
	movwf	T1CON			; Tim1配置为异步定时器唤醒模式，预分频系数设为不分频

	clrf	T1L
	clrf	T1H				; 清空Tim1计数
	movlw	0xff-0x40
	movwf	T1H
	movlw	0xff
	movwf	T1L				; 计数16384，半S中断1次

	bsf		T1IE
	bcf		T1IF			; 开启TIM1中断并清中断标志位

	return
