/***************************************************
* 			/Placa/Timer.h						   *
*   		Driver de rel[ogio do NKE LPC2378	   *
*   		Por Guilherme I Debom Machado		   *
****************************************************
* 												   *
* Esse arquivo é reponsável por acessar e alterar  *
* registradores do relogio e interrupcoes de timer *
* da placa LPC2378 com o Kernel do NKE.	   		   *
* 												   *
***************************************************/

#include <arch/nxp/lpc23xx.h>

#define habilitaIRQ() asm("msr cpsr_c, #0x5f\n");
#define desabilitaIRQ() asm("msr cpsr_c,#0xdf\n");

void InitTimers(void);
void InitRTC();
void InitT0(void);
void InitT1(void);
void getT0(int *t);
void getRTC_SEC(int *t);
void getRTC_MIN(int *t);
void getRTC_HOUR(int *t);
void setRTC_SEC(int t);
void setRTC_MIN(int t);
void setRTC_HOUR(int t);
void WaitT1(unsigned int t);
void IRQT0();
