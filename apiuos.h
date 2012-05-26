#ifndef __apiuos_h

#define __apiuos_h


#define NumberOfTasks 4
#define MAX_QUEUE 10


typedef unsigned int u_int;
typedef unsigned long u_long;

enum Task_state{				// State of the task
	ready = 1,					// 
	run,
	suspend,
	blocked,
	idle
};
struct TaskParam{
	int SuspDel;
	enum Task_state TaskState;
	void (*cf) (void);
};	// task parameter


extern void SendToUART(char *);
extern void Task_1(void);
extern void Task_2(void);
extern void Task_3(void);
extern void uos_init(void);
extern void CreateTask(void (*) (), u_int, enum Task_state);
extern void Run(void);
extern void DelayService(void);

#endif
