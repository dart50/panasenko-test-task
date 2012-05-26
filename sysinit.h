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


#define	RED_LED			_LATA9
#define GREEN_LED 		_LATA10
#define ON 				0
#define OFF 			1
#define BUTTON 			_RC0
#define MOTOR 			_RB0
#define STOP 			0
#define START 			1
#define HIGHSPEED 		30000
#define LOWSPEED 		10000
#define LENGHT 			5000
#define PR256 			(3<<4)				// timer prescaler
#define SYSCLK 			16000				// kHz
#define UART_BAUDRATE	9600

void IOInit(void);
void UARTInit(unsigned int unBaudrate);
void ADCInit(void);
void PWMInit(void);
void InitSysTimer(unsigned int unPeriod);

#endif									// __SYSINIT_H
