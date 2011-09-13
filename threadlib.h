#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

#ifndef THREADLIB
#define THREADLIB

//#define DEBUG_FLAG

#define FALSE 0
#define TRUE 1
#define STACK_SIZE 8<<10	//this is the stack size of thread 64KB

//Structure to represent the thread
typedef struct Mythread_{
	int threadId;			//thread id

	int yieldFlag;			
	int blocked;	//this flag indicates the sum of children and semaphores on which the the thread is blocked
	int parentBlocked;	//Indicates if parent is waiting for child to end
	ucontext_t *thread;	//used to store 
	struct Mythread_ *next;	//to maintain the ready queue
	struct Mythread_ *childList;	//to maintain list of children
	struct Mythread_ *sibling;	//to maintain list of children
	struct Mythread_ *parentptr;	//points to the parent
}Mythread;

//This structure is used to maintain the ready queue
typedef struct queue_{
	Mythread *head,*tail;	
}queue;

//This structure is used to store a thread on the list which stores the threads blocked on a semaphore
typedef struct MysemNode_{
	Mythread *thread;
	struct MysemNode_ *next;
} MysemNode;

//This structure is used to store a semaphore
typedef struct Mysemaphore_{
	int value;
	MysemNode *head, *tail;
} Mysemaphore;

//Global required all across the library
extern queue readyQueue;	//to store the ready to execute threads
extern int id;	//To maintain thread ids
extern Mythread *currThread;	//Points to the Mythread structure of currently running thread
extern int *sizeArr;	//Pointer 

//Definitions of all queue related functions
Mythread* remFromQueue(queue*);
void addToQueue(queue*, Mythread*);	
void printQueue(queue);
int nullQueue(queue);
#endif
