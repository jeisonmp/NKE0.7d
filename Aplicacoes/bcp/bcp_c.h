#ifndef BCP_H_c
#define BCP_H_c

#include <stdint.h>
typedef struct bcp_c Bcp_c;

struct bcp_c
{
	int valor;
	int valorB;
	int valorC;

	Bcp_c *right;
	Bcp_c *left;
};

int  BCP_ADD_c      (Bcp_c **, Bcp_c **, Bcp_c *);
int  BCP_ADD_FIFO_c (Bcp_c **, Bcp_c **, Bcp_c **);
void BCP_KILL_c     (Bcp_c **, Bcp_c **, Bcp_c *);

#endif
