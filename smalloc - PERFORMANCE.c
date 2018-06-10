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

	//marca a regiao como "nao cheia"
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

void* smalloc(int _sizebytes)
{
	int sizeints = (_sizebytes/4);
	int *addr =  NULL;
	int bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	int succss = 0;

	//sys_nkprint("\nSIZE: %d ", sizeints);
	
	/*
	* PROCEDIMENTO DE ALOCACAO
	*/
	
	//Retem as informacoes da regiao
	while (bitmask < NBITMASK)
	{
		// Testa se a regiao nao esta marcada como cheia
		if (!TESTBIT(TB1,bitmask))
		{
			posregion  = REGIONINI(bitmask);                 //posicao inicial (vetor de memoria) da regiao
			sizeregion = REGIONEND(bitmask) - posregion;     //tamanho da regiao em inteiros

			head = (struct smalloc *)&MEMORY[posregion];     //endereco inicial (memoria) da regiao
			end = head + sizeregion;                          //endereco final (memoria) da regiao

			// se cabe no bloco disponivel alloca, senao procura proximo bloco
			if (sizeints <= TB2[bitmask])
			{	
				succss = 1;
				break;
			}
		}

		bitmask++;
	}

	//sys_nkprint("REGIAO LIVRE %d ", bitmask);
	
	if (succss)
	{
		//sys_nkprint("ENCONTRADA ", 0);

		if (!head->nextfree) //Se igual a 0 (vazia)
		{
			addr = (int*)(head) + 1; // Pula o endereco do cabecalho (Fragmentacao do cabecalho do bloco)

			if (((struct smalloc*)addr + sizeints) >= end) // Se ocupou todo o bloco
			{
				head->nextfree = end;
				SETBIT(TB1,bitmask);            // Marca a regiao 'TB1' como cheia
			}
			else
			{
				head->nextfree = (struct smalloc *)(addr + TB2[bitmask]);
			}
		}
		else
		{
			addr = (int*)head->nextfree;

			if (((struct smalloc *)(addr))->nextfree != NULL &&
				((struct smalloc *)(addr))->nextfree != end) //se o proximo do proximo nao for o final
			{
				head->nextfree = (((struct smalloc *)(addr))->nextfree);
			}
			else if (((struct smalloc *)(addr))->nextfree != NULL &&
				     ((struct smalloc *)(addr))->nextfree == end)
			{
				//sys_nkprint(" COMPLETADA ", 0);
				head->nextfree = end;
				SETBIT(TB1,bitmask); // Marca a regiao como cheia
			}
			else
			{
				//Se sobrou espaco menor ou igual ao tamanho do bloco da regiao entao marca como cheia.
				if ((((int)end - (int)(head->nextfree))/4 + 1) - TB2[bitmask] < TB2[bitmask])
				{
					//sys_nkprint(" COMPLETADA ", 0);

					head->nextfree = end;
					SETBIT(TB1,bitmask); // Marca a regiao como cheia
				}
				else
				{
					head->nextfree = head->nextfree + TB2[bitmask];
					
					if (head->nextfree > (struct smalloc *)&MEMORY[SIZEMEM])
					{
						//sys_nkprint(" COMPLETADA ", 0);
						head->nextfree = end;
						SETBIT(TB1,bitmask); // Marca a regiao como cheia
					}
				}
			}
		}
		
		return addr;
	}

	//sys_nkprint(" FALHOU ", 0);
	return (void*)SMALLOCNULL;
}