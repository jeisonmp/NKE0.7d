
#include "../Kernel/kernel.h"

int buffer=0;

void task0()
{
  int i;
  for(i=0;i<10;i++)
  {
    sleep(2);
    buffer++;
  }
  taskexit();
}

void task1()
{ 
  int i;
  for(i=0;i<10;i++)
  {
    sleep(2);
    nkprint("buffer = %d\n",buffer);
  }
  taskexit();
}

int main (int argc, char *argv[])
{
  int t0,t1;
  taskcreate(&t0,task0);
  taskcreate(&t1,task1);
  start(RR);
  return 0;
}