#include "../Kernel/kernel.h"
#include "bcp/bcp.h"

int tmenor = 999999999;
int tmaior = 0;
int tmedia;
int tcont = 0;
int tsoma = 0;
int ttempo = 0;

int GLOBAL_CONT;

void TESTE1()
{	
	nkprint("**** INICIANDO TESTE NKMALLOC ****\n", 0);
	
	Bcp *BCPI = NULL;
	Bcp *BCPF = NULL;
	Bcp *novo;
	Bcp *aux;
	
	int i = 0;
	while(1)
	{
		i = 0;
		GLOBAL_CONT = 0;
		while(1)
		{
			nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)malloc_adapt(sizeof(Bcp));
						
			if (novo != NULL)
			{
				novo->valor = i;
				BCP_ADD(&BCPI, &BCPF, novo);
				nkprint(" VAL %d", i);
			}
			else
			{
				nkprint("\n--- LIMPEZA INICIADA POR PROCESSO %d ", 1);
				Bcp *aux;
				aux = BCPI;
				while (aux != NULL)
				{
					BCP_KILL(&BCPI, &BCPF, aux);
					nkfree(aux);
					aux = BCPI;
				}			
				nkprint("--- CONTAGEM: %d\n", GLOBAL_CONT);
				break;
			}
			i++;
			GLOBAL_CONT++;
		}
	}
		
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