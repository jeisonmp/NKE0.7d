#ifndef __LCD_H
#define __LCD_H

#include <arch/nxp/lpc23xx.h>
#include "init.h"
#include "interrupt.h"
#include "../Kernel/syscall.h"

#define LCD_E	8
#define LCD_RS	4
#define I_Bit 0x80
#define F_Bit 0x40
#define LIMPA 1 //Limpa a tela do LCD
/*LinhaXY do LCD 
 * 
    Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y  Y
  ------------------------------------------------
X| 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F|
X| 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F|
  ------------------------------------------------
  */
#define LINHA10 0x80
#define LINHA11 0x81
#define LINHA12 0x82
#define LINHA13 0x83
#define LINHA14 0x84
#define LINHA15 0x85
#define LINHA16 0x86
#define LINHA17 0x87
#define LINHA18 0x88
#define LINHA19 0x89
#define LINHA1A 0x8A
#define LINHA1B 0x8B
#define LINHA1C 0x8C
#define LINHA1D 0x8D
#define LINHA1E 0x8E
#define LINHA1F 0x8F
#define LINHA20 0xC0
#define LINHA21 0xC1
#define LINHA22 0xC2
#define LINHA23 0xC3
#define LINHA24 0xC4
#define LINHA25 0xC5
#define LINHA26 0xC6
#define LINHA27 0xC7
#define LINHA28 0xC8
#define LINHA29 0xC9
#define LINHA2A 0xCA
#define LINHA2B 0xCB
#define LINHA2C 0xCC
#define LINHA2D 0xCD
#define LINHA2E 0xCE
#define LINHA2F 0xCF

void StartLCD(void);
void WaitTimer1(unsigned int t);
void LCDputchar(int c);
void LCDnum(int valor);
void LCDputs(char *txt);
void LCDcomando(int c);
void LCDinit(void);
void LCDnumI(int valor,int c);
void LCDputsI(char *txt,int c);

#endif
