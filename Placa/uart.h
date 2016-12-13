#ifndef __UART_H
#define __UART_H

#include "../Kernel/kernel.h"
#include <arch/nxp/lpc23xx.h>

#ifdef PRINTK
#include <stdarg.h>
#endif
/* Mascaras do U0LSR */
#define THREMPTY	0x20
#define RXDREADY	0x01
#define BAUDRATE        19200
/* Configuracoes especificas */
#define SYSCLK 12000000

void U0init(void); //inicia a aplicação com o terminal (USB)
int U0getchar(void); //envia um comando do PC para placa
void U0putchar(int c); //imprimir um caracter
void U0puts(char *txt); // imprime uma string
void sys_nkprint(char *fmt,void *number); //envia um conteúdo qualquer pela serial
void printk(char *fmt,...); //envia um conteúdo qualquer pela serial

#endif
