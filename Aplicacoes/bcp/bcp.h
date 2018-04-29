#ifndef BCP_H
#define BCP_H

#include <stdint.h>
typedef struct bcp Bcp;

struct bcp
{
	int valor;

	Bcp *right;
	Bcp *left;
};

int  BCP_ADD      (Bcp **, Bcp **, Bcp *);
int  BCP_ADD_FIFO (Bcp **, Bcp **, Bcp **);
void BCP_KILL     (Bcp **, Bcp **, Bcp *);

#endif
