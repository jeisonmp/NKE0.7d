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
	
	//Limpa a mascara
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
		
	TESTE_MEMDISP++;
}

int* smalloc(int _sizebytes)
{
	int sizeints = (_sizebytes/4);
	int *addr =  NULL;
	int bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	int regionnumber = 0;
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	int succss = 0;
	
	int teste_fragmentacao = 0;
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
			teste_fragmentacao = 1;
			
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
	if (bitmask < 4 )
		regionnumber = 3;
	else if (bitmask > 3 && bitmask < 6)
		regionnumber = 4;
	else regionnumber = 5;
	
	if (succss)
	{
		//TESTE DE FRAGMENTACAO INTERNA
		//tamanho de inteiros 3
		if (sizeints == 3)
		{
			//REGIOES DE BLOCOS TAMANHOS 4
			if (bitmask > 3 && bitmask < 6)
			{
				TESTE_FRAGINTB += 1;
			}
			//REGIOES DE BLOCOS TAMANHOS 5
			else if (bitmask > 5)
			{
				TESTE_FRAGINTC_A += 2;
				TESTE_FRAGINTC += 2;
			}
		}
		else if (sizeints == 4)
		{
			//REGIOES DE BLOCOS TAMANHOS 4
			if (bitmask > 5)
			{
				TESTE_FRAGINTC_B += 1;
				TESTE_FRAGINTC += 1;
			}
		}
	
		// sys_nkprint("\nMEMDISP %d ", TESTE_MEMDISP);
		// sys_nkprint("NumALOCS %d ", TESTE_NUMALLOCS);
		// sys_nkprint("NumFREES %d ", TESTE_NUMFREES);
		
		// sys_nkprint("NumFAIL %d ", TESTE_NUMALLOCFAIL);
		// sys_nkprint("NumFULL %d ", TESTE_NUMMEMFULL);
		
		// sys_nkprint("ExtB %d ", TESTE_FRAGEXTB);
		// sys_nkprint("IntB %d ", TESTE_FRAGINTB);
		// sys_nkprint("ExtC %d ", TESTE_FRAGEXTC);
		// sys_nkprint("IntC %d ", TESTE_FRAGINTC);
		
		// sys_nkprint("reg %d ", regionnumber);
		// sys_nkprint("next %d ", head->nextfree);
		
		if (!head->nextfree) //Se igual a 0 (vazia)
		{
			sys_nkprint(" 1.0.0");
			addr = (int*)(head) + 1; // Pula o endereco do cabecalho (Fragmentacao do cabecalho do bloco)

			if (((struct smalloc*)addr + sizeints) >= end) // Se ocupou todo o bloco
			{
				sys_nkprint(" 1.1.0");
				head->nextfree = end;
				SETBIT(TB1,bitmask);            // Marca a regiao 'TB1' como cheia
			}
			else
			{
				sys_nkprint(" 1.2.0");
				head->nextfree = (struct smalloc *)(addr + TB2[bitmask]);
			}
		}
		else
		{
			sys_nkprint(" 2.0.0");
			addr = (int*)head->nextfree;

			if (((struct smalloc *)(addr))->nextfree != NULL &&
				((struct smalloc *)(addr))->nextfree != end) //se o proximo do proximo nao for o final
			{
				sys_nkprint(" 2.1.0");
				head->nextfree = (((struct smalloc *)(addr))->nextfree);
			}
			else if (((struct smalloc *)(addr))->nextfree != NULL &&
				     ((struct smalloc *)(addr))->nextfree == end)
			{
				sys_nkprint(" 2.2.0");
				head->nextfree = end;
				SETBIT(TB1,bitmask); // Marca a regiao como cheia
			}
			else
			{
				sys_nkprint(" 2.3.0");
				//Se sobrou espaco menor ou igual ao tamanho do bloco da regiao entao marca como cheia.
				if ((((int)end - (int)(head->nextfree))/4 + 1) - TB2[bitmask] <= TB2[bitmask])
				{
					sys_nkprint(" 2.3.1");
					head->nextfree = end;
					SETBIT(TB1,bitmask); // Marca a regiao como cheia
				}
				else
				{
					sys_nkprint(" 2.3.2");
					head->nextfree = head->nextfree + TB2[bitmask];
					
					if (head->nextfree >= (struct smalloc *)&MEMORY[SIZEMEM])
					{
						sys_nkprint(" FULL");
						
						head->nextfree = end;
						SETBIT(TB1,bitmask); // Marca a regiao como cheia
					}
				}
			}
		}
		
		sys_nkprint(" ( OK )");
		TESTE_MEMDISP--;
		TESTE_NUMALLOCS++;
		return addr;
	}
	else
	{
		sys_nkprint(" (FAIL)");
		
		TESTE_NUMALLOCFAIL++;
		
		//FRAGMENTACAO EXTERNA
		if (teste_fragmentacao)
		{
			if (sizeints == 4)
			{
				TESTE_FRAGEXTB++;
			}
			else if (sizeints == 5)
			{
				TESTE_FRAGEXTC++;
			}
			else
			{
				TESTE_NUMMEMFULL++;
			}
		}
		else
		{
			TESTE_NUMMEMFULL++;
		}
		
		//FRAGMENTACAO INTERNA
		if (sizeints == 3)
		{
			TESTE_FRAGINTB = 0;
			
			TESTE_FRAGINTC -= TESTE_FRAGINTC_A;
			TESTE_FRAGINTC_A = 0;
		}
		else if (sizeints == 4)
		{
			TESTE_FRAGINTC -= TESTE_FRAGINTC_B;
			TESTE_FRAGINTC_B = 0;
		}
		
		return (int*)SMALLOCNULL;
	}
}


void PRINTMEMORY()
{
	int *addr =  NULL;
	int bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	int nav = 0;

	//Retem as informacoes da regiao
	//while (bitmask < NREGIONS)
	while (bitmask < 2)
	{
		posregion = REGIONINI(bitmask);                        //posicao inicial (vetor de memoria) do bloco
		sizeregion = REGIONEND(bitmask) - posregion;    //tamanho em inteiros + head (1bit) + nextfree (1 bit) + end(1 bit)

		head = (struct smalloc *)&MEMORY[posregion];                        //posicao inicial (memoria) do bloco
		end = head + sizeregion;

		sys_nkprint("\nREG: %d ADRI: %d ADRF: %x SZEREG: %d\n", bitmask, head, end, sizeregion);

		if (bitmask == 0 || bitmask == 1)
		{
			for (nav = 0; nav <= sizeregion; nav++)
			{
				sys_nkprint("%x : value: %x\n", &MEMORY[posregion + nav], (MEMORY[posregion + nav]));
			}
		}
		else if (bitmask == 1)
		{
			for (nav = 0; nav <= sizeregion; nav++)
			{
				sys_nkprint("%x : value: %x\n", &MEMORY[posregion + nav], (MEMORY[posregion + nav]));
			}
		}

		bitmask++;
	}
}