; ���Ӵ���
Alarm_Handler:
	movf	Secondary_Status,F
	btfss	Z									; ��������ģʽ���ж����Ӵ���
	return

	movf	Alarm_Switch,F
	btfss	Z									; �����ӿ������ж����Ӵ���
	goto	Is_AlarmTrigger
	return
Is_AlarmTrigger:
	movlw	B'001'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm1_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm1_Hour,W
	btfsc	Z
	goto	Alarm1_HourMatch					; ����1Сʱƥ��
Alarm1_NoMatch:
	movlw	B'010'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm2_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm2_Hour,W
	btfsc	Z
	goto	Alarm2_HourMatch					; ����2Сʱƥ��
Alarm2_NoMatch:
	movlw	B'100'
	andwf	Alarm_Switch,W
	btfsc	Z
	goto	Alarm3_NoMatch
	movf	R_Time_Hour,W
	subwf	R_Alarm3_Hour,W
	btfsc	Z
	goto	Alarm3_HourMatch					; ����3Сʱƥ��
Alarm3_NoMatch:
	return


Alarm1_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm1_Min,W
	btfsc	Z
	goto	Alarm1_MinMatch						; ����1����ƥ��
	goto	Alarm1_NoMatch

Alarm2_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm2_Min,W
	btfsc	Z
	goto	Alarm2_MinMatch						; ����2����ƥ��
	goto	Alarm2_NoMatch

Alarm3_HourMatch:
	movf	R_Time_Min,W
	subwf	R_Alarm3_Min,W
	btfsc	Z
	goto	Alarm3_MinMatch						; ����3����ƥ��
	goto	Alarm3_NoMatch


Alarm1_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; �벻ƥ����ֱ���˳�
	bsf		Triggered_AlarmGroup,0				; ��λ����1����
	goto	Alarm_MatchHandle

Alarm2_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; �벻ƥ����ֱ���˳�
	bsf		Triggered_AlarmGroup,1				; ��λ����1����
	goto	Alarm_MatchHandle

Alarm3_MinMatch:
	movf	R_Time_Sec,W
	btfss	Z
	return										; �벻ƥ����ֱ���˳�
	bsf		Triggered_AlarmGroup,2				; ��λ����1����

Alarm_MatchHandle:
	bsf		Clock_Flag,1						; ��λ����״̬
	bsf		Beep_Flag,0
	bcf		Time_Flag,3
	clrf	Louding_Counter
	return




; AL����ʾ
AL_Display:
	movf	Alarm_Switch,F					; ���κ�һ�����ӿ���ʱ����ʾ
	btfsc	Z
	goto	SymbolAL_Clear

	goto	SymbolAL_Display				; �����ӿ���ʱ��ʾ




; ���ӿ��ذ�S����ʾ
AlarmSW_HS_Display:
	call	AlarmSWSymbol_Display
	goto	AlarmSW_Dipalay


; ����ʱ�����ð�S����ʾ
Alarm_HS_Display:
	call	AlarmSymbol_Display
	goto	Alarm_Dipalay


; ���ӿ���1S����ʾ
AlarmSW_Sec_Display:
	call	AlarmSWSymbol_Display
	btfsc	Clock_Flag,1
	call	SymbolAL_Clear						; ����ʱ1S��AL��
	goto	AlarmSW_Dipalay


; ����Сʱ����1S����ʾ
AlarmHour_Sec_Display:
	call	AlarmSymbol_Display
	goto	Undisplay_01


; ���ӷ�������1S����ʾ
AlarmMin_Sec_Display:
	call	AlarmSymbol_Display
	goto	Undisplay_23
