
#include "../Kernel/kernel.h"
int t[MaxNumberTask-2];

void teste()
{
  int id;
  getmynumber(&id);
  nkprint("Eu Sou o %d\n",id);
  taskexit();
}

int main (int argc, char *argv[])
{
  int i;
   for(i=0;i<MaxNumberTask-2;i++)
     taskcreate(&t[i],teste);
  start(RR);
  return 0;
}




