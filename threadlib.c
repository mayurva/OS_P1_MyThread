#include"threadlib.h"
#include"mythread.h"

void addChild(Mythread *t)
{
	if(currThread -> childList == NULL)	currThread -> childList = t;
	else
	{
		Mythread *temp = currThread -> childList;
		while(temp -> sibling != NULL)
			temp = temp -> sibling;
		temp -> sibling = t;
	}
}


MyThread MyThreadCreate(void (*start_funct)(void *),void *args)
{
	Mythread *t2;
	t2 = (Mythread*) malloc(sizeof(Mythread));
	t2 -> thread = malloc(sizeof(ucontext_t));
	sizeArr = malloc(STACK_SIZE);

	t2 -> threadId = id;
	id++;
	t2 -> yieldFlag = FALSE;

#ifdef DEBUG_FLAG
	printf("In create thread\n");
	printf("Thread %d,flag %d\n",t2 -> threadId,t2->yieldFlag);
#endif	
 
	t2 -> thread -> uc_link = NULL; //set the return point to main function
	t2 -> thread -> uc_stack.ss_sp = sizeArr;
	t2 -> thread -> uc_stack.ss_size = STACK_SIZE;
	t2 -> childList = t2 -> sibling = NULL;
	t2 -> parentptr = currThread;
	t2 -> blocked = 0;
	t2 -> parentBlocked = FALSE;
	getcontext(t2 -> thread);
	makecontext(t2 -> thread,(void(*)(void)) start_funct,1,args); //set context_child point to the function thread_fun
	addToQueue(&readyQueue,t2);
	addChild(t2);
	
	//setcontext(t2->thread);
	return t2;
}


void MyThreadYield(void)
{
	Mythread *t1,*temp;
	//t1 = malloc(sizeof(Mythread));
	//t1 -> thread = malloc(sizeof(ucontext_t));

	currThread-> yieldFlag = TRUE;
	t1 = currThread;
	//t1 -> yieldFlag = currThread-> yieldFlag = TRUE;	
	//t1 -> threadId = currThread -> threadId;	

#ifdef DEBUG_FLAG
	printf("This is in the yield...of thread %d\n",currThread -> threadId);
	printQueue(readyQueue);
#endif

	getcontext(t1->thread);

#ifdef DEBUG_FLAG	
	printf("flag is %d and thread is %d\n", currThread -> yieldFlag,currThread -> threadId);
#endif

	if(currThread -> yieldFlag)
	{
		addToQueue(&readyQueue,t1);

#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif

//		temp = malloc(sizeof(Mythread));
//		temp -> thread = malloc(sizeof(ucontext_t));
		temp = remFromQueue(&readyQueue);

#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif
		
		printf("Yield flag of next thread %d is %d\n",temp -> threadId,temp -> yieldFlag);
		temp -> yieldFlag = FALSE;
		currThread = temp;

#ifdef DEBUG_FLAG
		printf("Now setting context to %d\n",currThread -> threadId);
#endif

		setcontext(temp->thread);
	}
	else
	{

#ifdef DEBUG_FLAG
		printf("Exiting from yield of thread %d\n", currThread -> threadId);
#endif

		return;
	} 
}

void deleteChild()
{
#ifdef DEBUG_FLAG
	printf("*************Inside delete child***********\n");
#endif
	Mythread *temp;
	Mythread* t = currThread -> parentptr;
	if(t != NULL)
	{
		temp = t -> childList;
		if(temp == NULL)	return;
		if(temp -> threadId == currThread -> threadId)	t -> childList = temp -> sibling;
		else
			while(temp -> sibling != NULL)
			{
				if(temp -> sibling -> threadId == currThread -> threadId)
				{
					temp -> sibling = temp -> sibling -> sibling;
					break;
				}	
				temp = temp -> sibling;
			}
		if(currThread->parentBlocked == TRUE)
		{
			temp = currThread -> parentptr;
			temp -> blocked = temp -> blocked -1;
		#ifdef DEBUG_FLAG	
			printf("blocked value for thread %d is %d\n",temp->threadId,temp->blocked);		
		#endif
			if(temp -> blocked <= 0)
				addToQueue(&readyQueue,temp);
		}
	}
	
}

