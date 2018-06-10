/*
 * smalloc.h
 *
 *  Criado em: Nov 20, 2015
 *      Autor: Prass, Max J.
 *    Projeto: NanoKernel
 *  Descricao: Implementacao do malloc/free para Sistema de Tempo Real - Alocador Dinamico de Memoria Configuravel.
 *     Idioma: BR
 *
 */

#ifndef _SMALLOC_H
#define _SMALLOC_H

#include <stdio.h>
#include <stdint.h>

/*
 * VARIAVEIS DE CONTROLE - CONFIGURADAS DE ACORDO COM O SISTEMA
 * - SZREG         = Tamanho da primeira regiao
 * - SIZEMEM       = Define o tamanho do bloco de memoria total.
 * - NBITMASK      = Define o numro de regioes da memoria disponivel.
 * - TB2[NBITMASK] = Define o tamanho dos blocos em cada regiao.
 */

#define SZREG     4096
#define SZREG_log 12    //SZREG_log deve ser um numero igual a Log2(SZREG). Ex: Log2(32) = 5
#define SIZEMEM   4096 //3960
#define NBITMASK  1

/*
 * BIT ARRAYS
 * MACROS PARA TRATAMENTO DA MASCARA TB
 */
#define CLEANBIT(A,k)  (A &= ~(0x01 << k))
#define SETBIT(A,k)    (A |= (0x01 << k))
#define TESTBIT(A,k)   ((A << (31 - k)) >> 31)
#define REGIONINI(b)    ((0x01 << (b+SZREG_log)) - SZREG + b)  //0
#define REGIONEND(b)   ((0x01 << (b+SZREG_log+1)) - SZREG + b) //4064

static uint32_t MEMORY[SIZEMEM];                                   // Memoria disponivel de tamanho 'sizemem'
static uint32_t TB1 = 0; //0x0000000                               // Mapa de bits da memoria - 1=cheia, 0=nao cheia. Ex.: 0000011=Primeira e segunda regiao cheias.
static uint8_t  TB2[NBITMASK] = {                             5 };      // Define o tamanho dos blocos de cada regiao - 2o nivel
                               //64,128,256,512,1024,2048, 4096
/*
 * CONSTANTES - NAO CONFIGURAVEIS
 */

struct smalloc
{
	struct smalloc *nextfree;
};

int* smalloc(int size);
void sfree(void *addr);
//void PRINTMEMORY();
void k_heap_init(); // TODO: retirar pois foi usada somente para teste com outro alocador que tinha esta funcao.

#endif
