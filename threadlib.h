#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

#ifndef THREADLIB
#define THREADLIB

//#define DEBUG_FLAG

#define FALSE 0
#define TRUE 1
#define STACK_SIZE 64<<10	//check this for real threads

//typedef struct queue_;

typedef struct Mythread_{
	int threadId;
	int yieldFlag;
	int blocked;
	int parentBlocked;
	ucontext_t *thread;
	struct Mythread_ *next;
	struct Mythread_ *childList;
	struct Mythread_ *sibling;
	struct Mythread_ *parentptr;
}Mythread;

typedef struct queue_{
	Mythread *head,*tail;	
}queue;

typedef struct MysemNode_{
	Mythread *thread;
	struct MysemNode_ *next;
} MysemNode;

typedef struct Mysemaphore_{
	int value;
	MysemNode *head, *tail;
} Mysemaphore;

extern queue readyQueue;
extern int id;	//We are not using the id yet
extern Mythread *currThread;	
extern int *sizeArr;	//need to make this dynamic

//add more queue functions
Mythread* remFromQueue(queue*);
void addToQueue(queue*, Mythread*);	
void printQueue(queue);
int nullQueue(queue);
#endif
