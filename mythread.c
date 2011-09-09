#include<stdlib.h>
#include"mythread.h"

typedef struct _MyThread_{
	struct _MyThread_ *childlist;
	ucontext_t pthread;
	struct _MyThread_ *next;
} _MyThread;


MyThread MyThreadCreate(void(*start_funct)(void *), void *args)
{
	MyThread thread;
	int size_arr[100];	//this should be 8KB
	thread.childlist=thread.next=NULL;
	thread.pthread.uc_link = NULL; //set the return point to main function
	thread.pthread.uc_stack.ss_sp = size_arr;
	thread.pthread.uc_stack.ss_size = sizeof(size_arr);
	getcontext(&(thread.pthread));
	makecontext(&(thread.pthread),(void(*)(void)) start_funct,1,args); //set context_child point to the function thread_fun
	add_to_queue(&ready_queue,thread);
}

void MyThreadYield(void)
{
	printf("In yield\nQueue empty: %d\n",null_queue(ready_queue));
	if(!(null_queue(ready_queue)))
	{
		printf("Inside if of yeild\n");
		MyThread thread, temp;
		getcontext(&(thread.pthread));
		add_to_queue(&ready_queue,thread);
		thread = rem_from_queue(&ready_queue);
		setcontext(&(thread.pthread));
	}
}
void MyThreadExit(void)
{
	printf("Inside Exit\n");
	if(!(null_queue(ready_queue)))
	{
		MyThread thread;
		thread = rem_from_queue(&ready_queue);
		setcontext(&(thread.pthread));
	}
}


