
#include "../Kernel/kernel.h"

sem_t  s0,  s1 ;
int num = 0 ;

void task0(){

  int i ;

  for(i=0; i<10; i++) 
     {
       semwait (&s0) ;
       nkprint("Entrei no semaforo: task0\n",0);
       num += 50 ;
       nkprint("num = %d\n",num);
       sempost(&s1);
       nkprint("Sai do semaforo: task0\n",0);
     }
  taskexit();
}

void task1(){

  int i ;

  for (i=0; i<10; i++) 
    {
      semwait(&s1) ;
      nkprint("Entrei no semaforo: task1\n",0);
      num += 100 ;
      nkprint("num = %d\n",num);
      sempost(&s0) ;
      nkprint("Sai do semaforo: task1\n",0);
    }
  taskexit();
}

int main(int argc, char *argv[]){

  int t0,t1;
  seminit ( &s0, 1);
  seminit ( &s1, 0);

  taskcreate(&t0,task0);
  taskcreate(&t1,task1);
  start(RR);

  return 0;
}