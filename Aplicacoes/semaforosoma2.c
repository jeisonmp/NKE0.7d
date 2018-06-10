
#include "../Kernel/kernel.h"

sem_t s0;
int num=0;

void soma_20000()
{
    int i;
    for(i=0;i<5;i++)
    {
       semwait(&s0);
       num += 2;
       nkprint("num= %d ola \n",num);
       sempost(&s0);
    }
    taskexit();
}

void soma_50000()
{
    int i;
    for(i=0;i<5;i++)
    {
       semwait(&s0);
       num += 5;
       nkprint("num= %d mundo\n",num);
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
