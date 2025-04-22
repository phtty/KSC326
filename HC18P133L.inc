;==========================================================================
;
;       Register Definitions
;
;==========================================================================

W			EQU		H'0000'
F			EQU		H'0001'

;CORE REGISTER
INDF0		EQU		0x00
INDF1		EQU		0x01
PCL			EQU		0x02
STATUS		EQU		0x03
FSR0L		EQU		0x04
FSR0H		EQU		0x05
FSR1L		EQU		0x06
FSR1H		EQU		0x07
PCLATH		EQU		0x08
INTCON		EQU		0x09

;BANK0
;IO
TRISA		EQU		0x10
TRISB		EQU		0x11
TRISC		EQU		0x12
TRISD		EQU		0x13	;235专用
TRISE		EQU		0x14	;235专用
TRISF		EQU		0x15

PORTA		EQU		0x1C
PORTB		EQU		0x1D
PORTC		EQU		0x1E
PORTF		EQU		0x21

WPUA		EQU		0x28
WPUB		EQU		0x29
WPUC		EQU		0x2A
WPUF		EQU		0x2D

WPDA		EQU		0x34
WPDB		EQU		0x35
WPDC		EQU		0x36
WPDF		EQU		0x39

IOCA		EQU		0x40
IOCB		EQU		0x41
IOCC		EQU		0x42
IOCF		EQU		0x45

PORCTR		EQU		0x4C
DRENAL		EQU		0x4D
DRENBL		EQU		0x4E
DRENCL		EQU		0x4F
DRENFL		EQU		0x52

PIR1		EQU		0x54
PIR2		EQU		0x55
PIR3		EQU		0x56

T1L			EQU		0x58
T1H			EQU		0x59
T1CON		EQU		0x5A

T0			EQU		0x5B
T2			EQU		0x5C
PR2			EQU		0x5D
T2CON		EQU		0x5E
PR1L		EQU		0x5F
PR1CON		EQU		0x60

PIE1		EQU		0x70
PIE2		EQU		0x71
PIE3		EQU		0x72

OPTION		EQU		0x78
PCON		EQU		0x79
OSCCON		EQU		0x7A
CCPR2L		EQU		0x80
CCPR2H		EQU		0x81
CCP2CON		EQU		0x82

CCPR1L		EQU		0x83
CCPR1H		EQU		0x84
CCP1CON		EQU		0x85

ANSELL		EQU		0x8C
ANSELH		EQU		0x8D
ADRESL		EQU		0x92
ADRESH		EQU		0x93
ADCON0		EQU		0x94
ADCON1		EQU		0x95
ADCLK		EQU		0x96

PMDATL		EQU		0x9A
PMDATH		EQU		0x9B
PMADRL		EQU		0x9C
PMADRH		EQU		0x9D
PMCON		EQU		0x9E

DRENAH		EQU		0xA0
DRENBH		EQU		0xA1
DRENCH		EQU		0xA2
DRENFH		EQU		0xA5




;BANK1  			          	
BRT			EQU		0x23A
AUXR		EQU		0x23B
SCON		EQU		0x23C
SBUF		EQU		0x23D
SADEN		EQU		0x23E
SADDR		EQU		0x23F

PWM0DT		EQU		0x258
PWM0DL		EQU		0x259
PWM0DH		EQU		0x25A
PWM0PL		EQU		0x25B
PWM0PH		EQU		0x25C
PWM0C		EQU		0x25D
PWMEN		EQU		0x25E
FLTM		EQU		0x25F


LCDCON		EQU		0x2B0
COMAEN		EQU		0x2B1
COMBEN		EQU		0x2B2
COMCEN		EQU		0x2B3
COMFEN		EQU		0x2B6


;**********************************************
;*****************BIT Definitions *************
;**********************************************
;STATUS
#define			RP0			STATUS,5
#define			TO			STATUS,4
#define			PD			STATUS,3
#define			Z			STATUS,2
#define			DC			STATUS,1
#define			C			STATUS,0
;INTCON
#define			GIE			INTCON,7
#define			PEIE		INTCON,6
#define			T0IE		INTCON,5
#define			INTE		INTCON,4
#define			RBIE		INTCON,3
#define			T0IF		INTCON,2
#define			INTF		INTCON,1
#define			RBIF		INTCON,0
;PIR1
#define			ADIF		PIR1,6
#define			CCP1IF		PIR1,2
#define			T2IF		PIR1,1
#define			T1IF		PIR1,0
;PIR2
#define			PWM2IF		PIR2,7
#define			PWM1IF		PIR2,6
#define			PWM0IF		PIR2,5
#define			RXIF		PIR2,3
#define			TXIF		PIR2,2
#define			SPIF		PIR2,1
#define			CCP2IF		PIR2,0
;PIR3
#define			RFIF		PIR3,4
#define			RCIF		PIR3,1
#define			RAIF		PIR3,0
;T1CON
#define			T1CS1		T1CON,7
#define			T1CS0		T1CON,6
#define			T1CKPS1		T1CON,5
#define			T1CKPS0		T1CON,4
#define			T1OSCEN		T1CON,3
#define			T1SYNC		T1CON,2
#define			T1ON		T1CON,0
;T2CON
#define			T2CKPS3		T2CON,6
#define			T2CKPS2		T2CON,5
#define			T2CKPS1		T2CON,4
#define			T2CKPS0		T2CON,3
#define			T2ON		T2CON,2
;OPTION
#define			RBPUB		OPTION,7
#define			INTEDG		OPTION,6
#define			T0CS		OPTION,5
#define			T0SE		OPTION,4
#define			PSA			OPTION,3
#define			PS2			OPTION,2
#define			PS1			OPTION,1
#define			PS0			OPTION,0
;PIE1
#define			ADIE		PIE1,6
#define			CCP1IE		PIE1,2
#define			T2IE		PIE1,1
#define			T1IE		PIE1,0

