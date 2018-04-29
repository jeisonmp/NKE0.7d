#include "interrupt.h"

void IRQTimer0()
{
	desabilitaIRQ();
	VICVectAddr4 = (unsigned long)Interrupt;  /*Habilita Timer0 do timer 0*/
	VICIntSelect &= ~(0x10);
	VICIntEnable = (1 << 4);
	habilitaIRQ();
	while(1); //aguarda a interrupção do clock -- a interrupção acorda o main
}