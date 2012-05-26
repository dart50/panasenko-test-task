#include <p24FJ64GA004.h>
#include "apiuos.h"
#include "sysinit.h"



volatile struct TaskParam TableParam[NumberOfTasks];
void (*floc[MAX_QUEUE])(void);	// table of the task pointer
void task_free(void);
void uos_delay(u_int);
void TaskShedule(void);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Task_X(void)		// X - ID
{
	// place here your code
	//-------------------------
	
	//-------------------------
	TaskShedule();
}
*/

void Task_1(void)
{
	// place here your code
	//-------------------------
	RED_LED = ~RED_LED;
	uos_delay(2);
	//-------------------------
	TaskShedule();
}

void Task_2(void)
{
	// place here your code
	//--------------------------
	GREEN_LED = ~GREEN_LED;
	uos_delay(10);
	//--------------------------
	TaskShedule();
}

void Task_3(void)
{
	// place here your code
	//--------------------------
	SendToUART("TASK 3");
	uos_delay(6);
	//--------------------------
	TaskShedule();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	
void CreateTask(void (*addrt) (), u_int osdelay, enum Task_state RunState)
{
	static int nt;
	if(!nt)									// for first call
	{										// clear a queue
		int u = 0;
		while(u < MAX_QUEUE) floc[u++] = &task_free;
	}

//	static int nt = 0;
//	if(0 == nt){ // first call
//		for(u8 i = 0; i < MAX_QUEUE; i++)
//			floc[i] = &task_free;
//	}

	if(osdelay) TableParam[nt].TaskState = suspend;	// for incorrect data
	else if(RunState == suspend) TableParam[nt].TaskState = idle;
		 else TableParam[nt].TaskState = RunState;
	TableParam[nt].SuspDel = osdelay;
	TableParam[nt++].cf = addrt;			// copy address task to table of tasks
	if(nt >= NumberOfTasks) nt--;			// here need a foolproof code :-( !!!!
}

void TaskShedule(void)
{
	static u_int NTask;
	if(&task_free == floc[0])
	{
		int p;
		NTask = 0;
		for(p = 0; p < NumberOfTasks; p++)
		{
			if(idle == TableParam[p].TaskState)
			{
				TableParam[p].TaskState = run;
				floc[NTask++] = TableParam[p].cf;		// copy task to queue
			}
		// �������� �������� ������� �����
		}
	}	
	else
	{
		// check number of tasks
		int p = 0;
		while(floc[0] != TableParam[p].cf)			// current task
		{
			p++;
			if(p>NumberOfTasks) break;
		}
		TableParam[p].TaskState = ready;			// terminate the current task
		NTask--;
		if(!NTask) floc[0] = &task_free;			// there is the last task
		else
		{
			int k = 0;
			// shift of the queue 
			while(k < MAX_QUEUE)
			{
				floc[k] = floc[k+1];
				floc[MAX_QUEUE-1] = &task_free;		// last cell
				k++;
			}
		}		
	}
}

void DelayService(void)
{
	int p;
	for(p = 0; p < NumberOfTasks; p++)
	{
		if(TableParam[p].SuspDel)
		{
			TableParam[p].SuspDel--;
			if(!(TableParam[p].SuspDel)) TableParam[p].TaskState = idle;
		}
	}
}

void uos_delay(u_int tck)
{
	int p = 0;
	while(floc[0] != TableParam[p].cf)			// ����� ������� ������
	{
		if(p++>NumberOfTasks) break;
	}
	TableParam[p].SuspDel = tck;
}

void Run(void)
{
	(*floc[0])();	// start current task
};

void task_free(void) {TaskShedule();};
