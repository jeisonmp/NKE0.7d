#include "bcp_b.h"
#include "../../Kernel/kernel.h"

void BCP_KILL_b (struct bcp_b **BCPI, struct bcp_b **BCPF, struct bcp_b *BCP)
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
int BCP_ADD_b (struct bcp_b **inicio, struct bcp_b **fim, struct bcp_b *BCP)
{
	Bcp_b *novo;
	novo = BCP;
	
	if (novo)
	{
		BCP_ADD_FIFO_b(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO_b(struct bcp_b **inicio, struct bcp_b **fim, struct bcp_b **novo)
{
	//nkprint(" novo %d", *novo);

	if (*inicio == NULL)
	{
		*inicio		    = *novo;
		*fim     	    = *novo;
		(*novo)->right	= NULL;
		(*novo)->left   = NULL;
	}
	else
	{
		(*novo)->left =  *fim; //nkprint(" novo->left %d", *fim);
		(*fim)->right =  *novo; //nkprint(" ultimo->right %d", *novo);
		*fim	      =  *novo;
		(*fim)->right =  NULL;
	}

	return 1;
}