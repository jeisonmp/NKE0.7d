#include "Timer.h"
#include "uart.h"

/*
* Registradores:
* T0IR   -> Registrador de Interrupção, pode apagar interrupções (escrita) ou 
*           identificar qual das 8 interrupções estão pendentes (leitura)
* T0TCR  -> T0 Timer Control Register (bit 0 -> habilita, bit 1 -> Inicia em 0)
* T0TC   -> T0 Timer Count (Contém o valor da contagem)
* T0PR   -> T0 Prescale Register (Configura incremento do timer)
* T0PC   -> T0PC é incrementado até o valor de T0PR, quando isso ocorre T0TC é 
*           incrementado e T0PR é zerado
* T0MCR  -> é usada para controle caso uma interrupção seja gerada e o TC rese-
*           tado em ocorrencia de "match"
* T0MR0  -> Pode ser habilitado através do MCR para resetar o TC, parar TC e PC 
*           e/ou gerar interrupções quando MR0 alcança TC.
*/

void getT0(int *t){
	*t = T0TC;
}

void getRTC_SEC(int *t){
	*t = RTC_SEC;
}

void getRTC_MIN(int *t){
	*t = RTC_MIN;
}

void getRTC_HOUR(int *t){
	*t = RTC_HOUR;
}

void setRTC_SEC(int t){
	RTC_SEC = t;
}

void setRTC_MIN(int t){
	RTC_MIN = t;
}

void setRTC_HOUR(int t){
	RTC_HOUR = t;
}

void InitTimers(void){
	InitRTC();
	InitT0();
	InitT1();
}

void InitRTC(){
	RTC_CCR=0x11;
	RTC_CISS = 0x85;	/* Ativa RTC IRQ -> 1 interr/15,6mSec = 64,1 interr/Sec  */
	RTC_ILR=0;
	RTC_HOUR=0;
	RTC_MIN=0;
	RTC_SEC=0;
}

void IRQT0(){
	desabilitaIRQ();
	VICVectAddr4 = (unsigned long)Interrupt;  
	VICIntSelect &= ~(0x10);
	VICIntEnable = (1 << 4);
	habilitaIRQ();
	while(1); //aguarda a interrupção do clock -- a interrupção acorda o main
}
void WaitT1(unsigned int t){	//Timer bloqueante utilizado pelo LCD
	unsigned int tf;
	tf = T1TC + t;          /* tf = Valor futuro do T1TC */
	while(tf != T1TC);      /* espera ate que tf == T1TC */
}
void InitT1(void){	//T1 é usado pelo LCD 
	T1TCR = 0; 			// Desabilita T1
	T1PR = 23; 		// Prescaler para incrementar T1TC a cada milisegundo
	T1TCR = 2; 			// Inicializa T1
	T1TCR = 1; 			// Habilita T1
}

void InitT0(void){	/* Timer0 é usado para inicializar o kernel e contar */
	T0TCR = 0; 			/* Desabilita T0 */
	
	/* Define interrupção para iniciar o kernel */
	T0MR0 = 11999999; 	/* 1sec = 12000000 -1 at 12.0MHz */
	T0MR2 = 1;       	/* 1usec = 12000000/1000000 -1 at 12MHz */
	T0MCR = 3;        	/* Interrupt and Reset on MR0 */ 
  
	/* Define incremento do timer e habilita */
	T0PR  = 0;			/* T0TC Incrementa a cada ciclo (12MHz) */
	T0TCR = 1;        	/* Timer0 Enable */
}
