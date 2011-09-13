#include<stdlib.h>
#include"threadlib.h"
#include"mythread.h"

int nullQueue(queue q)
{
	if(q.head == NULL) return TRUE;
	return FALSE;
}
void addToQueue(queue *q, Mythread *ptr)
{
	if(q->head == NULL)
	{
		q->head = q-> tail = ptr;
	}
	else
	{
		q->tail->next = ptr;
		q->tail = ptr;
	}
	ptr -> next = NULL;
} 

Mythread* remFromQueue(queue *q)
{
	Mythread *ptr;
	ptr = q -> head;
	if(q -> head!=NULL)
	{
		if(q->head->next == NULL)	q->head = q -> tail = NULL;
		else	q->head = q -> head -> next;
		ptr -> yieldFlag = FALSE;
	}
	return ptr;
}

void printQueue(queue q)
{
	Mythread *ptr;
	ptr = q.head;
	printf("Queue is:");
	while(ptr)
	{
		printf("\tThread id: %d, Yield Flag: %d",ptr->threadId,ptr->yieldFlag);
		ptr = ptr -> next;
	}
	printf("\n");	
} 
