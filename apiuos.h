/*
-------------------------------------------------------
	Creation date:		3 may 2012
	Author:				Eugene Panasenko
	Project name:		Periodic tasks
	File name:			apiuos.h
-------------------------------------------------------
		
=======================================================
	Modification date:	23 may 2012
	Author:				Eugene Panasenko
	Changes:			Macro names declared in big letters. 
						Changed style of names.
=======================================================
*/

#ifndef __APIUOS_H
#define __APIUOS_H


typedef unsigned int u_int;
typedef unsigned long u_long;

enum task_state{				// State of the task
	READY = 1,
	RUN,
	SUSPEND,
	BLOCKED,
	IDLE
};

void CreateTask(void (*pAddressTask) (), u_int unOSDelay, enum task_state RunState);
void Run(void);
void DelayService(void);
void ChangeStateTask(void (*pAddressTask) (), enum task_state RunState);

#endif							// __APIUOS_H
