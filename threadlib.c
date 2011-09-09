/******************************************************************************
 *
 *  File Name........: threadlib.c
 *
 *  Description......: File which contains all the thread related functions
 *
 *  Created by.......: Mayur Awaghade
 *
 *****************************************************************************/
#include<stdlib.h>
#include<ucontext.h>

#include"mythread.h"
#include"threadlib.h"

Mythread_queue ready_queue;  
_Mythread* CurrRunningThread

MyThread MyThreadCreate(void(*start_funct)(void *), void *args)
{	
	MyThread thread;
	int* size_arr;

	(_MyThread*) thread = (_MyThread*) malloc(sizeof(_MyThread));	
	if(thread == NULL)	//When we run out of mem ory
	{
		printf("Not enough memory to create new thread\n"
		return NULL;
	}

	size_arr = (int*) malloc(STACK_SIZE);
	{
		printf("Not enough memory to create new thread\n"
		free(thread);
		return NULL;
	}

	//thread -> childlist = thread -> next=NULL;
	
	/*we need to set the return point to main function. We might have to pass the context of main here.. 
	not sure about the flow of execution*/
	thread -> pthread.uc_link = NULL; 					

	thread -> pthread.uc_stack.ss_sp = size_arr;
	thread -> pthread.uc_stack.ss_size = sizeof(size_arr);
	
	//getcontext(&(thread -> pthread));	This line is probably not needed
	
	makecontext(&(thread -> pthread),(void(*)(void)) start_funct,0,args); //set context_child point to the function thread_fun
	add_to_queue(&ready_queue,thread);

}

void MyThreadYield(void)
{
	printf("In yield\nQueue empty: %d\n",null_queue(ready_queue));
	if(!(null_queue(ready_queue)))
	{
		printf("Inside if of yield\n");

		new_thread = rem_from_queue(&ready_queue);
		if(new_thread == NULL)
		{
			printf("No thread in ready to Execute currently\n");
			return;
		}
		getcontext(&(curr_thread -> pthread));			
		add_to_queue(&ready_queue,curr_thread);
		swapcontext(&(curr_thread -> pthread),&(new_thread -> pthread));
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


