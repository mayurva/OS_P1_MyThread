#include <stdio.h>
#include "mythread.h"

int waits, signals, initv, yield;
MySemaphore sem;

void signalt(void *who)
{
printf("SIGNAL%d start\n", (int)who);
if (yield) MyThreadYield();
MySemaphoreSignal(sem);
printf("SIGNAL%d end\n", (int)who);
MyThreadExit();
}

void waitt(void *who)
{
printf("WAIT%d start\n", (int)who);
MySemaphoreWait(sem);
printf("WAIT%d end\n", (int)who);
MyThreadExit();
}

void t0(void *dummy)
{
int i;

sem = MySemaphoreInit(initv);
for (i = 0; i < waits; i++) {
MyThreadCreate(waitt, (void *)i);
}
for (i = 0; i < signals; i++) {
MyThreadCreate(signalt, (void *)i);
}
MyThreadJoinAll();
MySemaphoreDestroy(sem);
}

int main(int argc, char *argv[])
{
if (argc != 5) {
printf("Usage: %s <init value> <nwaits> <nsignals> <yield>\n", argv[0]);
return -1;
}
initv = atoi(argv[1]);
waits = atoi(argv[2]);
signals = atoi(argv[3]);
yield = atoi(argv[4]);
MyThreadInit(t0, 0);
MyThreadRun();
}
