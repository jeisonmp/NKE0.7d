#include "../Kernel/kernel.h"

typedef struct bcp Bcp;

struct bcp
{
	int valor;

	Bcp *right;
	Bcp *left;
};

void BCP_KILL (Bcp **BCPI, Bcp **BCPF, Bcp *BCP)
{
	nkprint("\nfree %d", global_cont_free);
	if (BCP == *BCPI)
	{
		nkprint(" (1)", 0);
		if (BCP == *BCPF)
		{
			nkprint(" (1.1)", 0);
			*BCPI = NULL;
			*BCPF = NULL;
		}
		else
		{
			nkprint(" (1.2)", 0);
			*BCPI = BCP->right;
			(*BCPI)->left = NULL;
		}
	}
	else if (BCP == *BCPF)
	{
		nkprint(" (2)", 0);
		*BCPF = BCP->left;
		(*BCPF)->right = NULL;
	}
	else
	{
		nkprint(" (3)", 0);
		Bcp *ant = NULL;
		Bcp *pos = NULL;

		if (BCP->left != NULL)
		{
			ant = BCP->left;
		}
		if (BCP->right != NULL)
		{
			pos = BCP->right;
		}

		if (ant != NULL && pos != NULL)
		{
			ant->right = pos;
			pos->left = ant;
		}
		else if (ant != NULL)
		{
			ant->right = *BCPF;
		}
		else
		{
			pos->left = *BCPI;
		}
	}
}

/*
 * Adiciona processo.
 */
int BCP_ADD (Bcp **inicio, Bcp **fim, Bcp *bcp)
{
	Bcp *novo;
	novo = bcp;
	
	if (novo)
	{
		BCP_ADD_FIFO(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO(Bcp **inicio, Bcp **fim, Bcp **novo)
{
	if (*inicio == NULL)
	{
		*inicio		    = *novo;
		*fim     	    = *novo;
		(*novo)->right	= NULL;
		(*novo)->left   = NULL;
	}
	else
	{
		Bcp *old;

		old = *fim;

		(*fim)->right =  *novo;
		*fim	      =  *novo;
		(*fim)->right =  NULL;
		(*novo)->left =  old;
	}

	return 1;
}

void TESTE1()
{	
	nkprint("**** INICIANDO TESTE NKMALLOC ****\n", 0);
	
	Bcp *BCPI = NULL;
	Bcp *BCPF = NULL;
	Bcp *novo;
	Bcp *aux;
	
	int i = 0;
	for (i; i < 10; i++)
	{
		nkprint("\n Alocando dado %d...\n", i);
		
		nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)malloc_adapt(sizeof(Bcp));
		
		nkprint("Final :",0);
		nkprint("end=%d ", &novo);
		nkprint("add=%d ", novo);
		
		novo->valor = i;
		
		BCP_ADD(&BCPI, &BCPF, novo);
	}
	
	nkprint("\n\nTerminou de alocar os 10 primeiros...\n", 0);
	
	nkprint("INICIO ****VERIFICACAO DOS DADOS****\n", 0);
	
	aux = BCPI;
	while (aux != NULL)
	{
		nkprint("left: %d - ", aux->left);
		nkprint("end: %d - ", aux);
		nkprint("right: %d\n", aux->right);
		
		aux = aux->right;
	}
	
	nkprint("\n***Testando liberacao de 2 recursos*****\n\n", 0);
	
	aux = BCPI;
	while (aux != NULL)
	{
		if (aux->valor == 2)
		{
			nkprint("liberando dado %d...", aux->valor);
			BCP_KILL(&BCPI, &BCPF, aux);
			
			nkfree(aux); //free_adapt(aux);
			
			nkprint("OK\n", 0);
			
			aux = BCPI;
		}
		else if (aux->valor == 4)
		{
			nkprint("liberando dado %d...", aux->valor);
			BCP_KILL(&BCPI, &BCPF, aux);
			nkfree(aux); //free_adapt(aux);
			nkprint("OK\n", 0);
			
			aux = BCPI;
		}

		aux = aux->right;
	}
	
	nkprint("\n\n****VERIFICACAO DOS DADOS****\n", 0);
	
	aux = BCPI;
	while (aux != NULL)
	{
		nkprint("left: %d - ", aux->left);
		nkprint("ptr: %d - ", aux);
		nkprint("end: %d - ", &aux);
		nkprint("right: %d\n", aux->right);
		
		aux = aux->right;
	}
	
	nkprint("\n***** FIM *****\n\n", 0);
	
	taskexit();
}

int main(int argc, char *argv[])
{
	int t1;
	taskcreate(&t1, TESTE1);
	start(RR);
	return 0;
}