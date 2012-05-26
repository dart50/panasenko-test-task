/*
-------------------------------------------------------
	Creation date:	1 may 2012
	Author:			Eugene Panasenko
	Project name:	Periodic tasks
	File name:		main.c
-------------------------------------------------------

*******************************************************
	This program provide periodic executing an user 
	defined tasks. 
*******************************************************
		
=======================================================
	Modification date:	3 may 2012
	Author:				Eugene Panasenko
	
		Added a dynamic registration of tasks.
		Task creation defined in \apiuos.c
		
=======================================================
	Modification date:	23 may 2012
	Author:				Eugene Panasenko
	
		Task creation defined in separate file 
		\UserAPI.c
		Macro names declared in big letters.
		
=======================================================
	Modification date:	26 may 2012
	Author:				Eugene Panasenko
		
		Deleted functions which not used.
=======================================================
*/


#include <p24FJ64GA004.h>
#include <stdio.h>
#include <adc.h>
#include <uart.h>
#include <PPS.h>
#include "apiuos.h"
#include "sysinit.h"
#include "UserAPI.h"


int main(void)
{
	IOInit();
	UARTInit(UART_BAUDRATE);				// 9600
	InitSysTimer(SYS_TICK_PERIOD);			// 500 ms
	CreateTask(Task_1, 0, IDLE);
	CreateTask(Task_2, 4, IDLE);
	CreateTask(Task_3, 2, IDLE);
	while(1)								// general cycle
	{
		Run();
	}
}

// The interrupt with period 0.5 seconds
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
	_T4IF = 0;
	DelayService();
}
