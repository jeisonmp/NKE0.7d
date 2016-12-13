/*
 * usercall.c
 *
 *  Created on: 30/07/2014
 *      Author: lucas
 */

#include "usercall.h"

void taskcreate(int *ID,void (*funcao)()) //parametros armazenados em R0 e R1 na chamada
{
  Parameters arg;
  arg.CallNumber=TASKCREATE;
  arg.p0=(unsigned char *)ID;
  arg.p1=(unsigned char *)funcao;
  CallSWI(0,&arg);
}

void semwait(sem_t *semaforo)
{
  Parameters arg;
  arg.CallNumber=SEM_WAIT;
  arg.p0=(unsigned char *)semaforo;
  CallSWI(0,&arg);
}

void sempost(sem_t *semaforo)
{
  Parameters arg;
  arg.CallNumber=SEM_POST;
  arg.p0=(unsigned char *)semaforo;
  CallSWI(0,&arg);
}

void seminit(sem_t *semaforo, int ValorInicial)
{
  Parameters arg;
  arg.CallNumber=SEM_INIT;
  arg.p0=(unsigned char *)semaforo;
  arg.p1=(unsigned char *)ValorInicial;
  CallSWI(0,&arg);
}

void writelcdn(int msg,int pos)
{
  Parameters arg;
  arg.CallNumber=WRITELCDN;
  arg.p0=(unsigned char *)msg;
  arg.p1=(unsigned char *)pos;
  CallSWI(0,&arg);
}

void writelcds(char* msg,int pos)
{
  Parameters arg;
  arg.CallNumber=WRITELCDS;
  arg.p0=(unsigned char *)msg;
  arg.p1=(unsigned char *)pos;
  CallSWI(0,&arg);
}

void taskexit(void)
{
  Parameters arg;
  arg.CallNumber=EXITTASK;
  CallSWI(0,&arg);
}

void sleep(int time)
{
  Parameters arg;
  arg.CallNumber=SLEEP;
  arg.p0=(unsigned char *)time;
  CallSWI(0,&arg);
}

void msleep(int time)
{
  Parameters arg;
  arg.CallNumber=MSLEEP;
  arg.p0=(unsigned char *)time;
  CallSWI(0,&arg);
}

void usleep(int time)
{
  Parameters arg;
  arg.CallNumber=USLEEP;
  arg.p0=(unsigned char *)time;
  CallSWI(0,&arg);
}

void ligaled(int valor)
{
  Parameters arg;
  arg.CallNumber=LIGALED;
  arg.p0=(unsigned char *)valor;
  CallSWI(0,&arg);
}

void start(int scheduler)
{
  Parameters arg;
  arg.CallNumber=START;
  arg.p0=(unsigned char *)scheduler;
  CallSWI(0,&arg);
}

void taskjoin(int identificador)
{
  Parameters arg;
  arg.CallNumber=TASKJOIN;
  arg.p0=(unsigned char *)identificador;
  CallSWI(0,&arg);
}

void setmyname(const char *name)
{
  Parameters arg;
  arg.CallNumber=SETMYNAME;
  arg.p0=(unsigned char *)name;
  CallSWI(0,&arg);
}

void nkprint(char *fmt,void *number)
{
  Parameters arg;
  arg.CallNumber=NKPRINT;
  arg.p0=(unsigned char *)fmt;
  arg.p1=(void *)number;
  CallSWI(0,&arg);
}

void getmynumber(int *number)
{
  Parameters arg;
  arg.CallNumber=GETMYNUMBER;
  arg.p0=(unsigned char *)number;
  CallSWI(0,&arg);
}

void nkread(char *tipo,void *value)
{
 Parameters arg;
  arg.CallNumber=NKREAD;  
  arg.p0=(char *)tipo;// o tipo do dado é informado em arg.p0 28/02/15
  arg.p1=(void *)value;// endereço da variavel que recebe o dado solicitado será armazenado em arg.p1
  CallSWI(0,&arg);
}
