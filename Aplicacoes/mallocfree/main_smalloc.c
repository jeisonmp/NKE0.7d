
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#include "../Kernel/kernel.h"
#include "adaptable/adaptalloc.h"
#include "bcp/bcp.h"

void* TESTE1()
{	
	Bcp *BCPI = NULL;
	Bcp *BCPF = NULL;
	Bcp *novo;
	Bcp *aux;
	
	int i = 1;
	for (i; i < 10; i++)
	{
		novo = (Bcp *)malloc_adapt(sizeof(Bcp));
		novo->valor = i;

		BCP_ADD(&BCPI, &BCPF, novo);
	}
	
	aux = BCPI;
	do
	{
		if (aux->valor == 2)
		{
			BCP_KILL(&BCPI, &BCPF, aux);
			aux = BCPI;
		}
		else if (aux->valor == 4)
		{
			BCP_KILL(&BCPI, &BCPF, aux);
			aux = BCPI;
		}

		aux = aux->right;

	} while (aux->right != NULL);
	
	for (i; i < 20; i++)
	{
		novo = (Bcp *)malloc_adapt(sizeof(Bcp));
		novo->valor = i;

		BCP_ADD(&BCPI, &BCPF, novo);
	}
	
	aux = BCPI;
	while (aux != NULL)
	{
		#ifdef __KERNEL_H
			nkprint("left: %d - ", aux->left);
			nkprint("end: %d - ", aux);
			nkprint("right: %d\n", aux->right);
		#else
			printf("left: %d - ", aux->left);
			printf("end: %d - ", aux);
			printf("right: %d\n", aux->right);
		#endif
		
		aux = aux->right;
	}
	
	aux = BCPI;
	while (aux != NULL)
	{
		BCP_KILL(&BCPI, &BCPF, aux);
		printf("endereco apagar: %d\n", aux);
		free_adapt(aux);
		aux = BCPI;
	}
	
	return NULL;
}

int main()
{
	TESTE1();

	return 0;
}