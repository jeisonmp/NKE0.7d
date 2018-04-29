#ifndef __sysCALL_H
#define __sysCALL_H

#include "scheduler.h"
#include "kernel.h"

enum sys_temCall{
  TASKCREATE,
  SEM_WAIT,
  SEM_POST,
  SEM_INIT,
  WRITELCDN,
  WRITELCDS,
  EXITTASK,
  SLEEP,
  MSLEEP,
  USLEEP,
  LIGALED,
  START, 
  TASKJOIN,
  SETMYNAME,
  NKPRINT,
  GETMYNUMBER,
  //NKREAD, /// TODO: Nao implementado
  NKMALLOC,
  NKFREE
};

enum TaskEstados{
  INITIAL,
  READY,
  RUNNING,
  DEAD,
  BLOCKED
};

void DoSystemCall(unsigned int *pilha,Parameters *arg);
void sys_taskcreate(int *ID,void (*funcao)());
void sys_semwait(sem_t *semaforo);
void sys_sempost(sem_t *semaforo);
void sys_seminit(sem_t *semaforo, int ValorInicial);
void sys_taskexit(void);
void sys_sleep(unsigned int segundo);
void sys_msleep(unsigned int mili);
void sys_usleep(unsigned int micro);
void sys_ligaled(int valor);
void sys_start(int scheduler);
void sys_taskjoin(int identificador);
void sys_setmyname(const char *name);
void sys_nkprint(char *fmt,void *number);
void sys_getmynumber(int *number);
//void sys_nkread(char *tipo, void *value); /// TODO: Nao implementado

#endif
