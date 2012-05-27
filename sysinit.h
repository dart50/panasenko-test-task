/*
-------------------------------------------------------
	Creation date:	3 may 2012
	Author:			Eugene Panasenko
	Project name:	Periodic tasks
	File name:		sysinit.h
-------------------------------------------------------
		
		
	23 may 2012
		Macro names declared in big letters. 
		Changed style of names.
*/

#ifndef __SYSINIT_H
#define __SYSINIT_H

#define UART_AVAILABLE

#define	PIN_RED_LED			_LATA9
#define PIN_GREEN_LED 		_LATA10
#define ON 				0
#define OFF 			1
#define PR256 			(3<<4)				// timer prescaler
#define UART_BAUDRATE	9600				// bit per second

void IOInit(void);
void UARTInit(unsigned int unBaudrate);
void ADCInit(void);
void PWMInit(void);
void InitSysTimer(unsigned int unPeriod);

#endif										// __SYSINIT_H
