/*
 * smalloc.c
 *
 *  Criado em: Nov 20, 2015
 *      Autor: Prass, Max J.
 *    Projeto: NanoKernel
 *  Descricao: Implementacao do malloc/free para Sistema de Tempo Real - Alocador Dinamico de Memoria Configuravel.
 *     Idioma: BR
 *
 */

#include "smalloc.h"

void sfree(void *addr)
{
	int posregion;
	int sizeregion;
	struct smalloc *head;
	struct smalloc *end;
	struct smalloc *auxaddr;    //auxiliar de addr
	int pos;
	int itg;
	
	TESTE_NUMFREES++;
	
	//Retem as informacoes da regiao
	int bitmask = 0;
	while (bitmask < NBITMASK)
	{
		posregion  = REGIONINI(bitmask);                 //posicao inicial (vetor de memoria) do bloco (small)
		sizeregion = REGIONEND(bitmask) - posregion;     //tamanho em inteiros + head (1 endereco)

		head = (struct smalloc *)&MEMORY[posregion];     //posicao inicial (memoria) do bloco (small)
		end = head + sizeregion;

		if ((int*)addr >= (int*)head && (int*)addr <= (int*)end)
		{
			break;
		}

		bitmask++;
	}

	CLEANBIT(TB1,bitmask);

	//Pega o endereco que se deseja liberar para depois pegar a posicao na regiao
	auxaddr = (struct smalloc *)addr;

	//Verifica a posicao na regiao, contando do inicio e realiza a limpeza de memoria
	pos = (uint32_t)(auxaddr - head);
	itg = 0;
	while (itg < TB2[bitmask])
	{
		MEMORY[posregion + pos + itg] = 0xffffffff;
		itg++;
	}

	//Nextfree de endereco que vai liberar aponta para proximo livre, apontado pelo Nextfree de cabecalho.
	auxaddr->nextfree = head->nextfree;
	//Nextfree de cabecalho aponta para endereco liberado.
	head->nextfree = (struct smalloc *)addr;
}

