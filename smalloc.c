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

void k_heap_init() {}

#include "smalloc.h"

extern int fragInterna;
extern int fragExterna;

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
	int bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	int succss = 0;
	
	/*
	* PROCEDIMENTO DE ALOCACAO
	*/
	
	//int fragExtern = 0;
	//int region = -1;

	//Retem as informacoes da regiao
	while (bitmask < NBITMASK)
	{
		// Testa se a regiao nao esta marcada como cheia
		if (!TESTBIT(TB1,bitmask))
		{
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
			/// FRAGMENTACAO
			/*else {
				fragExtern = 1;
				region = bitmask+1;
			}*/
		}

		bitmask++;
	}

	if (succss)
	{
		///
		/// FRAGMENTACAO
		///
		/*
		if (sizeints == 3)
		{
			//if (bitmask > 1)
			{
				fragInterna += 2*4;
			}
		}
		else if (sizeints == 4)
		{
			//if (bitmask == 2) 
			{
				fragInterna += 1*4;
			}
		}
		*/
		
		//printf("\nhead %x ", head);
		//printf(" nextfree %x ", head->nextfree);
		//printf(" end %x ", end);

		if (!head->nextfree) //Se igual a 0 (vazia)
		{ 
			// Pula o endereco do cabecalho (Fragmentacao do cabecalho do bloco)
			addr = (int*)(head) + 1; 
			if (((struct smalloc*)addr + sizeints) >= end) // Se ocupou todo o bloco
			{
				//printf(" 1.1.0");
				head->nextfree = end; //printf("nextfree %x ", head->nextfree);
				SETBIT(TB1,bitmask); //printf(" REG %d FULL!", bitmask);
			}
			else
			{
				//printf(" 1.2.0");
				head->nextfree = (struct smalloc *)(addr + TB2[bitmask]); //printf("nextfree %x ", head->nextfree);
			}
		}
		else
		{
			
			//printf("2.0.0");
			addr = (int*)head->nextfree; //printf(" addr %x ", addr);
			
			if (((struct smalloc *)(addr))->nextfree != NULL &&
				((struct smalloc *)(addr))->nextfree != end) //se o proximo do proximo nao for o final
			{
				//printf(" 2.1.0");
				head->nextfree = (((struct smalloc *)(addr))->nextfree); //printf("nextfree %x ", head->nextfree);
			}
			else if (((struct smalloc *)(addr))->nextfree != NULL &&
				     ((struct smalloc *)(addr))->nextfree == end)
			{
				//printf(" 2.2.0 %d", bitmask);
				head->nextfree = end; //printf(" nextfree %x ", head->nextfree);
				SETBIT(TB1,bitmask); //printf(" REG %d FULL!", bitmask);
			}
			else
			{
				//printf(" 2.3.0");
				//Se sobrou espaco menor ou igual ao tamanho do bloco da regiao entao marca como cheia.
				//printf(" size: %d sizeblk: %d", (((int)end - (int)(head->nextfree)) / 4 + 1) - TB2[bitmask], TB2[bitmask]);
				if ((((int)end - (int)(head->nextfree))/4 + 1) - TB2[bitmask] < TB2[bitmask])
				{
					//printf(" 2.3.1");
					head->nextfree = end; //printf(" nextfree %x ", head->nextfree);
					SETBIT(TB1,bitmask); //printf(" REG %d FULL!", bitmask);
				}
				else
				{
					//printf(" 2.3.2");
					head->nextfree = head->nextfree + TB2[bitmask]; //printf(" nextfree %x ", head->nextfree);
					
					//printf(" &MEMORY[SIZEMEM]: %d", (struct smalloc *)&MEMORY[SIZEMEM]);
					if (head->nextfree >= (struct smalloc *)&MEMORY[SIZEMEM])
					{
						//printf(" 2.3.2.1");
						head->nextfree = end; //printf(" nextfree %x ", head->nextfree);
						SETBIT(TB1,bitmask); //printf(" REG %d FULL!", bitmask);
					}
				}
			}
		}

		//printf(" ( OK )");
		return addr;
	}
	else
	{
		/// FRAGMENTACAO
		/*
		if (fragExtern == 1) {
			if (sizeints == 4) {
				fragExterna += 1*4;
			}
			else if (sizeints == 5) {
				if (region == 1 || region == 2) {
					fragExterna += 2*4;
				}
				else if (region == 2) {
					fragExterna += 1*4;
				}
			}
		}
		*/
		return SMALLOCNULL;
	}
}
/*
void PRINTMEMORY()
{
	int *addr = NULL;
	int bitmask = 0;              // Variavel para iteracao/navegacao dos bits de mascaras das 'TBL1' e 'TBS1'
	int sizeregion;
	int posregion;
	struct smalloc *head = NULL;
	struct smalloc *end = NULL;
	int nav = 0;

	/// 'BIT' = Usada para setar em tempo de DEBUG a regiao que queira imprimir.
	//int BIT = 6;
	while (bitmask < NBITMASK)
	{
		//if (bitmask == region)
		{
			posregion = REGIONINI(bitmask);                        //posicao inicial (vetor de memoria) do bloco
			sizeregion = REGIONEND(bitmask) - posregion;    //tamanho em inteiros + head (1bit) + nextfree (1 bit) + end(1 bit)

			head = (struct smalloc *)&MEMORY[posregion];                        //posicao inicial (memoria) do bloco
			end = head + sizeregion;

			printf("\nREG: %4d ADRI: %8x ADRF: %4x SZEREG: %d\n", bitmask, head, end, sizeregion);

			for (nav = 0; nav <= sizeregion; nav++)
			{
				printf("\n%x : value: %x", &MEMORY[posregion + nav], (MEMORY[posregion + nav]));
			}
		}

		bitmask++;
	}
}
*/