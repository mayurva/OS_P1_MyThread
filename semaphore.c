#include<stdlib.h>
#include<stdio.h>
#include"threadlib.h"
#include"mythread.h"

// Create a semaphore
MySemaphore MySemaphoreInit(int initialValue)
{
	Mysemaphore *sem;
	sem = malloc(sizeof(Mysemaphore));
	if(sem != NULL)
	{
		sem -> value = initialValue;
		sem -> head = sem -> tail = NULL;
	}
	return sem;
}

// Signal a semaphore
void MySemaphoreSignal(MySemaphore sem)
{
//	sem = (Mysemaphore)sem;
	Mysemaphore *semptr =  (Mysemaphore*)sem;
	semptr -> value++;
	if(semptr -> value > 0)
	{
		//check if wait queue is not null
		if(semptr -> head != NULL) 
		{
			//remove from wait queue
			Mythread *t;
			MysemNode *sn;
			sn = semptr -> head;
			if(semptr->head->next == NULL)	semptr->head = semptr -> tail = NULL;
			else	semptr->head = semptr -> head -> next;
			t = sn -> thread;
			free(sn);
			t -> blocked = t -> blocked -1;
		#ifdef DEBUG_FLAG	
			printf("blocked value for thread %d is %d\n",t->threadId,t->blocked);		
		#endif
			if(t -> blocked <= 0)
				addToQueue(&readyQueue,t);
		}
	}
}

// Wait on a semaphore
void MySemaphoreWait(MySemaphore sem)
{
	Mythread *t;
	Mysemaphore *semptr =  (Mysemaphore*)sem;
	t = currThread;
	getcontext(t -> thread);
	if(semptr -> value > 0)	semptr -> value--;
	else
	{
		t -> blocked = t -> blocked+1;

		//add to blocked queue of semaphore
		MysemNode *sn;
		sn = malloc(sizeof(MysemNode));
		sn -> thread = t;
		if(semptr->head == NULL)	semptr->head = semptr-> tail = sn;
		else
		{
			semptr->tail->next = sn;
			semptr->tail = sn;
		}
		sn -> next = NULL;
	
		//remove next thread from queue and set its context
		t = remFromQueue(&readyQueue);
		currThread = t;
		setcontext(t->thread);
	}
}

// Destroy on a semaphore
int MySemaphoreDestroy(MySemaphore sem)
{
	//remove all blocked threads and add to ready queue
	Mysemaphore *semptr =  (Mysemaphore*)sem;
	MysemNode *sn,*temp;
	sn = semptr -> head;
	while(sn != NULL) 
	{
		//remove from wait queue
		Mythread *t;
		t = sn -> thread;
		t -> blocked = t -> blocked -1;
	#ifdef DEBUG_FLAG	
		printf("blocked value for thread %d is %d\n",t->threadId,t->blocked);		
	#endif
		if(t -> blocked <= 0)
			addToQueue(&readyQueue,t);
		temp = sn;
		sn = sn -> next;
		free(temp);
	}
}
