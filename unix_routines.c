#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include"threadlib.h"
#include"mythread.h"

queue readyQueue;
Mythread *currThread;
Mythread *mainThread;
int *sizeArr;
int id;

void MyThreadInit(void(*startTh_funct)(void *), void *args)
{
	id = 0;
	readyQueue.head = readyQueue.tail = NULL;

	mainThread = (Mythread*) malloc(sizeof(Mythread));
	mainThread -> thread = (ucontext_t*) malloc(sizeof(ucontext_t));
	sizeArr = malloc(STACK_SIZE);

	mainThread -> yieldFlag = FALSE;	
	mainThread -> threadId = id;

	id++;	
	mainThread -> thread -> uc_link = NULL; //set the return point to main function
	mainThread -> thread -> uc_stack.ss_sp = sizeArr;
	mainThread -> thread -> uc_stack.ss_size = STACK_SIZE;
	mainThread -> childList = mainThread -> sibling = mainThread -> parentptr = NULL;
	mainThread -> blocked = 0;
	mainThread -> parentBlocked = FALSE;
	
	getcontext(mainThread -> thread);
	makecontext(mainThread -> thread,(void(*)(void)) startTh_funct,1,args);
	currThread = mainThread;
}

void MyThreadRun(void)
{
	ucontext_t temp;
	getcontext(&temp);
	swapcontext(&temp,mainThread->thread);
//	setcontext(mainThread->thread);
}
