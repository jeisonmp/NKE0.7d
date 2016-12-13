#include "kernel.h"

unsigned int KernelStack[300];
unsigned short TaskRunning=0;
unsigned int NumberTaskAdd=0;

unsigned short serial_fila[MaxNumberTask];
unsigned short posicao_a_inserir = 0;
unsigned short posicao_a_att = 0;

TaskDescriptor Descriptors[MaxNumberTask];
ReadyList ready_queue;
SerialData serial_queue[MaxNumberTask];
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
