#include<stdio.h>
#include"threadlib.h"
#include"mythread.h"

void thread_fun1(void *args)
{
	printf("We are in Thread_fun1\n");
	printf("Thread_fun1 yields now\n");
	MyThreadYield();
	printf("Back into the thread_fun1\n");
	MyThreadExit();
}

void thread_fun2(void *args)
{
	printf("We are in Thread_fun2\n");
	printf("Thread_fun2 yields now\n");
	MyThreadYield();
	printf("Back into the thread_fun2\n");	
	MyThreadExit();
}

void thread_fun3(void *args)
{
	printf("We are in Thread_fun3\n");
	printf("Thread_fun3 yields now\n");
	MyThreadYield();
	printf("Back into the thread_fun3\n");	
	MyThreadExit();
}

void main_thread(void *args)
{
	MyThread t1,t2,t3;
	t1 = MyThreadCreate(thread_fun1,args);
	t2 = MyThreadCreate(thread_fun2,args);
	t3 = MyThreadCreate(thread_fun3,args);
	printf("We are in main Thread\n");
	printf("main Thread yields now\n");
	MyThreadYield();
	printf("We are in main thread after resuming. this thread had yielded before\n");
	printf("Main thread joins 2.. this is to make sure that main thread finished in the end\n");

	MyThreadJoin(t2);
	printf("Main thread joins all whatever remains.. this is to make sure that main thread finished in the end\n");
	MyThreadJoinAll();	
/*	MyThreadJoin(t2);
	printf("Main thread joins 3.. this is to make sure that main thread finished in the end\n");
	MyThreadJoin(t3);*/
	MyThreadExit();
}

int main()
{
	MyThreadInit(main_thread,(void*)1);
	MyThreadRun();
	printf("Reached end of main\n");
	return 0;	
}


