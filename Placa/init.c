#include "init.h"

void SaidaLed()
{
  FIO4DIR = 0xff;	/* Configura LEDs como saida	*/
  FIO4SET = 0xff;		/* LEDs iniciam apagados */
}

void InitTimer0(void)
{
  float t;
  t=12000000*ClkT;
  T0MR0 = t-1; /* 1msec = 12000000/1000 - 1 at 12.0 MHz */
  T0MR2 = 1;
  T0MCR = 3; /* Interrupt and Reset on MR0 */
  T0TCR = 1; /* Timer0 Enable */
}

void init_timer1_lcd(void)
{
	T1TCR = 0;              /* Desabilita T0 */
	T1PR = 24 - 1; /* Incrementa o contador T0TC 1000 vezes por segundo */
	T1TCR = 2;      /* Inicializa T0 */
	T1TCR = 1;      /* Habilita T0  */
}
