#include <p24fj64ga004.h>
#include <uart.h>
#include <PPS.h>
#include <adc.h>
#include "sysinit.h"

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF);
_CONFIG2(IESO_OFF & SOSCSEL_LPSOSC & WUTSEL_FST & FNOSC_FRCPLL & FCKSM_CSECME & OSCIOFNC_ON);


void IOInit(void)
{
	TRISA = 0xf9ff;				// PORTA - input, RA9,RA10 - output(LEDs)
	TRISB = 0xfffe;				// PORTB - input, RB0 - output
	TRISC = 0xffff & ~(1<<9);	// RC9 - TX UART line
	CNPU1 = 0xffff;
	_CN8PUE = 1;				// pull-up for the button
	__builtin_write_OSCCONL(OSCCON & 0xbf); 		// disable a lock
	RPOR12 = 3<<8;		// UART1 TX to RP25
	RPOR0 = 18;
	__builtin_write_OSCCONL(OSCCON | 0x40); 		// lock is on
	RED_LED = GREEN_LED = OFF;
	MOTOR = STOP;
}	

void InitSysTimer(int period)	// (ms) using TMR4 & TMR5
{
	int rest;
	if(period > 1000) period = 1000;
	_T4IE = 0;				// disable timer interrupt
	IPC6 = 7<<12;			// highest priority
	T4CON = 0;
	T4CON |= Pr256;			// Prescale 1/256
	rest = SysClk/256;
	PR4 = rest*period;		// Maximum cycle of the counter before interrupt
	TMR4 = 0;				// before start the timer set it to zero
	T4CON |= 0x8000;		// start timer4
	_T4IF = 0;				// clear flag
	_T4IE = 1;				// enable interrupt timer 
}

void UARTInit(unsigned int baudrate)	// max. 19200
{
	U1MODE = UART_DIS & UART_IrDA_DISABLE & UART_MODE_SIMPLEX & UART_UEN_00 & UART_DIS_WAKE & UART_EN_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & UART_1STOPBIT;
	U1STA = UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & UART_INT_RX_BUF_FUL & UART_ADR_DETECT_DIS;
	U1BRG = (unsigned long)SysClk*125/2/baudrate-1;
	U1MODE = UART_EN & UART_IrDA_DISABLE & UART_MODE_SIMPLEX & UART_UEN_00 & UART_DIS_WAKE & UART_EN_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & UART_1STOPBIT;
}

void InitDelayTimer(void)
{
	_T5IE = 0;
	T5CON = 0;
	PR5 = 65535;
	T5CON |= Pr256;
	T5CON |= 0x8000;			// enable timer
}

// Commands
void LedBlink(unsigned int delay)
{
	GREEN_LED = ON;
	TMR5 = 0;
	while(TMR5 < delay);
	GREEN_LED = OFF;
}



void ADCInit(void)
{
	AD1PCFG = ENABLE_AN0_ANA; // all inputs - digital except AN0
	AD1CON2 = ADC_VREF_AVDD_AVSS & ADC_SCAN_OFF & ADC_INTR_EACH_CONV & ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF;
	AD1CON3 = (31<<8) | 50;
	AD1CON1 = ADC_MODULE_OFF & ADC_IDLE_STOP & ADC_FORMAT_INTG & ADC_CLK_MANUAL & ADC_AUTO_SAMPLING_OFF & ADC_SAMP_OFF;
	AD1CHS = 0;				// channel 0
	IFS0bits.AD1IF = 0;
	IEC0bits.AD1IE = 0;		// disable interrupt
	EnableADC1;
}	

unsigned int ADCStartConversion(unsigned int channel)
{
	AD1CHS = channel;
	AD1CON1bits.SAMP = 1;
	Nop();
	Nop();
	Nop();
	Nop();
	AD1CON1bits.SAMP = 0;
	while(!AD1CON1bits.DONE);		
	return ADC1BUF0;				// get adc value
}

void SetPWMValue(unsigned int PWMValue)
{
	OC1RS=(PWMValue>PR3) ? (PR3-10):PWMValue;
}	
void PWMInit(void)
{
	PR3 = 0xF000;
	OC1RS = 0;
	OC1R = 0;
	OC1CON = 0;
	OC1CON = 0x000F;				// PWM mode
	TMR3 = 0;
	T3CON = 0x8000;
	SetPWMValue(0);
}

void SendToUART(char *buf)
{
	U1TXREG = *buf++;
	while(*buf != '\0')
	{
		while(U1STAbits.UTXBF);  		// if the buffer is full
		U1TXREG = *buf++;
	}
}
