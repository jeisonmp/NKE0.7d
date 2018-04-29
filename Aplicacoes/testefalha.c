#include "../Kernel/kernel.h"

int soma(int a, int b)
{
	return a + b;
}

int subtrai(int a, int b)
{
	return a - b;
}

void TESTE1()
{
	int valor1 = soma(3,4);
	sys_nkprint("valor de soma: %d\n", valor1);
	
	int valor2 = subtrai(2, 1);
	sys_nkprint("valor de subracao: %d\n", valor2);
	
	taskexit();
}

int main(int argc, char *argv[])
{
	int t1;
	taskcreate(&t1, TESTE1);
	start(RR);
	return 0;
}