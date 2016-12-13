#include "initkernel.h"

void StartKernel(void)
{
  StartLED();
  InitTimers();
  LCDinit();
  U0init();
  ini_serial_fila();
  StartReadyList();
  CreateIdleAndMain();
  InitT0();
  MoveToSP(&Descriptors[1].Stack[SizeTaskStack-1]);
  IRQT0(); //liga o kernel
}

void CreateIdleAndMain(void)
{
  static int TidMain=1, TidIdle=0;
  /*Criaçao da Idle Task */
  Descriptors[0].Tid=TidIdle;
  Descriptors[0].EP=idletask;
  Descriptors[0].State=INITIAL;
  Descriptors[0].Join=0;
  Descriptors[0].SP=&Descriptors[0].Stack[SizeTaskStack-1];
  Descriptors[0].Prio=0;
  /*Criaçao da Main */
  Descriptors[1].Tid=TidMain;
  Descriptors[1].EP=(void(*)())main;
  Descriptors[1].State=INITIAL;
  Descriptors[1].Join=0;
  Descriptors[1].SP=&Descriptors[1].Stack[SizeTaskStack-1];
  Descriptors[1].Prio=0;
  TaskRunning=1;
  NumberTaskAdd=1;
}

void StartReadyList(void)
{
  int i;
  for(i=0;i<MaxNumberTask-1;i++)
	  ready_queue.queue[i]=0;
  ready_queue.head=0;
  ready_queue.tail=0;
  return;
}

void ini_serial_fila(void)
{
   int i;
	for(i=0; i<MaxNumberTask; i++)
	     serial_fila[i] = 0;
}
