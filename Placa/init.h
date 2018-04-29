#ifndef __INIT_H
#define __INIT_H

#include "interrupt.h"
#include <arch/nxp/lpc23xx.h>

void SaidaLed(void);
void InitTimer0(void);
void init_timer1_lcd(void);

#endif
