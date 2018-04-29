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

sem_t s0;

Bcp *BCPI1 = NULL;
Bcp *BCPF1 = NULL;

Bcp_b *BCPI2 = NULL;
Bcp_b *BCPF2 = NULL;

Bcp_c *BCPI3 = NULL;
Bcp_c *BCPF3 = NULL;

void EMPTY_MEM()
{
	semwait(&s0);
			
	Bcp *aux1;
	aux1 = BCPI1;
	while (aux1 != NULL)
	{
		BCP_KILL(&BCPI1, &BCPF1, aux1);
		nkfree(aux1);
		aux1 = BCPI1;
	}
		
	Bcp_b *aux2;
	aux2 = BCPI2;
	while (aux2 != NULL)
	{	
		BCP_KILL_b(&BCPI2, &BCPF2, aux2);
		nkfree(aux2);
		aux2 = BCPI2;
	}
	
	Bcp_c *aux3;
	aux3 = BCPI3;
	while (aux3 != NULL)
	{
		BCP_KILL_c(&BCPI3, &BCPF3, aux3);
		nkfree(aux3);
		aux3 = BCPI3;
	}

	sempost(&s0);
}
void PROCESSO1()
{	
	Bcp *novo = NULL;
	
	uint8_t i = 0;
	
	while(1)
	{
		nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		
		if (novo != (struct smalloc*)0x0)
		{
			nkprint(" val %d ", i);
			novo->valor = i;
			BCP_ADD(&BCPI1, &BCPF1, novo);
		}
		else
		{
			nkprint("\n ******* P1 FALHOU ******* ", 0);
			nkprint("\n ******* P1 LIMPANDO MEMORIA *******", 0);

			EMPTY_MEM();

			nkprint("\n ******* P1 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	taskexit();
}

void PROCESSO2()
{	
	Bcp_b *novo = NULL;
	
	uint8_t i = 0;
	
	while(1)
	{
		nkmalloc(&novo, sizeof(Bcp_b)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		
		if (novo != (struct smalloc*)0x0)
		{
			nkprint(" val %d ", i);
			novo->valor = i;
			BCP_ADD_b(&BCPI2, &BCPF2, novo);
			msleep(50);
		}
		else
		{
			nkprint("\n ******* P2 FALHOU ******* ", 0);
			nkprint("\n ******* P2 LIMPANDO MEMORIA *******", 0);
			
			EMPTY_MEM();

			nkprint("\n ******* P2 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	taskexit();
}

void PROCESSO3()
{
	Bcp_c *novo = NULL;
	
	uint8_t i = 0;
	
	while(1)
	{	
		nkmalloc(&novo, sizeof(Bcp_c)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		
		if (novo != (struct smalloc*)0x0)
		{
			nkprint(" val %d ", i);
			novo->valor = i;
			BCP_ADD_c(&BCPI3, &BCPF3, novo);
		}
		else
		{			
			nkprint("\n ******* P3 FALHOU ******* ", 0);
			nkprint("\n ******* P3 LIMPANDO MEMORIA *******", 0);
			
			EMPTY_MEM();

			nkprint("\n ******* P3 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	taskexit();
}

int main(int argc, char *argv[])
{
  	seminit(&s0, 1);
	int t1, t2, t3;
	taskcreate(&t2, PROCESSO2);   //size 4
	taskcreate(&t3, PROCESSO3);   //size 5
	taskcreate(&t1, PROCESSO1);   //size 3
	start(RR);
	return 0;
}
