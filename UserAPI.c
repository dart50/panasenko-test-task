/*
-------------------------------------------------------
	Creation date:		23 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			UserAPI.c
-------------------------------------------------------
*/

#include <p24FJ64GA004.h>
#include "UserAPI.h"
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
	
	// if need change state of task
	ChangeStateTask(Task_X, BLOCKED);
	
	//-------------------------
	TaskShedule();
}

********************************************************/

void Task_1(void)
{
	// place here your code
	//-------------------------
	RED_LED = ~RED_LED;
	UOSDelay(2);
	//-------------------------
	TaskShedule();
}

void Task_2(void)
{
	// place here your code
	//--------------------------
	GREEN_LED = ~GREEN_LED;
	UOSDelay(10);
	//--------------------------
	TaskShedule();
}

void Task_3(void)
{
	// place here your code
	//--------------------------
	SendToUART("TASK 3>> ");
	UOSDelay(6);
	//--------------------------
	TaskShedule();
}
