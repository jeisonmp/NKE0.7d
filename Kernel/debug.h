#ifndef __MONITOR_H
#define __MONITOR_H

#include <arch/nxp/lpc23xx.h>
#include "kernel.h"
#include "../Placa/uart.h"

/* Configuracoes especificas */
#define SYSCLK 12000000

void TTYout(void); //executado a cada vez que ocorre uma interupção do relógio
void TTYstart(void);
void TTYshared(int id);
void TTYplot(void);

#endif
