#include "bcp.h"
#include "../../Kernel/kernel.h"

/*<Descricao>
 * Elimina processo da fila de processamento.
 *</Descricao>
 */
void BCP_KILL (Bcp **BCPI, Bcp **BCPF, Bcp *BCP)
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
int BCP_ADD (Bcp **inicio, Bcp **fim, Bcp *bcp)
{
	Bcp *novo;
	novo = bcp;
	
	if (novo)
	{
		BCP_ADD_FIFO(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO(Bcp **inicio, Bcp **fim, Bcp **novo)
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
		Bcp *old;

		old = *fim;

		(*fim)->right =  *novo;
		*fim	      =  *novo;
		(*fim)->right =  NULL;
		(*novo)->left =  old;
	}

	return 1;
}
