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
ucontext_t *mainPrg;

void MyThreadInit(void(*startTh_funct)(void *), void *args)
{
	//Initialize the global data structures
	id = 0;
	readyQueue.head = readyQueue.tail = NULL;

	//Initiate data structure to create main thread
	mainThread = (Mythread*) malloc(sizeof(Mythread));
	if(mainThread == NULL)
	{
		perror("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}
	mainThread -> thread = (ucontext_t*) malloc(sizeof(ucontext_t));
	if(mainThread -> thread == NULL)
	{
		perror("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}
	sizeArr = malloc(STACK_SIZE);
	if(sizeArr == NULL)
	{
		perror("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}

	mainThread -> yieldFlag = FALSE;	//Refer:yieldFlag in threadlib.h
	mainThread -> threadId = id;		//thread id is zero for main thread
	mainThread -> blocked = 0;
	mainThread -> parentBlocked = FALSE;
	mainThread -> childList = mainThread -> sibling = mainThread -> parentptr = NULL;

	//Setup context of main thread. This code was referred from wikipedia link mentioned in REFERENCES	
	mainThread -> thread -> uc_link = NULL; 
	mainThread -> thread -> uc_stack.ss_sp = sizeArr;
	mainThread -> thread -> uc_stack.ss_size = STACK_SIZE;

	getcontext(mainThread -> thread);
	makecontext(mainThread -> thread,(void(*)(void)) startTh_funct,1,args);

	//Creating the return context
	mainPrg = malloc(sizeof(ucontext_t));
	if(mainPrg == NULL)
	{
		perror("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}
	sizeArr = malloc(STACK_SIZE);
	if(sizeArr == NULL)
	{
		perror("ERROR:Not enough memory to create main Thread.. exiting..\n");
		exit(-1);
	}

	mainPrg -> uc_link = NULL; 
	mainPrg -> uc_stack.ss_sp = sizeArr;
	mainPrg -> uc_stack.ss_size = STACK_SIZE;
	getcontext(mainPrg);
	makecontext(mainPrg,(void(*)(void)) MyThreadRun,1,args);
		
	//Setup data structures before starting execution
	currThread = mainThread;
	id++;

	setcontext(mainThread->thread);	//Start the execution of main thread
}

void MyThreadRun(void)
{
	free(mainPrg -> uc_stack.ss_sp);
	free(mainPrg);
}
