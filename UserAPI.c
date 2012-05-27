/*
-------------------------------------------------------
	Creation date:		23 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			UserAPI.c
-------------------------------------------------------
*/

#include <p24FJ64GA004.h>
#include "userapi.h"
#include "sysinit.h"

/**********************EXAMPLE*************************
void Task_X(void)			// X - unique name
{
	// insert your code here
	//-------------------------
	
	....
	....
	....
	
	// if need delay
	UOSDelay(2);
}

********************************************************/

void Task_1(void)
{
	// place here your code
	//-------------------------
	PIN_RED_LED = ~PIN_RED_LED;
	UOSDelay(2);
	//-------------------------
}

void Task_2(void)
{
	// place here your code
	//--------------------------
	PIN_GREEN_LED = ~PIN_GREEN_LED;
	UOSDelay(10);
	//--------------------------
}

void Task_3(void)
{
	// place here your code
	//--------------------------
	SendToUART("TASK 3>> \r\n");
	UOSDelay(6);
	//--------------------------
}
