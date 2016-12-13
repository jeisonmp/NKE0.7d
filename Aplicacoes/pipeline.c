
#include "../Kernel/kernel.h"

sem_t s0,s1,s2;
int num=0;

void task0()
{
    int i;
    for(i=0;i<10;i++)
    {
       semwait(&s0);
       num++;
       nkprint("num = %d\n",num);
       sempost(&s1);
    }
    taskexit();
}

void task1()
{
    int i;
    for(i=0;i<10;i++)
    {
       semwait(&s1);
       num++;
       nkprint("num = %d\n",num);
       sempost(&s2);
    }
    taskexit();
}

void task2()
{
    int i;
    for(i=0;i<10;i++)
    {
       semwait(&s2);
       num++;
       nkprint("num = %d\n",num);
       sempost(&s0);
    }
    taskexit();
}

int main(int argc, char *argv[])
{
  int t2,t3,t4;
  seminit(&s0, 1);
  seminit(&s1, 0);
  seminit(&s2, 0);
  taskcreate(&t2,task0);
  taskcreate(&t3,task1);
  taskcreate(&t4,task2);
  start(RR);
  return 0;
}