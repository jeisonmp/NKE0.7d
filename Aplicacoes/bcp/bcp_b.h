#ifndef BCP_H_b
#define BCP_H_b

#include <stdint.h>
typedef struct bcp_b Bcp_b;

struct bcp_b
{
	int valor;
	int valorB;

	Bcp_b *right;
	Bcp_b *left;
};

int  BCP_ADD_b      (Bcp_b **, Bcp_b **, Bcp_b *);
int  BCP_ADD_FIFO_b (Bcp_b **, Bcp_b **, Bcp_b **);
void BCP_KILL_b     (Bcp_b **, Bcp_b **, Bcp_b *);

#endif
