#include"threadlib.h"
#include"mythread.h"

//To add a thread t as a child to current Thread
void addChild(Mythread *t)
{
	if(t != NULL)
	{
		if(currThread -> childList == NULL)	currThread -> childList = t;
		else
		{
			Mythread *ptr = currThread -> childList;
			while(ptr -> sibling != NULL)
				ptr = ptr -> sibling;
			ptr -> sibling = t;
		}
	}
}

//Create a new Thread
MyThread MyThreadCreate(void (*start_funct)(void *),void *args)
{
	Mythread *t;
	t = (Mythread*) malloc(sizeof(Mythread));
	if(t == NULL)	printf("ERROR:Not enough memory to create a Thread.. No thread created\n");
	else
	{
		t -> thread = malloc(sizeof(ucontext_t));
		if(t->thread == NULL)	printf("ERROR:Not enough memory to create a Thread.. No thread created\n");
		else
		{
			sizeArr = malloc(STACK_SIZE);
			if(sizeArr == NULL)	printf("ERROR:Not enough memory to create a Thread.. No thread created\n");
			else
			{

				t -> threadId = id;	//set the thread id
				id++;
				t -> yieldFlag = FALSE;
	
			#ifdef DEBUG_FLAG
				printf("In create thread\n");
				printf("Thread %d,flag %d\n",t -> threadId,t->yieldFlag);
			#endif	

				t -> childList = t -> sibling = NULL;
				t -> parentptr = currThread;
				t -> blocked = 0;
				t -> parentBlocked = FALSE;
 
				//Create context of the thread. This code was referred from wikipedia link mentioned in REFERENCES
				t -> thread -> uc_link = NULL; //set the return point to main function
				t -> thread -> uc_stack.ss_sp = sizeArr;
				t -> thread -> uc_stack.ss_size = STACK_SIZE;
				
				getcontext(t -> thread);
				makecontext(t -> thread,(void(*)(void)) start_funct,1,args); //set context_child point to the function thread_fun
				
				addToQueue(&readyQueue,t);	//add the thread to ready queue
				addChild(t);		//add t as a child to current thread
			}
		}
	}
	return t;
}


void MyThreadYield(void)
{
	Mythread *t;

	currThread-> yieldFlag = TRUE;	//to indicate the current thread is yielding
	t = currThread;

#ifdef DEBUG_FLAG
	printf("This is in the yield...of thread %d\n",currThread -> threadId);
	printQueue(readyQueue);
#endif

	getcontext(t->thread);	//save current context of the thread

#ifdef DEBUG_FLAG	
	printf("flag is %d and thread is %d\n", currThread -> yieldFlag,currThread -> threadId);
#endif

	if(currThread -> yieldFlag)
	{
		addToQueue(&readyQueue,t);

#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif
		//remove next thread from ready queue
		t = remFromQueue(&readyQueue);

#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif
		
		t -> yieldFlag = FALSE;
		currThread = t;

#ifdef DEBUG_FLAG
		printf("Now setting context to %d\n",currThread -> threadId);
#endif

		setcontext(t->thread);
	}
	else
	{

#ifdef DEBUG_FLAG
		printf("Exiting from yield of thread %d\n", currThread -> threadId);
#endif

		return;
	} 
}

//function to remove a child from parent because child is exiting
void deleteChild()
{
#ifdef DEBUG_FLAG
	printf("*************Inside delete child***********\n");
#endif
	Mythread *temp;
	Mythread *t = currThread -> parentptr;
	if(t != NULL)
	{
		//find and remove the child in parents child list
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
		//check if parent was waiting on child
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

//Join the current thread with specified child
int MyThreadJoin(MyThread thread)
{
	int flag = -1;
	if(thread!=NULL)
	{
		Mythread *t = currThread -> childList;
		if(t!=NULL)
		{
			//find if child is present in the parent's child list
			do
			{
				if(t->threadId = ((Mythread* ) thread) -> threadId)
				{
				//child found. block the parent on it
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
			//Current thread should yield here
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
	}
	return flag;
}

void MyThreadJoinAll(void) 
{
	Mythread *t = currThread -> childList;
	if(t!=NULL)
	{
		//traverse the whole child list of the current thread and block it on each of them
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
	//this thread should yield now
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
		Mythread *t;
		deleteChild();	//Delete from parent's child list
	
		//free the allocated memory
		free(currThread -> thread -> uc_stack.ss_sp); 
		free(currThread -> thread);
		free(currThread);
 
#ifdef DEBUG_FLAG
		printQueue(readyQueue);
#endif

		//set the next thread for execution		
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
#ifdef DEBUG_FLAG
		printf("********8ERROR/WARNING*******Function fall out");
#endif
}
