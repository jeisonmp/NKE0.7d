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

void PROCESSO1()
{	
	Bcp *BCPI = NULL;
	Bcp *BCPF = NULL;
	Bcp *novo = NULL;
	Bcp *aux;
	
	unsigned long int i = 3000000;
	int tentativas = 0;
	
	while(1)
	{	
		nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		nkprint(" val %d ", i);
		
		if (novo != (struct smalloc*)0x0)
		{
			novo->valor = i;
			BCP_ADD(&BCPI, &BCPF, novo);
		}
		else
		{
			nkprint("\n ******* P1 FALHOU ******* ", 0);
			nkprint("\n ******* P1 LIMPANDO MEMORIA *******", 0);
			aux = BCPI;
			while (aux != NULL)
			{
				BCP_KILL(&BCPI, &BCPF, aux);
				nkfree(aux);
				aux = BCPI;
			}
			nkprint("\n ******* P1 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	nkprint("\n***** FINAL DE P1 *****", 0);
	
	taskexit();
}

void PROCESSO2()
{	
	Bcp_b *BCPI = NULL;
	Bcp_b *BCPF = NULL;
	Bcp_b *novo = NULL;
	Bcp_b *aux;
	
	unsigned long int i = 4000000;
	int tentativas = 0;
	
	while(1)
	{	
		nkmalloc(&novo, sizeof(Bcp_b)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		nkprint(" val %d ", i);
		
		if (novo != (struct smalloc*)0x0)
		{
			novo->valor = i;
			BCP_ADD_b(&BCPI, &BCPF, novo);
		}
		else
		{
			nkprint("\n ******* P2 FALHOU ******* ", 0);
			nkprint("\n ******* P2 LIMPANDO MEMORIA *******", 0);
			aux = BCPI;
			while (aux != NULL)
			{	
				BCP_KILL_b(&BCPI, &BCPF, aux);
				nkfree(aux);
				aux = BCPI;
			}
			nkprint("\n ******* P2 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	nkprint("\n***** FINAL DE P2 *****", 0);
	
	taskexit();
}

void PROCESSO3()
{	
	Bcp_c *BCPI = NULL;
	Bcp_c *BCPF = NULL;
	Bcp_c *novo = NULL;
	Bcp_c *aux;
	
	unsigned long int i = 5000000;
	int tentativas = 0;
	
	while(1)
	{	
		nkmalloc(&novo, sizeof(Bcp_c)); //novo = (Bcp *)smalloc(sizeof(Bcp));
		nkprint(" val %d ", i);
		
		if (novo != (struct smalloc*)0x0)
		{
			novo->valor = i;
			BCP_ADD_c(&BCPI, &BCPF, novo);
		}
		else
		{
			nkprint("\n ******* P3 FALHOU ******* ", 0);
			nkprint("\n ******* P3 LIMPANDO MEMORIA *******", 0);
			aux = BCPI;
			while (aux != NULL)
			{
				BCP_KILL_c(&BCPI, &BCPF, aux);
				nkfree(aux);
				aux = BCPI;
			}
			nkprint("\n ******* P3 LIMPEZA COMPLETA *******", 0);
		}
		
		i++;
	}
	
	nkprint("\n***** FINAL DE P3 *****", 0);
	
	taskexit();
}

int main(int argc, char *argv[])
{
	int t1, t2, t3;
	taskcreate(&t1, PROCESSO1);   //size 3
	taskcreate(&t2, PROCESSO2);   //size 4
	taskcreate(&t3, PROCESSO3);   //size 5
	start(RR);
	return 0;
}