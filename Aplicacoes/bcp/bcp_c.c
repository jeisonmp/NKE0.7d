#include "bcp_c.h"
#include "../../Kernel/kernel.h"

/*<Descricao>
 * Elimina processo da fila de processamento.
 *</Descricao>
 */
void BCP_KILL_c (Bcp_c **BCPI, Bcp_c **BCPF, Bcp_c *BCP)
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
		Bcp_c *ant = NULL;
		Bcp_c *pos = NULL;

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
int BCP_ADD_c (Bcp_c **inicio, Bcp_c **fim, Bcp_c *bcp)
{
	Bcp_c *novo;
	novo = bcp;
	
	if (novo)
	{
		BCP_ADD_FIFO_c(inicio, fim, &novo);
	}

	return 1;
}

/*
 * Fila
 */
int BCP_ADD_FIFO_c (Bcp_c **inicio, Bcp_c **fim, Bcp_c **novo)
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
		Bcp_c *old;

		old = *fim;

		(*fim)->right =  *novo;
		*fim	      =  *novo;
		(*fim)->right =  NULL;
		(*novo)->left =  old;
	}

	return 1;
}
