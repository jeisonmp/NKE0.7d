/******************************************************
crt.S
Este modulo em assembly tem os vetores de interrupcao
e os procedimentos de inicializacao
***************************************************** */
	.extern	main
	.extern	exit

	.text
	.code 32
	.align	0
/* Nomes dos segmentos definidos no script do linker *.ld */
	.extern	__bss_beg__
	.extern	__bss_end__
	.extern	__stack_end__
	.extern	__data_beg__
	.extern	__data_end__
	.extern	__data+beg_src__

	.global	initial
	.global	endless_loop

	/* Tamanho das pilhas */
	.set	UND_STACK_SIZE,	0x00000800
	.set	ABT_STACK_SIZE,	0x00000800
	.set	FIQ_STACK_SIZE,	0x00000800
	.set	IRQ_STACK_SIZE,	0X00000800
	.set	SVC_STACK_SIZE,	0x00000800 

	/* Bits de definicao dos modos de operacao no CPSR */
	.set  MODE_USR,	0x10		/* User	Mode */
	.set  MODE_FIQ,	0x11		/* FIQ Mode */
	.set  MODE_IRQ,	0x12		/* IRQ Mode */
	.set  MODE_SVC,	0x13		/* Supervisor Mode */
	.set  MODE_ABT,	0x17		/* Abort Mode */
	.set  MODE_UND,	0x1B		/* Undefined Mode */
	.set  MODE_SYS,	0x1F		/* System Mode */

	.equ  I_BIT, 0x80		/* when	I bit is set, IRQ is disabled */
	.equ  F_BIT, 0x40		/* when	F bit is set, FIQ is disabled */

initial:
_initial:
_mainCRTStartup:
/* Configura a pilha para cada modo de operacao: system/user, SWI e IRQ */
	ldr   r0, .LC6
	msr   CPSR_c, #MODE_UND|I_BIT|F_BIT /* Undefined Instruction Mode */
	mov   sp, r0
	sub   r0, r0, #UND_STACK_SIZE
	msr   CPSR_c, #MODE_ABT|I_BIT|F_BIT /* Abort Mode */
	mov   sp, r0
	sub   r0, r0, #ABT_STACK_SIZE
	msr   CPSR_c, #MODE_FIQ|I_BIT|F_BIT /* FIQ Mode	*/
	mov   sp, r0
	sub   r0, r0, #FIQ_STACK_SIZE
	msr   CPSR_c, #MODE_IRQ|I_BIT|F_BIT /* IRQ Mode	*/
	mov   sp, r0
	sub   r0, r0, #IRQ_STACK_SIZE
	msr   CPSR_c, #MODE_SVC|I_BIT|F_BIT /* Supervisor Mode */
	mov   sp, r0
	sub   r0, r0, #SVC_STACK_SIZE
	msr   CPSR_c, #MODE_SYS|I_BIT|F_BIT /* System Mode */
	mov   sp, r0

	/* Inicia em modo supervisor. */
	msr   CPSR_c, #MODE_SVC|I_BIT|F_BIT

	/* Limpa BSS. */

	mov	a2, #0		/* Fill value */
	mov	fp, a2		/* Null frame pointer */
	mov	r7, a2		/* Null frame pointer for Thumb	*/

	ldr	r1, .LC1	/* Start of memory block */
	ldr	r3, .LC2	/* End of memory block */
	subs	r3, r3,	r1	/* Length of block */
	beq	.end_clear_loop
	mov	r2, #0

.clear_loop:
	strb	r2, [r1], #1
	subs	r3, r3,	#1
	bgt	.clear_loop

.end_clear_loop:

/* Initialise data. */
	ldr	r1, .LC3		/* Start of memory block */
	ldr	r2, .LC4		/* End of memory block */
	ldr	r3, .LC5
	subs	r3, r3,	r1		/* Length of block */
	beq	.end_set_loop

.set_loop:
	ldrb	r4, [r2], #1
	strb	r4, [r1], #1
	subs	r3, r3,	#1
	bgt	.set_loop

.end_set_loop:

/******** Configura clock */
		ldr	r3, = 0xE01FC100
		mov	r2, #0x21	/* Liga	clock 12M */
		str	r2, [r3, #0xa0]	/* SCS = 0x21	GPIO em	modo Fast */
Lp2:
		ldr	r2, [r3, #0xa0]	/* Espera estabilizar		*/
		tst	r2, #0x40	/* while(SCS & 0x40);		*/
		beq	Lp2
		mov	r2, #1		/* Usa clock 12M para a	CPU	*/
		str	r2, [r3, #0x0c]	/* CLKSRCSEL = 1;		*/
		mov	r2, #0		/* Divisor por 1 na CPU		*/
		str	r2, [r3, #0x04]	/* CCLKCFG=0;			*/
/* Divisor por 4 nos perifericos */
		ldr	r2, = 0x55555555
		str	r2, [r3, #0xA8]	/* PCLKSEL0 = 0x55555555;	*/
		str	r2, [r3, #0xAC]	/* PCLKSEL1 = 0x55555555;	*/
		mov	r2, #0		/*Deve fazer PINSEL10=0	para que */
/* Coloca 0 em todos os	PINSELs	(Todos os pinos	como GPIO)*/
		ldr	r3, = 0xE002C000
		str	r2, [r3, #0x00]
		str	r2, [r3, #0x04]
		str	r2, [r3, #0x08]
		str	r2, [r3, #0x0c]
		str	r2, [r3, #0x10]
		str	r2, [r3, #0x14]
		str	r2, [r3, #0x18]
		str	r2, [r3, #0x1c]
		str	r2, [r3, #0x20]
		str	r2, [r3, #0x24]
		str	r2, [r3, #0x28]
/* Configura o MAM */
		ldr	r3, = 0xE01FC000
		str	r2, [r3, #0]	/* MAMCR=0 */
		mov	r2, #3
		str	r2, [r3, #4]	/* MAMTIM=3 */
		mov	r2, #2
		str	r2, [r3, #0]	/* MAMCR=2 */
/******	Fim da configuracao do clock *******/

/* Entra na funcao main() do programa em C  */
		mov	r0, #0		/* argc = 0  */
		mov	r1, r0		/* argv = NULL */
		mov	lr, r0
		ldr	pc, = StartKernel

		.align 0
.LC1:		.word	__bss_beg__
.LC2:		.word	__bss_end__
.LC3:		.word	__data_beg__
.LC4:		.word	__data_beg_src__
.LC5:		.word	__data_end__
.LC6:		.word	__stack_end__

.section .startup,"ax"
		.code 32
		.align 0
/* Vetores de interrupcao */
_vectors:	ldr	PC, Reset_Addr
		ldr	PC, Undef_Addr
		ldr	PC, SWI_Addr
		ldr	PC, PAbt_Addr
		ldr	PC, DAbt_Addr
		nop	/* Reservado para a soma (checksum) dos vetores */
		ldr	PC, [PC,#-0x0120]
		ldr	PC, FIQ_Addr

Reset_Addr:	.word	initial		/* definido neste modulo */
Undef_Addr:	.word	UNDEF_Routine	/* definido no main.c  */
SWI_Addr:	.word	SystemContext	/* definido no main.c  */
PAbt_Addr:	.word	UNDEF_Routine	/* definido no main.c  */
DAbt_Addr:	.word	UNDEF_Routine	/* definido no main.c  */
FIQ_Addr:	.word	FIQ_Routine	/* definido no main.c  */
		.word	0	/* Arredonda para um total de 16 vetores  */
