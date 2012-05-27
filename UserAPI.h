/*
-------------------------------------------------------
	Creation date:	23 may 2012
	Author:			Eugene Panasenko
	Project name:	Periodic tasks
	File name:		userapi.h
-------------------------------------------------------
*/


#ifndef __USERAPI_H
#define __USERAPI_H


#define MAX_NUMBER_OF_TASK	4
#define SYS_TICK_PERIOD		500		// milliseconds

// don't edit this block
// =====================================================
void Task_1(void);
void Task_2(void);
void Task_3(void);
void UOSDelay(unsigned int unTCK);
// =====================================================

// place for prototype functions
void SendToUART(char *pcStringBuf);

#endif							// __USERAPI_H