int* smalloc(int _sizebytes)
{
	int sizeints = (_sizebytes/4);
	int *addr =  NULL;
	uint8_t bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	uint8_t regionnumber = 0;
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	uint8_t succss = 0;
	
	uint8_t TESTE_FRAGMENTACAO = 0;
	uint8_t TESTE_REG1_DISPONIVEL = 0;
	uint8_t TESTE_REG2_DISPONIVEL = 0;
	uint8_t TESTE_REG3_DISPONIVEL = 0;
	/*
	* PROCEDIMENTO DE ALOCACAO
	*/
	
	//Retem as informacoes da regiao
	while (bitmask < NBITMASK)
	{
		// Testa se a regiao nao esta marcada como cheia
		if (!TESTBIT(TB1,bitmask))
		{
			// TESTE //NAO FAZ PARTE DO ALGORITMO
			TESTE_FRAGMENTACAO = 1;
			
			posregion  = REGIONINI(bitmask);                 //posicao inicial (vetor de memoria) do bloco
			sizeregion = REGIONEND(bitmask) - posregion;     //tamanho em inteiros + head (1 endereco)

			head = (struct smalloc *)&MEMORY[posregion];     //posicao inicial (memoria) do bloco
			end = head + sizeregion;
			
			// se cabe no bloco disponivel alloca, senao procura proximo bloco
			if (sizeints <= TB2[bitmask])
			{	
				succss = 1;
				break;
			}
		}

		bitmask++;
	}
	
	//TESTE nao faz parte do algoritmo
	regionnumber = bitmask;
	if (bitmask < 2 )
		regionnumber = 1;
	else if (bitmask > 1 && bitmask < 3)
		regionnumber = 2;
	else regionnumber = 3;

	if (!TESTBIT(TB1, 0) && !TESTBIT(TB1, 1))
		TESTE_REG1_DISPONIVEL = 1;
	if (!TESTBIT(TB1, 2))
		TESTE_REG2_DISPONIVEL = 1;
	if (!TESTBIT(TB1, 3))
		TESTE_REG3_DISPONIVEL = 1;
	
	if (succss)
	{
		//TESTE DE FRAGMENTACAO INTERNA
		//tamanho de inteiros 3
		if (sizeints == 3)
		{
			//REGIOES DE BLOCOS TAMANHOS 4
			if (bitmask > 1 && bitmask < 3)
			{
				TESTE_REG2_FRAGINT += 1;
			}
			//REGIOES DE BLOCOS TAMANHOS 5
			else if (bitmask > 2)
			{
				TESTE_REG3_FRAGINT += 2;
			}
		}
		else if (sizeints == 4)
		{
			//REGIOES DE BLOCOS TAMANHOS 4
			if (bitmask > 2)
			{
				TESTE_REG3_FRAGINT += 1;
			}
		}
				
		if (!head->nextfree) //Se igual a 0 (vazia)
		{
			//sys_nkprint(" 1.0.0");
			addr = (int*)(head) + 1; // Pula o endereco do cabecalho (Fragmentacao do cabecalho do bloco)

			if (((struct smalloc*)addr + sizeints) >= end) // Se ocupou todo o bloco
			{
				//sys_nkprint(" 1.1.0");
				head->nextfree = end;
				SETBIT(TB1,bitmask);            // Marca a regiao 'TB1' como cheia
			}
			else
			{
				//sys_nkprint(" 1.2.0");
				head->nextfree = (struct smalloc *)(addr + TB2[bitmask]);
			}
		}
		else
		{
			//sys_nkprint(" 2.0.0");
			addr = (int*)head->nextfree;

			if (((struct smalloc *)(addr))->nextfree != NULL &&
				((struct smalloc *)(addr))->nextfree != end) //se o proximo do proximo nao for o final
			{
				//sys_nkprint(" 2.1.0");
				head->nextfree = (((struct smalloc *)(addr))->nextfree);
			}
			else if (((struct smalloc *)(addr))->nextfree != NULL &&
				     ((struct smalloc *)(addr))->nextfree == end)
			{
				//sys_nkprint(" 2.2.0");
				head->nextfree = end;
				SETBIT(TB1,bitmask); // Marca a regiao como cheia
			}
			else
			{
				//sys_nkprint(" 2.3.0");
				//Se sobrou espaco menor ou igual ao tamanho do bloco da regiao entao marca como cheia.
				if ((((int)end - (int)(head->nextfree))/4 + 1) - TB2[bitmask] < TB2[bitmask])
				{
					//sys_nkprint(" 2.3.1");
					head->nextfree = end;
					SETBIT(TB1,bitmask); // Marca a regiao como cheia
				}
				else
				{
					//sys_nkprint(" 2.3.2");
					head->nextfree = head->nextfree + TB2[bitmask];
					
					if (head->nextfree > (struct smalloc *)&MEMORY[SIZEMEM])
					{
						//sys_nkprint(" REGION %d FULL");
						
						head->nextfree = end;
						SETBIT(TB1,bitmask); // Marca a regiao como cheia
					}
				}
			}
		}
		
		TESTE_NUMALLOCS++;
	
		sys_nkprint("\n--------------------------");
		sys_nkprint("\n-------MALLOC (OK)-------");
		sys_nkprint("\n--REG: %d, ", regionnumber);
		sys_nkprint("SIZE: %d----", sizeints);
		if (TESTE_REG1_DISPONIVEL)
			sys_nkprint("\nREG1 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG1 DISPONIVEL   : NAO ");
		if (TESTE_REG2_DISPONIVEL)
			sys_nkprint("\nREG2 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG2 DISPONIVEL   : NAO ");
		if (TESTE_REG3_DISPONIVEL)
			sys_nkprint("\nREG3 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG3 DISPONIVEL   : NAO ");
		sys_nkprint("\n-----------------------");
		sys_nkprint("\nNUM ALLOCS        : %d ", TESTE_NUMALLOCS);
		sys_nkprint("\nNUM FREES         : %d ", TESTE_NUMFREES);
		sys_nkprint("\nFALHAS MALLOC     : %d ", TESTE_NUMALLOCFAIL);
		sys_nkprint("\nFALHAS MEM FULL   : %d ", TESTE_NUMMEMFULL);
		sys_nkprint("\nFRAGMENT REG1 INT : %d ", TESTE_REG1_FRAGINT);
		sys_nkprint("\nFRAGMENT REG2 INT : %d ", TESTE_REG2_FRAGINT);
		sys_nkprint("\nFRAGMENT REG3 INT : %d ", TESTE_REG3_FRAGINT);
		sys_nkprint("\nFRAGMENT REG1 EXT : %d ", TESTE_REG1_FRAGEXT);
		sys_nkprint("\nFRAGMENT REG2 EXT : %d ", TESTE_REG2_FRAGEXT);
		sys_nkprint("\nFRAGMENT REG3 EXT : %d ", TESTE_REG3_FRAGEXT);
		
		return addr;
	}
	else
	{		
		TESTE_NUMALLOCFAIL++;
		
		//FRAGMENTACAO EXTERNA
		if (sizeints == 3)
		{
			if (TESTE_REG1_DISPONIVEL || TESTE_REG2_DISPONIVEL || TESTE_REG3_DISPONIVEL)
				TESTE_NUMALLOCFAIL++;
			else TESTE_NUMMEMFULL++;
		}
		if (sizeints == 4)
		{
			if (TESTE_REG1_DISPONIVEL)
				TESTE_REG2_FRAGEXT++;
			else if (TESTE_REG2_DISPONIVEL || TESTE_REG3_DISPONIVEL)
				TESTE_NUMALLOCFAIL++;
			else TESTE_NUMMEMFULL++;
		}
		else if (sizeints == 5)
		{
			if (TESTE_REG1_DISPONIVEL || TESTE_REG2_DISPONIVEL)
				TESTE_REG3_FRAGEXT++;
			else if (TESTE_REG3_DISPONIVEL)
				TESTE_NUMALLOCFAIL++;
			else TESTE_NUMMEMFULL++;
		}
				
		sys_nkprint("\n------------------------");
		sys_nkprint("\n-----MALLOC (ERROR)-----");
		sys_nkprint("\n--REG: %d, ", regionnumber);
		sys_nkprint("SIZE: %d----", sizeints);
		if (TESTE_REG1_DISPONIVEL)
			sys_nkprint("\nREG1 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG1 DISPONIVEL   : NAO ");
		if (TESTE_REG2_DISPONIVEL)
			sys_nkprint("\nREG2 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG2 DISPONIVEL   : NAO ");
		if (TESTE_REG3_DISPONIVEL)
			sys_nkprint("\nREG3 DISPONIVEL   : SIM ");
		else sys_nkprint("\nREG3 DISPONIVEL   : NAO ");
		sys_nkprint("\n-----------------------");
		sys_nkprint("\nNUM ALLOCS        : %d ", TESTE_NUMALLOCS);
		sys_nkprint("\nNUM FREES         : %d ", TESTE_NUMFREES);
		sys_nkprint("\nFALHAS MALLOC     : %d ", TESTE_NUMALLOCFAIL);
		sys_nkprint("\nFALHAS MEM FULL   : %d ", TESTE_NUMMEMFULL);
		sys_nkprint("\nFRAGMENT REG1 INT : %d ", TESTE_REG1_FRAGINT);
		sys_nkprint("\nFRAGMENT REG2 INT : %d ", TESTE_REG2_FRAGINT);
		sys_nkprint("\nFRAGMENT REG3 INT : %d ", TESTE_REG3_FRAGINT);
		sys_nkprint("\nFRAGMENT REG1 EXT : %d ", TESTE_REG1_FRAGEXT);
		sys_nkprint("\nFRAGMENT REG2 EXT : %d ", TESTE_REG2_FRAGEXT);
		sys_nkprint("\nFRAGMENT REG3 EXT : %d ", TESTE_REG3_FRAGEXT);
		
		
		//FRAGMENTACAO INTERNA
		TESTE_REG1_FRAGINT = 0;
		TESTE_REG2_FRAGINT = 0;
		TESTE_REG3_FRAGINT = 0;
	}

	return (int*)SMALLOCNULL;
}