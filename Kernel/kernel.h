#ifndef __KERNEL_H
#define __KERNEL_H

//#define SHARED_NUMBER 1

#ifdef SHARED_NUMBER
  extern int shared[SHARED_NUMBER-1];
#endif

#define PRINTK
//#define DEBUG_START
//#define DEBUG_SYSTEM
//#define DEBUG_PLOT

/*Define Clock de interrupção
 * Tabela de Interrupção :
 * 1       ClkT = 1   segundo
 * 0.1     ClkT = 100 milissegundos
 * 0.01    Clkt = 10  milissegundos
 * 0.001   Clkt = 1   milissegundo
 * 0.0001  ClkT = 100 microssegundos
 * 0.00001 ClkT = 10  microssegundos
*/
#define ClkT 1
//Define tamanho da pilha de cada tarefa
#define SizeTaskStack 24
//Tamanho da Fila
#define MaxNumberTask 10

typedef struct
{
  unsigned int *SP;
  int Tid;
  const char *name;
  unsigned short Prio;
  float Time;
  unsigned short Join;
  unsigned short State;
  void (*EP)();
  unsigned int Stack[SizeTaskStack];
}TaskDescriptor;

typedef struct 
{
    int queue[MaxNumberTask];
    unsigned short tail;
    unsigned short head;
}ReadyList;

typedef struct 
{
    short count;
    int sem_queue[MaxNumberTask], tail, header;
}sem_t;

typedef struct
{
  int CallNumber;
  unsigned int *p0;
  unsigned char *p1;
  unsigned char *p2;
  unsigned char *p3;
}Parameters;

typedef struct
{/*       Esta  struct  tem  por  objetivo  disponibilizar endereços para as tasks que chamam a chamada de sistema
   nkread.    Quando  uma  task  quer  ler dados  da serial, uma serial queue  é colocada na  fila para esta task, 
   respeitado a  ordem de  chegada. Quando chega dado na  uart0, a serial_queue da vez recebe o dado.
   	 Atualmente  quatro  tipos  de  dados  são suportados. Cada  chamada  de sistema nkread receberá  o valor  no 
   endereço da variável, por intermédio da serial queue, o tipo de dado que foi solicitado na mesma. Para aumentar
   os tipos de dados deve-se declarar nesta struct o tipo  de  dado  que  vai poder ser lido  na serial  e enviado
   para task. Também  deverá  ser  incluído em sys_nkread a verificação para o novo tipo  de dado possível  de ser
   tratado.  E por  fim, ser incluída  a função  de tratamento  em uart.c para que  se  este  formato  de dado for
   detectado, assim como é feito nos atuais. 
   	Dúvidas: miguel_cafruni@hotmail.com                                                                        */
  int tid;
  float *real;//28/02/15float
  char *string;
  int *inteiro;//07/03/2015
  char *caracter;
}SerialData;

extern unsigned int KernelStack[300];
extern unsigned short TaskRunning;
extern TaskDescriptor Descriptors[MaxNumberTask];
extern ReadyList ready_queue;
extern unsigned int NumberTaskAdd;
extern int SchedulerAlgorithm;
extern SerialData serial_queue[MaxNumberTask];

extern unsigned short serial_fila[MaxNumberTask];
extern unsigned short posicao_a_inserir;
extern unsigned short posicao_a_att;

void GoTask(unsigned int *SP,void (*PC)()) __attribute__ ((__long_call__));
void MoveToSP(unsigned int *SP) __attribute__ ((__long_call__));
void SystemContext(unsigned char *pilha,unsigned char *CallNumber,unsigned char *p0,unsigned char *p1,unsigned char *p2,unsigned char *p3,unsigned char *p4) __attribute__ ((interrupt("SWI")));
int CallSWI(int size,Parameters *arg) __attribute__ ((__long_call__));
int Interrupt(void) __attribute__ ((interrupt("IRQ"))) __attribute__ ((__long_call__));
void RestoreContext(unsigned int *pilha) __attribute__((__long_call__));
unsigned int SwiContext(void) __attribute__ ((__long_call__));
void FIQ_Routine(void) __attribute__ ((interrupt("FIQ")));
int main(int argc, char *argv[]);
void idletask();


//Includes necessario para o usuario do usuario
#include "../Placa/driver_includes.h"
#include "../Kernel/scheduler.h"
#include "../Kernel/usercall.h"


#endif
