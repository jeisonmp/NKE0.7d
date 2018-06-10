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

int global_cont_alloc;
int global_cont_free = 0;
int tmenor = 999;
int tmaior;
int tmedia;
int tcont;
int tsoma;
int ttempo;

int global_bytes = 0;
int fragInterna = 0;
int fragExterna = 0;
int memoria = 0;

Bcp *BCPI1 = NULL;
Bcp *BCPF1 = NULL;

Bcp_b *BCPI2 = NULL;
Bcp_b *BCPF2 = NULL;

Bcp_c *BCPI3 = NULL;
Bcp_c *BCPF3 = NULL;

void EMPTY_MEM(int process)
{
	nkprint(" FREE_PROCESS:%d", process);
	nkprint(" bytes:%d", global_bytes);
	nkprint(" alloc_cont:%d", global_cont_alloc);
	nkprint(" free_cont:%d", global_cont_free);
	nkprint(" diff:%d", global_cont_alloc - global_cont_free);
	nkprint(" menor:%d", tmenor);
	nkprint(" maior:%d", tmaior);
	nkprint(" media:%d", tmedia);
	nkprint(" bytes:%d", memoria);
	nkprint(" fragInt:%d", fragInterna);
	nkprint(" fragExt:%d", fragExterna);
	
	memoria = 0;
	fragInterna = 0;
	fragExterna = 0;

	int cont_free = 0;
	int random = (rand() % 4) + 1;
	int pular = random;
	int cont_free_region;

	//if (process == 1)
	{
		Bcp *aux1;
		aux1 = BCPI1;
		while (aux1 != NULL)
		{
			if (pular < 1)
			{
				BCP_KILL(&BCPI1, &BCPF1, aux1);
				nkfree(aux1);
				aux1 = BCPI1;
				cont_free++;
				global_cont_free++;
				
				pular = random;
			}
			else {
				aux1 = aux1->right;
				pular--;
			}
		}
		aux1 = BCPI1;
		while (aux1 != NULL)
		{
			BCP_KILL(&BCPI1, &BCPF1, aux1);
			nkfree(aux1);
			cont_free++;
			global_cont_free++;

			aux1 = BCPI1;
		}

		cont_free_region = cont_free;
		nkprint(" free: p1:%d", cont_free_region);
	}

	//if (process == 2)
	{
		Bcp_b *aux2;
		aux2 = BCPI2;
		while (aux2 != NULL)
		{
			if (pular < 1)
			{
				BCP_KILL_b(&BCPI2, &BCPF2, aux2);
				nkfree(aux2);
				aux2 = BCPI2;
				cont_free++;
				global_cont_free++;
				
				pular = random;
			}
			else {
				aux2 = aux2->right;
				pular--;
			}
		}
		aux2 = BCPI2;
		while (aux2 != NULL)
		{
			BCP_KILL_b(&BCPI2, &BCPF2, aux2);
			nkfree(aux2);
			cont_free++;
			global_cont_free++;

			aux2 = BCPI2;
		}

		cont_free_region = cont_free - cont_free_region;
		nkprint(" p2:%d", cont_free_region);
	}

	//if (process == 3)
	{
		Bcp_c *aux3;
		aux3 = BCPI3;
		while (aux3 != NULL)
		{
			if (pular < 1)
			{
				BCP_KILL_c(&BCPI3, &BCPF3, aux3);
				nkfree(aux3);
				aux3 = BCPI3;
				cont_free++;
				global_cont_free++;
				
				pular = random;
			}
			else {
				aux3 = aux3->right;
				pular--;
			}
		}
		aux3 = BCPI3;
		while (aux3 != NULL)
		{
			BCP_KILL_c(&BCPI3, &BCPF3, aux3);
			nkfree(aux3);
			cont_free++;
			global_cont_free++;

			aux3 = BCPI3;
		}

		cont_free_region = cont_free - cont_free_region;
		nkprint(" p3:%d", cont_free_region);
	}
	
	nkprint("\n LIBERACAO ANT:%d", cont_free);
}

void PROCESSO1()
{	
	Bcp *novo = NULL;
	nkprint(" 1:%d ", 1);
	nkprint(" sizeof1:%d ", sizeof(int));
	nkmalloc(&novo, sizeof(Bcp)); //novo = (Bcp *)smalloc(sizeof(Bcp));
	
	if (novo != NULL) {
       	global_cont_alloc++;
		global_bytes += 3*4;
		memoria += 3*4;
		BCP_ADD(&BCPI1, &BCPF1, novo);
	}
	else {
		EMPTY_MEM(1);
	}
}

void PROCESSO2()
{	
	Bcp_b *novo = NULL;	
	nkprint(" 2:%d ", 2);
	nkprint(" sizeof2:%d ", sizeof(int));
	nkmalloc(&novo, sizeof(Bcp_b));
			
	if (novo != NULL) {
       	global_cont_alloc++;
		global_bytes += 4*4;
		memoria += 4*4;
		BCP_ADD_b(&BCPI2, &BCPF2, novo);
	}
	else {
		EMPTY_MEM(2);
	}
}

void PROCESSO3()
{
	Bcp_c *novo = NULL;	
	nkprint(" 3:%d ", 3);
	nkprint(" sizeof3:%d ", sizeof(int));
	nkmalloc(&novo, sizeof(Bcp_c));
			
	if (novo != NULL) {
       	global_cont_alloc++;
		global_bytes += 5*4;
		memoria += 5*4;
		BCP_ADD_c(&BCPI3, &BCPF3, novo);	
	}
	else {
		EMPTY_MEM(3);
	}
}

void MAIN()
{
	nkprint(">>>>> INICIALIZA >>>>>>", 0);

	int corte = 0;
	int random;

	while(1)
	{
		random = (rand() % 10) + 1;
		if (random < 4)
		{
			PROCESSO1();
		}
		else if (random > 3 && random < 7) {
			PROCESSO2();
		}
		else {
			PROCESSO3();
		}
	}
}

int main(int argc, char *argv[])
{
	/// TODO: Somente para o TLSF
	//k_heap_init();

	MAIN();

	return 0;
}
