#include "syscall.h"
#include "smalloc.h"
//#include "tlsf.h"

void DoSystemCall(unsigned int *pilha,Parameters *arg)
{ 
  Descriptors[TaskRunning].SP=pilha;
  MoveToSP(&KernelStack[289]);
  int timeini = 0;
  int timefim = 0;
  int tempo = 0;
  
#ifdef SHARED_NUMBER
  TTYshared(1);
#endif
  switch(arg->CallNumber)
  {
    case TASKCREATE:
      sys_taskcreate((int *)arg->p0,(void(*)())arg->p1);
      break;
    case SEM_WAIT:
      sys_semwait((sem_t *)arg->p0);
      break;
    case SEM_POST:
      sys_sempost((sem_t *)arg->p0);
      break;
    case SEM_INIT:
      sys_seminit((sem_t *)arg->p0,(int)arg->p1);
      break;
    case WRITELCDN:
      LCDcomando((int)arg->p1);
      LCDnum((int)arg->p0);
      break;
    case WRITELCDS:
      LCDcomando((int)arg->p1);
      LCDputs((char*)arg->p0);
      break;
    case EXITTASK:
      sys_taskexit();
      break;
    case SLEEP:
      sys_sleep((int)arg->p0);
      break;
    case MSLEEP:
      sys_msleep((int)arg->p0);
      break;
    case USLEEP:
      sys_usleep((int)arg->p0);
      break;
    case LIGALED:
      sys_ligaled((int)arg->p0);
      break;
    case START:
      sys_start((int)arg->p0);
      break;
    case TASKJOIN:
      sys_taskjoin((int)arg->p0);
      break;
    case SETMYNAME:
      sys_setmyname((const char *)arg->p0);
      break;
    case NKPRINT:
       sys_nkprint((char *)arg->p0,(void *)arg->p1);
       break;
    case GETMYNUMBER:
       sys_getmynumber((int *)arg->p0);
       break;
    case NKREAD:
       sys_nkread((char *)arg->p0,(void *)arg->p1);
       break;
    case NKMALLOC:
       timeini = T1TC * 2;

       *arg->p0 = smalloc((int)arg->p1);
       //*arg->p0 = k_malloc((int)arg->p1);

       
       timefim = T1TC * 2;
       tempo = timefim - timeini;
       
       if (tempo < tmenor)
       {
          tmenor = tempo;
       }
       if (tempo > tmaior)
       {
          tmaior = tempo;
       }
       cont++;
       soma+= tempo;
       tmedia = soma/cont;
           
       sys_nkprint(" menor: %d ", tmenor);
       sys_nkprint(", maior: %d ", tmaior);
       sys_nkprint(", media: %d ", tmedia);
       sys_nkprint(", tempo: %dus", tempo);
       sys_nkprint(", soma: %d", soma);
       sys_nkprint(", cont: %d\n", cont);
       
       break;
    case NKFREE:
       sfree((void*)arg->p0);
       //k_free((void*)arg->p0);
       break;
    default:
       break;
  }
  RestoreContext(Descriptors[TaskRunning].SP);
}

void sys_taskcreate(int *ID,void (*funcao)())
{
  NumberTaskAdd++;
  *ID=NumberTaskAdd;
  Descriptors[NumberTaskAdd].Tid=*ID;
  Descriptors[NumberTaskAdd].EP=funcao;
  Descriptors[NumberTaskAdd].State=INITIAL;
  Descriptors[NumberTaskAdd].Join=0;
  Descriptors[NumberTaskAdd].Time=0;
  Descriptors[NumberTaskAdd].Prio=0;
  Descriptors[NumberTaskAdd].SP=&Descriptors[NumberTaskAdd].Stack[SizeTaskStack-1];
  return;
}

void sys_semwait(sem_t *semaforo)
{
    semaforo->count--;
    if(semaforo->count < 0)
    {
	    semaforo->sem_queue[semaforo->tail] = TaskRunning;
	    Descriptors[TaskRunning].State = BLOCKED ;
	    semaforo->tail++;
	    if(semaforo->tail == MaxNumberTask-1) semaforo->tail = 0;
	      Dispatcher();
    }
}

void sys_sempost(sem_t *semaforo)
{
    semaforo->count++;
    if(semaforo->count <= 0)
    {
      Descriptors[semaforo->sem_queue[semaforo->header]].State = READY;
      InsertReadyList(semaforo->sem_queue[semaforo->header]);
      semaforo->header++;
      if(semaforo->header == MaxNumberTask-1) semaforo->header = 0;
    }
}

void sys_seminit(sem_t *semaforo, int ValorInicial)
{
    semaforo->count = ValorInicial;
    semaforo->header = 0;
    semaforo->tail = 0;
}

void sys_taskexit(void)
{
  Descriptors[TaskRunning].State=DEAD;
  Dispatcher();
}

void sys_sleep(unsigned int segundo)
{
  Descriptors[TaskRunning].Time = segundo/ClkT;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_msleep(unsigned int mili)
{
  Descriptors[TaskRunning].Time = (mili/ClkT)/1000;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_usleep(unsigned int micro)
{
  Descriptors[TaskRunning].Time = (micro/ClkT)/1000000;
  if(Descriptors[TaskRunning].Time > 0)
  {
    Descriptors[TaskRunning].State = BLOCKED;
    Dispatcher();
  }
}

void sys_ligaled(int valor)
{
  LigaLED(valor);
}

void sys_start(int scheduler)
{
  int i=2;
  SchedulerAlgorithm=scheduler;
  Descriptors[1].State=BLOCKED;
  switch (SchedulerAlgorithm)
  {
    case RR:
      for(i=2;i<=NumberTaskAdd;i++)
	     InsertReadyList(i);
      break;
    default:
      break;
  }
#ifdef DEBUG_START
  TTYstart();
#endif
  Dispatcher();
}

void sys_taskjoin(int identificador)
{
  if(Descriptors[identificador].State!=DEAD)
  {
    Descriptors[TaskRunning].State=BLOCKED;
    Descriptors[TaskRunning].Join=identificador;
    Dispatcher();
  }
}

void sys_setmyname(const char *name)
{
  Descriptors[TaskRunning].name=name;
}

void sys_getmynumber(int *number)
{
  *number=Descriptors[TaskRunning].Tid;
}
