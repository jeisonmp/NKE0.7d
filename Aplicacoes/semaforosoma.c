
#include "../Kernel/kernel.h"

sem_t s0;
int num;

void soma_20000()
{
    int i;
    num = 0;
    for(i=0;i<10000;i++)
    {
       semwait(&s0);
   //    nkprint("num= %d ola \n", &num);
       nkread("%d", &num);
       sempost(&s0);
    }
    taskexit();
}

void soma_50000()
{
    int i;
    for(i=0;i<10000;i++)
    {
       semwait(&s0);
       num += 5;
       nkprint("num= %d mundo\n", &num);
       sempost(&s0);
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  int t2,t3;
  seminit(&s0, 1);
  taskcreate(&t2,soma_20000);
  taskcreate(&t3,soma_50000);
  start(RR);
  return 0;
}
