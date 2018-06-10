#include "bcp.h"
#include "../../Kernel/kernel.h"

void BCP_KILL (struct bcp **BCPI, struct bcp **BCPF, struct bcp *BCP)
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
		Bcp *ant = NULL;
		Bcp *pos = NULL;

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
int BCP_ADD (struct bcp **inicio, struct bcp **fim, struct bcp *BCP)
{
	Bcp *novo;
	novo = BCP;
	
	if (novo)
	{
		BCP_ADD_FIFO(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO(struct bcp **inicio, struct bcp **fim, struct bcp **novo)
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