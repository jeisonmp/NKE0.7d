#include "kernel.h"
unsigned int KernelStack[300];
unsigned short TaskRunning=0;
unsigned int NumberTaskAdd=0;
unsigned int NumberTaskAddAll=0;
TaskDescriptor Descriptors[MaxNumberTask];
ReadyList ready_queue;
int SchedulerAlgorithm=0;

#ifdef SHARED_NUMBER
  int shared[SHARED_NUMBER-1];
#endif

void idletask(void)
{
  setmyname(__FUNCTION__);
  while(1);
}

/* Estas rotinas sao chamados pelo crt.S. 
Devem existir, mas ainda nao estao sendo usadas */
void UNDEF_Routine(){}
void FIQ_Routine(){}