;PIE2
#define			PWM2IE		PIE2,7
#define			PWM1IE		PIE2,6
#define			PWM0IE		PIE2,5
#define			USBIE		PIE2,4
#define			UARTIE		PIE2,2
#define			SPIE		PIE2,1
#define			CCP2IE		PIE2,0

;PIE3
#define			RFIE		PIE3,4
#define			REIE		PIE3,3
#define			RDIE		PIE3,2
#define			RCIE		PIE3,1
#define			RAIE		PIE3,0

;IOCB
#define			IOCB7		IOCB,7
#define			IOCB6		IOCB,6
#define			IOCB5		IOCB,5
#define			IOCB4		IOCB,4
#define			IOCB3		IOCB,3
#define			IOCB2		IOCB,2
#define			IOCB1		IOCB,1
#define			IOCB0		IOCB,0
;CCP1CON
#define			DC1B1		CCP1CON,5
#define			DC1B0		CCP1CON,4
#define			CCP1M3		CCP1CON,3
#define			CCP1M2		CCP1CON,2
#define			CCP1M1		CCP1CON,1
#define			CCP1M0		CCP1CON,0
;CCP2CON
#define			DC2B1		CCP2CON,5
#define			DC2B0		CCP2CON,4
#define			CCP2M3		CCP2CON,3
#define			CCP2M2		CCP2CON,2
#define			CCP2M1		CCP2CON,1
#define			CCP2M0		CCP2CON,0
;PCON
#define			LVD2EN		PCON,7
#define			LVD1EN		PCON,6
#define			WDTENS		PCON,4
#define			LVD2F		PCON,3
#define			LVD1F		PCON,2
#define			POR			PCON,1
#define			BOR			PCON,0
;OSCCON
#define			T0OSCEN		OSCCON,7
#define			HXEN		OSCCON,1
#define			SCS 		OSCCON,0

;WPUA
#define			WPUA7		WPUA,7
#define			WPUA6		WPUA,6
#define			WPUA5		WPUA,5
#define			WPUA4		WPUA,4
#define			WPUA3		WPUA,3
#define			WPUA2		WPUA,2
#define			WPUA1		WPUA,1
#define			WPUA0		WPUA,0
;WPUB
#define			WPUB7		WPUB,7
#define			WPUB6		WPUB,6
#define			WPUB5		WPUB,5
#define			WPUB4		WPUB,4
#define			WPUB3		WPUB,3
#define			WPUB2		WPUB,2
#define			WPUB1		WPUB,1
#define			WPUB0		WPUB,0
;WPUC
#define			WPUC7		WPUC,7
#define			WPUC6		WPUC,6
#define			WPUC5		WPUC,5
#define			WPUC4		WPUC,4
#define			WPUC3		WPUC,3
#define			WPUC2		WPUC,2
#define			WPUC1		WPUC,1
#define			WPUC0		WPUC,0 
;WPUF
#define			WPUF7		WPUF,7
#define			WPUF6		WPUF,6
#define			WPUF5		WPUF,5
#define			WPUF4		WPUF,4
#define			WPUF3		WPUF,3
#define			WPUF2		WPUF,2
#define			WPUF1		WPUF,1
#define			WPUF0		WPUF,0

