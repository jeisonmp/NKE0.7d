#include "bcp_b.h"
#include "../../Kernel/kernel.h"

/*<Descricao>
 * Elimina processo da fila de processamento.
 *</Descricao>
 */
void BCP_KILL_b (Bcp_b **BCPI, Bcp_b **BCPF, Bcp_b *BCP)
{
	if (BCP == *BCPI)
	{
		if (BCP == *BCPF)
		{
			*BCPI = NULL;
			*BCPF = NULL;
		}
		else
		{
			*BCPI = BCP->right;
			(*BCPI)->left = NULL;
		}
	}
	else if (BCP == *BCPF)
	{
		*BCPF = BCP->left;
		(*BCPF)->right = NULL;
	}
	else
	{
		Bcp_b *ant = NULL;
		Bcp_b *pos = NULL;

		if (BCP->left != NULL)
		{
			ant = BCP->left;
		}
		if (BCP->right != NULL)
		{
			pos = BCP->right;
		}

		if (ant != NULL && pos != NULL)
		{
			ant->right = pos;
			pos->left = ant;
		}
		else if (ant != NULL)
		{
			ant->right = *BCPF;
		}
		else
		{
			pos->left = *BCPI;
		}
	}
}

/*
 * Adiciona processo.
 */
int BCP_ADD_b (Bcp_b **inicio, Bcp_b **fim, Bcp_b *bcp)
{
	Bcp_b *novo;
	novo = bcp;
	
	if (novo)
	{
		BCP_ADD_FIFO_b(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO_b (Bcp_b **inicio, Bcp_b **fim, Bcp_b **novo)
{
	if (*inicio == NULL)
	{
		*inicio		    = *novo;
		*fim     	    = *novo;
		(*novo)->right	= NULL;
		(*novo)->left   = NULL;
	}
	else
	{
		Bcp_b *old;

		old = *fim;

		(*fim)->right =  *novo;
		*fim	      =  *novo;
		(*fim)->right =  NULL;
		(*novo)->left =  old;
	}

	return 1;
}
