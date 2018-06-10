#include "../Kernel/kernel.h"
#include "bcp/bcp.h"
#include "bcp/bcp_b.h"
#include "bcp/bcp_c.h"

//
// TESTE DE FRAGMENTACAO EXTERNA E INTERNA
// com tentativas de alocação até estourar
// a memória ou ocorrer fragmentação externa
//

// CONFIGURACAO USADA
//    SZREG     32
//    SIZEMEM   4000
//    NBITMASK  8
//    TB2[NBITMASK] = { 3,3,3,3,4,4,5,5 };

#include <stdint.h>

int global_cont;
int global_cont_free = 0;
int tmenor = 999;
int tmaior;
int tmedia;
int tcont;
int tsoma;
int ttempo;

sem_t s0;

Bcp *BCPI1 = NULL;
Bcp *BCPF1 = NULL;

Bcp_b *BCPI2 = NULL;
Bcp_b *BCPF2 = NULL;

Bcp_c *BCPI3 = NULL;
Bcp_c *BCPF3 = NULL;

void EMPTY_MEM(int process)
{
	semwait(&s0);
	nkprint(" NOVO PROCESS %d -", process);
	nkprint(" global cont %d ", global_cont);
	nkprint(" global_free cont %d ", global_cont_free);
	nkprint(" tempo %d", ttempo);
	nkprint(" menor %d", tmenor);
	nkprint(" maior %d", tmaior);
	nkprint(" media %d", tmedia);
	
	int cont_free = 0;

	//if (process == 1)
	{		
		Bcp *aux1;
		aux1 = BCPI1;
		while (aux1 != NULL)
		{
			BCP_KILL(&BCPI1, &BCPF1, aux1);
			nkfree(aux1);
			aux1 = BCPI1;
			cont_free++;
		}
	}
	
	//if (process == 2)
	{
		Bcp_b *aux2;
		aux2 = BCPI2;
		while (aux2 != NULL)
		{	
			BCP_KILL_b(&BCPI2, &BCPF2, aux2);
			nkfree(aux2);
			aux2 = BCPI2;
			cont_free++;
		}
	}

	//if (process == 3)
	{
		Bcp_c *aux3;
		aux3 = BCPI3;
		while (aux3 != NULL)
		{
			BCP_KILL_c(&BCPI3, &BCPF3, aux3);
			nkfree(aux3);
			aux3 = BCPI3;
			cont_free++;
		}
	}
	
	nkprint("\n LIBERACAO ANTERIOR %d regs -", cont_free);
	sempost(&s0);
}

void PROCESSO1()
{	
	Bcp *novo = NULL;
	
	int i = 0;
	nkprint(" START PROCESS 1 ", 0);
	
	while(1)
	{
		int cont = 0;
		while(1)
		{		
			cont++;	
			nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)smalloc(sizeof(Bcp));
			
			if (novo != NULL)
			{
				novo->valor = i;
				semwait(&s0);
				BCP_ADD(&BCPI1, &BCPF1, novo);
				sempost(&s0);
			}
			else
			{
				EMPTY_MEM(1);
				break;
			}
		}
		i++;
	}
	
	taskexit();
}

void PROCESSO2()
{	
	Bcp_b *novo = NULL;
	
	int i = 0;
	nkprint(" START PROCESS 2 ", 0);
	
	while(1)
	{
		int cont = 0;
		while(1)
		{		
			cont++;
			nkmalloc(&novo, sizeof(Bcp_b)); //novo = (Bcp *)smalloc(sizeof(Bcp));
			
			if (novo != NULL)
			{
				novo->valor = i;
				semwait(&s0);
				BCP_ADD_b(&BCPI2, &BCPF2, novo);
				sempost(&s0);
			}
			else
			{
				EMPTY_MEM(2);
				break;
			}
		}
		i++;
	}
	
	taskexit();
}

void PROCESSO3()
{
	Bcp_c *novo = NULL;
	
	int i = 0;
	nkprint(" START PROCESS 3 ", 0);
	
	while(1)
	{
		int cont = 0;
		while(1)
		{		
			cont++;
			nkmalloc(&novo, sizeof(Bcp_c)); //novo = (Bcp *)smalloc(sizeof(Bcp));
			
			if (novo != NULL)
			{
				novo->valor = i;
				semwait(&s0);
				BCP_ADD_c(&BCPI3, &BCPF3, novo);	
				sempost(&s0);
			}
			else
			{
				EMPTY_MEM(3);
				break;
			}
		}
		i++;
	}
	
	taskexit();
}

int main(int argc, char *argv[])
{
	/// TODO: Somente para o TLSF
	k_heap_init();

  	seminit(&s0, 1);
	int t1, t2, t3;
	taskcreate(&t1, PROCESSO1);   //size 3
	taskcreate(&t2, PROCESSO2);   //size 4
	taskcreate(&t3, PROCESSO3);   //size 5
	start(RR);
	return 0;
}
