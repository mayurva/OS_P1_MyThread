/******************************************************************************
 *
 *  File Name........: threadlib.h
 *
 *  Description......: Common data structures definition and declaration
 *
 *  Created by Mayur Awaghade
 *
 *****************************************************************************/
#ifndef THREADLIB_H
#define THREADLIB_H

#define TRUE 1
#define FLASE 0

#define STACK_SIZE 8<<10

//This structure is used to store a context of MyThread
typedef struct _MyThread_{
	struct _MyThread_ *childlist;
	ucontext_t pthread;
	struct _MyThread_ *next;
} _MyThread;

//Below lines declare Thread related functions

//This structure points to a dynamic queue of threads
typedef struct MyThread_list_t{
	_MyThread *head;
	_MyThread *tail;
} Mythread_queue;

//Below lines declare Queue related functions
extern Mythread_queue ready_queue;
extern _Mythread* currRunningThread;

Mythread RemFromQueue(Mythread_queue*);
void InitQueue(Mythread_queue*);
void AddToQueue(Mythread_queue*,MyThread thread);

#endif //THREADLIB_H
