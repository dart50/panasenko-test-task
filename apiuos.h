
/*-----------------------------------------------------
	Creation date:		3 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			apiuos.h
-------------------------------------------------------
		
=======================================================
	Modification date:	23 may 2012
	Author:				Eugene Panasenko
	
		Macro names declared in big letters. Changed 
	style of names.
						
=======================================================
	Modification date:	27 may 2012
	Author:				Eugene Panasenko

		Added comments for enumeration members. Removed
	unused prototypes.

=====================================================*/

#ifndef __APIUOS_H
#define __APIUOS_H


typedef unsigned int u_int;
typedef unsigned long u_long;

enum TASK_STATE{				// State of the task
	READY = 1,					// task was executed
	RUN,						// currently task is executing
	SUSPEND,					// task was suspended for time specified in variable *.unSuspDel
	IDLE						// task waits for their turn
};

void CreateTask(void (*pvAddressTask) (), u_int unOSDelay, enum TASK_STATE RunState);
void Run(void);
void DelayService(void);

#endif							// __APIUOS_H
