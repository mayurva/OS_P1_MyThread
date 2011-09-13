#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include"threadlib.h"
#include"mythread.h"

//Definition of Globals
queue readyQueue;
Mythread *currThread;
Mythread *mainThread;
int *sizeArr;
int id;

void MyThreadInit(void(*startTh_funct)(void *), void *args)
{
	//Initialize the global data structures
	id = 0;
	readyQueue.head = readyQueue.tail = NULL;

	//Initiate data structure to create main thread
	mainThread = (Mythread*) malloc(sizeof(Mythread));
	if(mainThread == NULL)
	{
		printf("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}
	mainThread -> thread = (ucontext_t*) malloc(sizeof(ucontext_t));
	if(mainThread -> thread == NULL)
	{
		printf("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}
	sizeArr = malloc(STACK_SIZE);
	if(sizeArr == NULL)
	{
		printf("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}

	mainThread -> yieldFlag = FALSE;	//Refer:yieldFlag in threadlib.h
	mainThread -> threadId = id;		//thread id is zero for main thread
	mainThread -> blocked = 0;
	mainThread -> parentBlocked = FALSE;

	//Setup context of main thread. This code was referred from wikipedia link mentioned in REFERENCES	
	mainThread -> thread -> uc_link = NULL; 
	mainThread -> thread -> uc_stack.ss_sp = sizeArr;
	mainThread -> thread -> uc_stack.ss_size = STACK_SIZE;
	mainThread -> childList = mainThread -> sibling = mainThread -> parentptr = NULL;
	getcontext(mainThread -> thread);
	makecontext(mainThread -> thread,(void(*)(void)) startTh_funct,1,args);
}

void MyThreadRun(void)
{
	//Setup data structures before starting execution
	currThread = mainThread;
	id++;

	setcontext(mainThread->thread);	//Start the execution of main thread
}
