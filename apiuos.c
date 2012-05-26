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
*/

#include <p24FJ64GA004.h>
#include "apiuos.h"
#include "sysinit.h"
#include "UserAPI.h"


volatile static struct TaskParam{			// task parameters
	unsigned int unSuspDel;
	enum task_state TaskState;				// current task state
	void (*pTaskTableAddress) (void);
} TableParam[NUMBER_OF_TASK];

volatile static u_int unQueueTask;			// queue of tasks

// queue of tasks init
static void (*pvTaskLocation[MAX_QUEUE])(void);
static void TaskFree(void);

/* -------------------------------------------------------------
this function is intended for task creating in general cycle
----------------------------------------------------------------*/
void CreateTask(void (*pAddressTask) (), u_int unOSDelay, enum task_state RunState)
{
	static unsigned int unNumberTask;
	
	//  clear the queue of tasks
	if(0 == unNumberTask)					// for first call
	{										// clear a queue
		unsigned int unQueue;				// Queue
		for(unQueue = 0; unQueue < MAX_QUEUE; unQueue++)
		{
			pvTaskLocation[unQueue] = &TaskFree;
		}	
	}
	
	// for task that has a pause - set the state to SUSPEND
	if(unOSDelay > 0)
	{
		TableParam[unNumberTask].TaskState = SUSPEND;	// for incorrect data
	}
	else
	{
		if(RunState == SUSPEND)
		{
			TableParam[unNumberTask].TaskState = IDLE;
		}
		else
		{
			TableParam[unNumberTask].TaskState = RunState;
		}
	}
	TableParam[unNumberTask].unSuspDel = unOSDelay;
	// copy address task to table of tasks
	TableParam[unNumberTask].pTaskTableAddress = pAddressTask;
	unNumberTask++;
	// to avoid exceed the table size. Last cell - last function
	if(unNumberTask >= NUMBER_OF_TASK) unNumberTask--;
}

void TaskShedule(void)
{	
	if((&TaskFree) == pvTaskLocation[0])			// queue is free
	{
		unsigned int unNumber;

		unQueueTask = 0;							// zeroing of queue
		for(unNumber = 0; unNumber < NUMBER_OF_TASK; unNumber++)
		{
			if(IDLE == TableParam[unNumber].TaskState)
			{
				TableParam[unNumber].TaskState = RUN;
				pvTaskLocation[unQueueTask] = TableParam[unNumber].pTaskTableAddress;		// copy task to queue
				unQueueTask++;
			}
		}
	}	
	else
	{
		unsigned int unNumber = 0;
		
		// finding of current task
		while(pvTaskLocation[0] != TableParam[unNumber].pTaskTableAddress)
		{
			unNumber++;
			if(unNumber > NUMBER_OF_TASK) break;
		}
		
		// if task not was stopped in other task
		if(RUN == TableParam[unNumber].TaskState)
		{
			TableParam[unNumber].TaskState = READY;					// terminate the current task
			unQueueTask--;
		}
		
		if(0 == unQueueTask)
		{
			pvTaskLocation[0] = &TaskFree;							// there is the last task
		}
		else
		{
				// shift of the queue 
				for(unNumber = 0; unNumber < (MAX_QUEUE - 1); unNumber++)
				{
					pvTaskLocation[unNumber] = pvTaskLocation[unNumber+1];
				}
				pvTaskLocation[MAX_QUEUE-1] = &TaskFree;				// last cell
		}
	}
}

void DelayService(void)
{
	unsigned int unCycleTask;

	for(unCycleTask = 0; unCycleTask < NUMBER_OF_TASK; unCycleTask++)
	{
		if(TableParam[unCycleTask].unSuspDel > 0)
		{
			TableParam[unCycleTask].unSuspDel--;
			if(0 == TableParam[unCycleTask].unSuspDel) TableParam[unCycleTask].TaskState = IDLE;
		}
	}
}

void UOSDelay(u_int unTickDelay)
{
	unsigned int unCurTask;
	
	// finding current task
	for(unCurTask = 0; pvTaskLocation[0] != TableParam[unCurTask].pTaskTableAddress; unCurTask++)
	{
		if(unCurTask > NUMBER_OF_TASK)
		{
			break;
		}
	}
	TableParam[unCurTask].unSuspDel = unTickDelay;
}

void Run(void)
{
	(*pvTaskLocation[0])();	// start current task
}

/* ----------------------------------------------------
do nothing
------------------------------------------------------*/
void TaskFree(void)
{
	TaskShedule();
}
