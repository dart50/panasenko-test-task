/*
-------------------------------------------------------
	Creation date:		23 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			sysinit.c
-------------------------------------------------------
		
=======================================================
	Modification date:	26 may 2012
	Author:				Eugene Panasenko
		
		Clock setting moved in IOInit(), deleted unused
	functions.
						
=======================================================
*/

#include <p24fj64ga004.h>
#include <uart.h>
#include <PPS.h>
#include <adc.h>
#include "sysinit.h"

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF);
_CONFIG2(IESO_OFF & SOSCSEL_LPSOSC & WUTSEL_FST & FNOSC_FRCPLL & FCKSM_CSECME & OSCIOFNC_ON);


void IOInit(void)
{
	// set 8MHz system clock
	CLKDIV &= ~(7 << 8);
	
	// PORTA - input, RA9,RA10 - output(LEDs)
	TRISA = 0xf9ff;
	
	// PORTB - input, RB0 - output
	TRISB = 0xfffe;
	
	// RC9 - TX UART line
	TRISC = 0xffff & ~(1<<9);
	
	// pull-up for the button
	CNPU1 = 0xffff;
	_CN8PUE = 1;
	
	// disable a lock
	__builtin_write_OSCCONL(OSCCON & 0xbf);
	
	// UART1 TX to RP25 pin
	RPOR12 = 3<<8;
	RPOR0 = 18;
	
	// lock is on
	__builtin_write_OSCCONL(OSCCON | 0x40);
	
	RED_LED = GREEN_LED = OFF;
	MOTOR = STOP;
}	

void InitSysTimer(unsigned int unPeriod)	// (ms) using TMR4 & TMR5
{
	unsigned int unRest;
	
	if(unPeriod > 1000) unPeriod = 1000;
	
	// disable timer interrupt
	_T4IE = 0;
	
	// highest priority			
	IPC6 = 7 << 12;
	T4CON = 0;
	
	// Prescale 1/256
	T4CON |= PR256;
	unRest = SYSCLK / 256;
	
	// Maximum cycle of the counter before an interrupt
	PR4 = unRest * unPeriod;
	
	// before start the timer set it to zero
	TMR4 = 0;
	
	// start timer4
	T4CON |= 0x8000;
	
	// enable interrupt timer 
	_T4IF = 0;
	_T4IE = 1;
}

void UARTInit(unsigned int unBaudrate)	// max. 19200
{
	U1MODE =	UART_DIS & UART_IrDA_DISABLE & 
				UART_MODE_SIMPLEX & UART_UEN_00 & 
				UART_DIS_WAKE & UART_EN_LOOPBACK & 
				UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & 
				UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & 
				UART_1STOPBIT;
				
	U1STA = 	UART_IrDA_POL_INV_ZERO & 
				UART_SYNC_BREAK_DISABLED & 
				UART_TX_ENABLE & UART_INT_RX_BUF_FUL &
				UART_ADR_DETECT_DIS;
				
	U1BRG = ((((unsigned long) SYSCLK) * 125) / 2) / (unBaudrate - 1);
	
	U1MODE = 	UART_EN & UART_IrDA_DISABLE & 
				UART_MODE_SIMPLEX & UART_UEN_00 & 
				UART_DIS_WAKE & UART_EN_LOOPBACK & 
				UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & 
				UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & 
				UART_1STOPBIT;
}

void SendToUART(char *pcStringBuf)
{
	U1TXREG = *pcStringBuf++;
	while(*pcStringBuf != '\0')
	{
		while(U1STAbits.UTXBF);  		// if the buffer is full
		U1TXREG = *pcStringBuf++;
	}
}
