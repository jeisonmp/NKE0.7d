#include "driver_LED.h"

void StartLED(){	/* Inicia LEDs apagados*/
	SetLED();
	LigaLED(0);
};

void SetLED(){
	FIO4DIR = 0xff;	/* Bits 0 a 7 ligados nos LEDs: devem ser saidas */
};

void LigaLED(int a){
	FIO4SET = 0xff;	/* Apaga todos os LEDs */
	FIO4CLR = a;	/* Liga um LED da sequencia */
};