;WPDA
#define			WPDA7		WPDA,7
#define			WPDA6		WPDA,6
#define			WPDA5		WPDA,5
#define			WPDA4		WPDA,4
#define			WPDA3		WPDA,3
#define			WPDA2		WPDA,2
#define			WPDA1		WPDA,1
#define			WPDA0		WPDA,0
;WPDB
#define			WPDB7		WPDB,7
#define			WPDB6		WPDB,6
#define			WPDB5		WPDB,5
#define			WPDB4		WPDB,4
#define			WPDB3		WPDB,3
#define			WPDB2		WPDB,2
#define			WPDB1		WPDB,1
#define			WPDB0		WPDB,0
;WPDC
#define			WPDC7		WPDC,7
#define			WPDC6		WPDC,6
#define			WPDC5		WPDC,5
#define			WPDC4		WPDC,4
#define			WPDC3		WPDC,3
#define			WPDC2		WPDC,2
#define			WPDC1		WPDC,1
#define			WPDC0		WPDC,0
;WPDF
#define			WPDF7		WPDF,7
#define			WPDF6		WPDF,6
#define			WPDF5		WPDF,5
#define			WPDF4		WPDF,4
#define			WPDF3		WPDF,3
#define			WPDF2		WPDF,2
#define			WPDF1		WPDF,1
#define			WPDF0		WPDF,0

;IOCA
#define			IOCA7		IOCA,7
#define			IOCA6		IOCA,6
#define			IOCA5		IOCA,5
#define			IOCA4		IOCA,4
#define			IOCA3		IOCA,3
#define			IOCA2		IOCA,2
#define			IOCA1		IOCA,1
#define			IOCA0		IOCA,0
;IOCC
#define			IOCC7		IOCC,7
#define			IOCC6		IOCC,6
#define			IOCC5		IOCC,5
#define			IOCC4		IOCC,4
#define			IOCC3		IOCC,3
#define			IOCC2		IOCC,2
#define			IOCC1		IOCC,1
#define			IOCC0		IOCC,0
;IOCF
#define			IOCF7		IOCF,7
#define			IOCF6		IOCF,6
#define			IOCF5		IOCF,5
#define			IOCF4		IOCF,4
#define			IOCF3		IOCF,3
#define			IOCF2		IOCF,2
#define			IOCF1		IOCF,1
#define			IOCF0		IOCF,0





;----- PMCON Bits ----------------------------------------------------------

#define			RDON		PMCON,0





;**********************************************
;*****************PWM**************************
;**********************************************
;PWM0C
#define			FLTS		PWM0C,5
#define			FLTC		PWM0C,4
#define			PWM0S1		PWM0C,3
#define			PWM0S0		PWM0C,2
#define			CK01		PWM0C,1
#define			CK00		PWM0C,0
;FLTM
#define			FLT0M1		FLTM,1
#define			FLT0M0		FLTM,0
;PWMEN
#define			EFLT		PWM0C,6
#define			EPWM01		PWM0C,3
#define			EPWM0		PWM0C,0
;***************PWM END***********************


;**********************************************
;*****************ADC**************************
;**********************************************
;ANSELL
#define			ANSEL7		ANSELL,7
#define			ANSEL6		ANSELL,6
#define			ANSEL5		ANSELL,5
#define			ANSEL4		ANSELL,4
#define			ANSEL3		ANSELL,3
#define			ANSEL2		ANSELL,2
#define			ANSEL1		ANSELL,1
#define			ANSEL0		ANSELL,0
;ANSELH
#define			ANSEL14		ANSELH,6
#define			ANSEL13		ANSELH,5
#define			ANSEL12		ANSELH,4
#define			ANSEL11		ANSELH,3
#define			ANSEL10		ANSELH,2
#define			ANSEL9		ANSELH,1
#define			ANSEL8		ANSELH,0
;ADCON0
#define			VHS1		ADCON0,7
#define			VHS0		ADCON0,6
#define			CHS3		ADCON0,5
#define			CHS2		ADCON0,4
#define			CHS1		ADCON0,3
#define			CHS0		ADCON0,2
#define			ADON		ADCON0,1
#define			ADEN		ADCON0,0
;ADCON1
#define			ADFM		ADCON1,7
#define			ADCS2		ADCON1,6
#define			ADCS1		ADCON1,5
#define			ADCS0		ADCON1,4
#define			ADREF		ADCON1,0
;ADCLK
#define			ADCLK2		ADCLK,2
#define			ADCLK1		ADCLK,1
#define			ADCLK0		ADCLK,0
;***************ADC END***********************

;**********************************************
;*****************LCD**************************
;**********************************************
;LCDCON
#define			LCDEN		LCDCON,7
#define			RLCD1		LCDCON,6
#define			RLCD0		LCDCON,5
#define			FRAM		LCDCON,4
;***************LCD END***********************


;**********************************************
;*****************UART**************************
;**********************************************
;AUXR
#define			UARTEN		AUXR,6
#define			UARTM0		AUXR,5
#define			BRTR		AUXR,4
#define			BRTX12		AUXR,3
#define			S1BRS		AUXR,2
#define			SMOD		AUXR,1
#define			SMOD0		AUXR,0
;SCON
#define			SM0			SCON,7
#define			SM1			SCON,6
#define			SM2			SCON,5
#define			REN			SCON,4
#define			TR8			SCON,3
#define			RB8			SCON,2
#define			RXWK		SCON,1
;***************UART END***********************

;==========================================================================
errorlevel -306
; File End
;
;==========================================================================