int MyThreadJoin(MyThread thread)
{
	Mythread *t = currThread -> childList;
	int flag = -1;
	if(t!=NULL)
	{
		do
		{
			if(t->threadId = ((Mythread* ) thread) -> threadId)
			{
				currThread->blocked = currThread->blocked+1;
			#ifdef DEBUG_FLAG
				printf("blocked value for thread %d is %d\n",currThread->threadId,currThread->blocked);
			#endif
				t -> parentBlocked = TRUE;
				flag = 0;
				break;
			}
			t = t->sibling;
		}
		while(t!=NULL);
	}
	if(!flag)
	{
		currThread -> yieldFlag = TRUE;
		getcontext(currThread -> thread);
		if(currThread -> yieldFlag == TRUE)
		{

		#ifdef DEBUG_FLAG
			printQueue(readyQueue);
		#endif
			t = (Mythread*) remFromQueue(&readyQueue);

		#ifdef DEBUG_FLAG
			printQueue(readyQueue);
		#endif
		
			if(t == NULL)
			{
			#ifdef DEBUG_FLAG
				printf("No thread in ready to Execute currently\n");
			#endif
				return;
			}
			printf("Yield flag of next thread %d is %d\n",t -> threadId,t -> yieldFlag);
			t -> yieldFlag = FALSE;
			currThread = t;

		#ifdef DEBUG_FLAG
			printf("Inside join..Thread %d picked up from queue.. flag is %d\n",currThread->threadId,currThread -> yieldFlag);
			printQueue(readyQueue);
		#endif
			setcontext(t->thread);
		}
	}
	return flag;
}

void MyThreadJoinAll(void) 
{
	Mythread *t = currThread -> childList;
	if(t!=NULL)
	{
		do
		{
			currThread->blocked = currThread->blocked+1;
		#ifdef DEBUG_FLAG
			printf("blocked value for thread %d is %d\n",currThread->threadId,currThread->blocked);
		#endif
			t -> parentBlocked = TRUE;
			t = t->sibling;
		}
		while(t!=NULL);
	}
	currThread -> yieldFlag = TRUE;
	getcontext(currThread -> thread);
	if(currThread -> yieldFlag == TRUE)
	{
	#ifdef DEBUG_FLAG
		printQueue(readyQueue);
	#endif
		t = (Mythread*) remFromQueue(&readyQueue);
		
	#ifdef DEBUG_FLAG
		printQueue(readyQueue);
	#endif

		if(t == NULL)
		{
		#ifdef DEBUG_FLAG
			printf("No thread in ready to Execute currently\n");
		#endif
			return;
		}
		t -> yieldFlag = FALSE;
		currThread = t;

#ifdef DEBUG_FLAG
		printf("Inside join..Thread %d picked up from queue.. flag is %d\n",currThread->threadId,currThread -> yieldFlag);
		printQueue(readyQueue);
#endif
		setcontext(t->thread);
	}
}

void MyThreadExit(void)
{

#ifdef DEBUG_FLAG
	printf("Inside Exit  of thread %d\n",currThread->threadId);
#endif

	//if(!(nullQueue(readyQueue)))
	//{
		Mythread *t;
//		t = malloc(sizeof(Mythread));
//		t -> thread = malloc(sizeof(ucontext_t));

		deleteChild();
		free(currThread -> thread -> uc_stack.ss_sp); 
		free(currThread -> thread);
		free(currThread);
 
#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif

		t = (Mythread*) remFromQueue(&readyQueue);
		
		if(t == NULL)
		{
		#ifdef DEBUG_FLAG
			printf("No thread in ready to Execute currently\n");
		#endif
			return;
		}
		t -> yieldFlag = FALSE;
		currThread = t;

#ifdef DEBUG_FLAG
		printf("Thread %d picked up from queue.. flag is %d\n",currThread->threadId,currThread -> yieldFlag);
		printQueue(readyQueue);
#endif

		setcontext(t->thread);
		printf("********8ERROR/WARNING*******Function fall out");
}
