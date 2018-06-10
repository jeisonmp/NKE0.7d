#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "kernel.h"
#include "../Placa/lcd.h"
#include "../Placa/interrupt.h"
#include "syscall.h"
#include "initkernel.h"
#include "debug.h"
#include <string.h>

enum Scheduler{
  RR,
  RM,
  EDF
};

void Select(unsigned int *pilha);
void InsertReadyList(int id);
void SetTime(void);
void Dispatcher(void);
void UseKernelStack(void);
void WakeUP(void);
void DoNets(void);

#endif