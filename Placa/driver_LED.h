/****************************************************
* 			/Placa/driver_LED					    *
*   		Driver de LED - LPC2378				    *
*   		Por Guilherme I Debom Machado		    *
*****************************************************
* 													*
* Esse arquivo é responsável pelo controle dos LEDs	*
* da placa LP2378 no NKE.							*
* 													*
****************************************************/

#include <arch/nxp/lpc23xx.h>

void StartLED();		/* Inicia LEDs apagados*/
void SetLED();			/* Configura LEDs como saída */
void LigaLED(int a);    /* Liga LEDs */

