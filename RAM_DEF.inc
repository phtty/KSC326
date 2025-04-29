cblock				100H
; 用于现场保护的寄存器备份
W_BAK
STATUS_BAK
PCLATH_BAK

; 变址寻址专用变量
R_X
R_X_BAK

; T0定时器计数
T0_Counter
; T1定时器计数
T1_Counter
; 响闹秒数计数
Louding_Counter
; 退出设置模式计数
Return_Counter
; DP模式计数
DP_Counter
; 去抖延时计数
Debounce_Counter
; 快加/长按计数
LongPress_Counter
; 测温计时
TemperTest_Counter

Calendar_Flag
;bit0 = 1 闰年
;bit1 = 1 增日期

Beep_Flag
;bit0 = 1 开启蜂鸣
;bit1 = 1 按键音标志位

Clock_Flag
;bit0 = 1 12h模式
;bit1 = 1 响闹模式

Alarm_Switch
;bit0 = 1 闹钟1开启
;bit1 = 1 闹钟2开启
;bit2 = 1 闹钟3开启

Triggered_AlarmGroup
;bit0 = 1 闹钟1触发
;bit1 = 1 闹钟2触发
;bit2 = 1 闹钟3触发

Time_Flag
;bit0 = 1 1Hz标志
;bit1 = 1 2Hz标志
;bit2 = 1 4Hz标志
;bit3 = 1 16Hz标志
;bit4 = 1 32Hz标志

TimeFlag_SW
;bit0 = 1 4Hz计数开关
;bit1 = 1 16Hz计数开关
;bit2 = 1 32Hz计数开关

SysEvent_Flag
;bit0 = 1 更新显示标志
;bit1 = 1 DP显示标志
;bit2 = 1 省电模式标志
;bit3 = 1 RGB变色标志
;bit4 = 1 

Primary_Status
;0 = 时间显示
;1 = 日期显示
;2 = 温度显示
;3 = 闹钟1显示
;4 = 闹钟2显示
;5 = 闹钟3显示
Secondary_Status
;0 = 显示模式
;1 = 设置项1
;2 = 设置项2
;3 = 设置项3

DP_Mode
;0 = dp-1
;1 = dp-2
;2 = dp-3

Key_Flag
;bit0 = 1 扫键标志
;bit1 = 1 松键标志
;bit2 = 1 首次触发标志
;bit3 = 1 快加/长按标志
;bit4 = 1 功能跳过标志
;bit5 = 1 
;bit6 = 1 
;bit7 = 1 

Depress_Flag
;bit0 = 1 Down键待处理		|松键处理
;bit1 = 1 Up键待处理		|松键处理
;bit2 = 1 Set键待处理		|松键处理

ADC_Flag
;bit0 = 1 负温度标志
;bit1 = 1 华氏度模式标志

; 作循环右移的循环计数
Light_Level
;0 = 高亮
;2 = 半亮(此产品无半亮)
;3 = 低亮


PORTA_BackUp
PORTF_BackUp

Beep_Serial

R_ADC_L
R_ADC_H
R_Temperature			; 温度（摄氏）
R_Temperature_F			; 温度（华氏）

R_Time_Sec				; 时间秒
R_Time_Min				; 时间分
R_Time_Hour				; 时间小时

R_Alarm1_Min			; 闹钟1分钟
R_Alarm2_Min			; 闹钟2分钟
R_Alarm3_Min			; 闹钟3分钟
R_Alarm1_Hour			; 闹钟1小时
R_Alarm2_Hour			; 闹钟2小时
R_Alarm3_Hour			; 闹钟3小时

R_Date_Day				; 日期，从1开始
R_Date_Month			; 月份，从1开始
R_Date_Year				; 年份，由于只计2000-2099，所以由0-99代替
R_Date_Week				; 星期



; GRB颜色通道的发送缓冲区
SendBuff_G
SendBuff_R
SendBuff_B

; 用于标识显示位选是否开启
DisSwtich:
DisSwtich1
DisSwtich2
DisSwtich3
DisSwtich4

; 存放当前的GRB时刻
GRB_Tick:
GRB_Tick1
GRB_Tick2
GRB_Tick3
GRB_Tick4
GRB_Tick5
GRB_Tick6
GRB_Tick7
GRB_Tick8
GRB_Tick9
GRB_Tick10
GRB_Tick11
GRB_Tick12
GRB_Tick13
GRB_Tick14
GRB_Tick15
GRB_Tick16
GRB_Tick17
GRB_Tick18
GRB_Tick19
GRB_Tick20
GRB_Tick21
GRB_Tick22
GRB_Tick23
GRB_Tick24
GRB_Tick25
GRB_Tick26
GRB_Tick27
GRB_Tick28
GRB_Tick29
GRB_Tick30
GRB_Tick31
GRB_Tick32


; 临时变量
P_Temp:
P_Temp1
P_Temp2
P_Temp3
P_Temp4
P_Temp5
P_Temp6
P_Temp7

I_Temp:
I_Temp1
I_Temp2
I_Temp3

endc

cblock	300H
endc
