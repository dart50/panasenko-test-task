/*
-------------------------------------------------------
	Creation date:		23 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			apious.c
-------------------------------------------------------

=======================================================
	This program provide periodic executing an user 
	defined tasks. 
		
=======================================================
	Modification date:	26 may 2012
	Author:				Eugene Panasenko
		
		In some functions "while" cycle changed to "for"
						
=======================================================
	Modification date:	27 may 2012
	Author:				Eugene Panasenko
	
		Added self-debugging code for definition
	UART_AVAILABILITY. Modified function CreateTask():
	now attempt pass wrong parameters will result in an
	error. Queue of tasks was changed to single task.
=====================================================*/

#include <p24FJ64GA004.h>
#include "apiuos.h"
#include "sysinit.h"
#include "UserAPI.h"

volatile static struct TaskParam{			// task parameters
	unsigned int unSuspDel;					// delay for state SUSPEND
	enum TASK_STATE TaskState;				// current task state
	void (*pvTaskTableAddress) (void);		// address of the task
} TableParam[MAX_NUMBER_OF_TASK];				// table of the task

static u_int unTableTaskPosition = 0;		// queue of tasks
static u_int unNumberTask = 0;

// queue of tasks init
static void (*pvTaskLocation)(void);
static void TaskFree(void);

/* -------------------------------------------------------------
	This function intended for creating list of the task
----------------------------------------------------------------*/
void CreateTask(void (*pvAddressTask) (), u_int unOSDelay, enum TASK_STATE RunState)
{	
	
	// clear the current task
	if(0 == unNumberTask) pvTaskLocation = &TaskFree;
	
	// if parameters are not correct
	if((unOSDelay > 0) && (SUSPEND != RunState))
	{
		#ifdef UART_AVAILABLE
			SendToUART("ERROR in parameters CreateTask(): unOSDelay > 0, but RunState is not SUSPEND \r\n");
		#else
			#error "UART is not available!"
		#endif										// UART_AVAILABLE
		while(1);
	}
	if((RunState == SUSPEND) && (0 == unOSDelay))
	{
		#ifdef UART_AVAILABLE
			SendToUART("ERROR in parameters CreateTask(): RunState is SUSPEND, but unOSDelay equal 0 \r\n");
		#endif										// UART_AVAILABLE
		while(1);
	}
	
	TableParam[unNumberTask].TaskState = RunState;	// Retain the state
	TableParam[unNumberTask].unSuspDel = unOSDelay;	// & delay
	
	// copy address of task to task list
	TableParam[unNumberTask].pvTaskTableAddress = pvAddressTask;
	
	unNumberTask++;									// increment counter for next task
	
	if(unNumberTask >= MAX_NUMBER_OF_TASK)				// if was exceeded the table size
	{
		#ifdef UART_AVAILABLE
			SendToUART("ERROR: Number of tasks exceeds the table size! \r\n");
		#endif										// UART_AVAILABLE
		while(1);
	}
}

/*---------------------------------------------------------------------
	This function provides a handling the queue of tasks
---------------------------------------------------------------------*/
static void TaskShedule(void)
{	
	if((&TaskFree) == pvTaskLocation)				// queue is free
	{	
		// search the ready task
		for(unTableTaskPosition = 0; unTableTaskPosition < unNumberTask; unTableTaskPosition++)
		{
			// if tasks are waiting for their turn, change them state
			if(IDLE == TableParam[unTableTaskPosition].TaskState)
			{
				TableParam[unTableTaskPosition].TaskState = RUN;
				pvTaskLocation = TableParam[unTableTaskPosition].pvTaskTableAddress;
				break;
			}
		}
	}
	else											// any task executing
	{
		// terminate the current task and switch to next
		if(SUSPEND != TableParam[unTableTaskPosition].TaskState)
		{
			TableParam[unTableTaskPosition].TaskState = READY;
		}
		unTableTaskPosition++;
		
		// and begin execute next task
		for(	; unTableTaskPosition < unNumberTask; unTableTaskPosition++)
		{
			if(IDLE == TableParam[unTableTaskPosition].TaskState)
			{
				TableParam[unTableTaskPosition].TaskState = RUN;
				pvTaskLocation = TableParam[unTableTaskPosition].pvTaskTableAddress;
				break;
			}
		}
		
		if(unTableTaskPosition >= unNumberTask)		// it is end of table
		{
			unTableTaskPosition = 0;				// go to the top
			pvTaskLocation = &TaskFree;				// because there is the last task
		}
	}
}

/*----------------------------------------------------------------
	It creates delay for task
----------------------------------------------------------------*/
void DelayService(void)
{
	unsigned int unCounter;

	for(unCounter = 0; unCounter < unNumberTask; unCounter++)
	{
		// decrement delay for tasks that in SUSPEND state
		if((TableParam[unCounter].unSuspDel > 0) && (SUSPEND == TableParam[unCounter].TaskState))
		{
			TableParam[unCounter].unSuspDel--;
			
			// change state to IDLE if delay equal zero
			if(0 == TableParam[unCounter].unSuspDel) TableParam[unCounter].TaskState = IDLE;
		}
	}
}

/*-----------------------------------------------------------------
	This function ensures creation of a delay
-----------------------------------------------------------------*/
void UOSDelay(u_int unTickDelay)
{
	TableParam[unTableTaskPosition].unSuspDel = unTickDelay;
	TableParam[unTableTaskPosition].TaskState = SUSPEND;
}

/*-----------------------------------------------------------------
	Main cycle of tasks
-----------------------------------------------------------------*/
void Run(void)
{
	TaskShedule();
	(*pvTaskLocation)();	// start current task
}

/* ----------------------------------------------------
	Do nothing
------------------------------------------------------*/
static void TaskFree(void) {};
