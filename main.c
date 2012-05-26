#include <p24fj64ga004.h>
#include <stdio.h>
#include <adc.h>
#include <uart.h>
#include <PPS.h>
#include "apiuos.h"
#include "sysinit.h"


int main(void)
{
	CLKDIV &= ~(7<<8);			// set 8MHz system clock
	IOInit();
	UARTInit(9600);
	ADCInit();
	PWMInit();
	InitSysTimer(500);
	CreateTask(Task_1, 0, idle);
	CreateTask(Task_2, 4, idle);
	CreateTask(Task_3, 2, idle);
	while(1)					// general cycle
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
