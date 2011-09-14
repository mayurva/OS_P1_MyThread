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
		sem -> destroyed = FALSE;
		sem -> in_use = 0;
		sem -> head = sem -> tail = NULL;
	}
	else perror("ERROR: Not enough space to initialize the semaphore\nSemaphore not created. Function returns NULL\n");
	return sem;
}

// Signal a semaphore
void MySemaphoreSignal(MySemaphore sem)
{
	if(sem != NULL)
	{
		Mysemaphore *semptr =  (Mysemaphore*)sem;
		if(semptr -> destroyed == FALSE)
		{	
			semptr -> value++;
			semptr -> in_use--;		
			if(semptr -> value > 0)
			{
				//check if wait queue is not null
				if(semptr -> head != NULL) 
				{
					//remove 1st element from wait queue
					Mythread *t;
					MysemNode *sn;
					sn = semptr -> head;
					if(semptr->head->next == NULL)	semptr->head = semptr -> tail = NULL;
					else	semptr->head = semptr -> head -> next;
					t = sn -> thread;
					free(sn);
					t -> blocked = t -> blocked -1;	//The thread is no more blocking on this semaphore.. decreament the blocked counter
				#ifdef DEBUG_FLAG	
					printf("blocked value for thread %d is %d\n",t->threadId,t->blocked);		
					printSem(sem);
				#endif
				//If this was the last resource current thread was blocked on, then add it to ready queue
					if(t -> blocked <= 0)	
						addToQueue(&readyQueue,t);
				}
			}
		}
	}
}	

// Wait on a semaphore
void MySemaphoreWait(MySemaphore sem)
{
	if(sem!=NULL)
	{	
		Mythread *t;
		Mysemaphore *semptr =  (Mysemaphore*)sem;
		if(semptr -> destroyed == FALSE)
		{	
			t = currThread;
			//Getting context here is important. This makes sure that whenever thread resumes execution, it rechecks the availability of the resource
			getcontext(t -> thread);	
			if(semptr -> value > 0)
			{
				semptr -> value--;
				semptr -> in_use++;
			}
			else
			{
				t -> blocked = t -> blocked+1;		//increament the blocked count
		
				//add to blocked queue of semaphore
				MysemNode *sn;
				sn = malloc(sizeof(MysemNode));
				if(sn == NULL)	perror("ERROR:Not enough memory\nCan't add the thread to semaphore wait queue\n");
				else
				{
					sn -> thread = t;
					if(semptr->head == NULL)	semptr->head = semptr-> tail = sn;
					else
					{
						semptr->tail->next = sn;
						semptr->tail = sn;
					}
					sn -> next = NULL;
				}
				//remove next thread from the ready queue and set its context
				t = remFromQueue(&readyQueue);
				if(t == NULL)
				{
				#ifdef DEBUG_FLAG	
					printf("No thread to execute in ready queue\n");
				#endif
					setcontext(mainPrg);
				}
				currThread = t;
				setcontext(t->thread);
			}
		}
	}
}

// Destroy a semaphore
int MySemaphoreDestroy(MySemaphore sem)
{
	Mysemaphore *semptr =  (Mysemaphore*)sem;
	if(semptr == NULL)	return -1;
	else if (semptr -> head != NULL || semptr -> in_use != 0)	return -1;
	else 
	{
		semptr -> destroyed = TRUE;
		return 0;
	}
}
