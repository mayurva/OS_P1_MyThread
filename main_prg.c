/******************************************************************************
 *
 *  File Name........: main_prg.c
 *
 *  Description......: Main program for testing purpose
 *
 *  Created by: Mayur Awaghade
 *
 *****************************************************************************/

#include"mythread.h"
#include"threadlib.h"
Mythread_queue ready_queue;

//This is function for the thread
void thread_fun(void *args)
{
	int *flag;
	sscanf(args, "%p", &flag);	
	printf("In child context\n");
	*flag = 1;
	printf("Child will exit now\n");
	MyThreadExit();	
	printf("This line should not get printed I suppose\n");
}


int main()
{
	int flag = 0;
	void *args;
	init_queue(&ready_queue);

	printf("Main program Started\n");

	args = malloc(sizeof(char)*10);	
	sprintf(args,"%p",&flag);
	MyThreadCreate(thread_fun,args);
	printf("Child process created and added to queue\n");

	printf("main program yields now\n");
	MyThreadYield();

	printf("Back to main program\n");
	MyThreadExit();	

	printf("Main program will exit now\n");
	return 0;


