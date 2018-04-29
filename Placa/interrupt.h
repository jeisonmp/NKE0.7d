#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <arch/nxp/lpc23xx.h>
#include "../Kernel/kernel.h"

#define habilitaIRQ() asm("msr cpsr_c, #0x5f\n");
#define desabilitaIRQ() asm("msr cpsr_c,#0xdf\n");

void IRQTimer0(void);

#endif