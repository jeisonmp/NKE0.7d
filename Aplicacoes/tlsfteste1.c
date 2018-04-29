
/// Referencias:
/// https://www.embarcados.com.br/tlsf/
/// http://www.gii.upv.es/tlsf/files/papers/mmasmano_phdthesis.pdf
///

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bcp/bcp.h"
#include "tlsf/tlsf.h"

static uint8_t heap[8192] = { 0 };

#define POOL_SIZE 1024 * 1024
// Pool size is in bytes.
char pool[POOL_SIZE];

void* TESTE1()
{
	int *ptr[100];
	int free_mem;

	free_mem = init_memory_pool(POOL_SIZE, pool);

	Bcp *BCPI = NULL;
	Bcp *BCPF = NULL;
	Bcp *novo = NULL;
	Bcp *aux;

	unsigned long int i = 100000000;
	int tentativas = 0;

	while (1)
	{
		int cont = 0;
		for (cont; cont < 100; cont++)
		{
			novo = (Bcp*)tlsf_malloc(sizeof(Bcp)); //novo = (Bcp *)smalloc(sizeof(Bcp));0x00cf9164 0x00cf9170
			printf(" val %d ", i);

			if (novo != (Bcp*)0x0)
			{
				novo->valor = i;
				BCP_ADD(&BCPI, &BCPF, novo);
				PRINTMEMORY();
			}
			else
			{
				printf("\n ******* P1 FALHOU tantaviva : %d *******\n", tentativas);
				break;
			}

			i++;
		}

		printf("\n ******* P1 LIMPANDO TODA MEMORIA... *******\n", 0);
		aux = BCPI;
		while (aux != NULL)
		{
			printf("\n P1 Liberou val %d ", i);
			printf("end %d...\n", novo);

			BCP_KILL(&BCPI, &BCPF, aux);
			tlsf_free(aux);
			aux = BCPI;
		}

		i++;
		if (i > 199999999) i = 0;
	}

	printf("\n***** FINAL DE P1 *****\n\n", 0);

	destroy_memory_pool(pool);

	return NULL;
}

int main()
{
	if (!TDD())
	{
		printf("FALHA NA COMPILAÇÃO - TESTES FALHARAM");
		system("pause");
		return 0;
	}

	TESTE1();

	system("pause");
	return 0;
}