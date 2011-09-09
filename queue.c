/******************************************************************************
 *
 *  File Name........: threadlib.c
 *
 *  Description......: File which contains all the Queue
 *
 *  Created by.......: Mayur Awaghade
 *
 *****************************************************************************/


#include"mythread.h"
#include"threadlib.h"

int null_queue(Mythread_queue queue)
{ 
	if (queue.head == NULL)	return TRUE;
	return FALSE;
}

void init_queue(Mythread_queue *queue) { queue -> head = queue -> tail = NULL; }

void add_to_queue(Mythread_queue *queue, MyThread thread)
{
	if(!(null_queue(ready_queue))) queue -> tail -> next = thread;
	else queue -> head = thread;
	queue -> tail = thread;
	queue -> tail -> next =  NULL;
}

Mythread rem_from_queue(Mythread_queue *queue)
{
	MyThread thread;
	thread = queue -> head;
	if(queue->head != NULL)	queue -> head = queue -> head -> next;
	return thread;	
}

