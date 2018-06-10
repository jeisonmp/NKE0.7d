#include "lcd.h"

void StartLCD()
{
  init_timer1_lcd();
  LCDinit();
}
void WaitTimer1(unsigned int t)
{
  unsigned int tf;
  tf = T1TC + t;          /* tf = Valor futuro do T1TC */
  while(tf != T1TC);      /* espera ate que tf==T1TC */
}
//Escreve *Char
void LCDputchar(int c)
{
  FIO3PIN0 = c;
  FIO2SET = LCD_RS;	/* RS=1: caractere */
  FIO2SET = LCD_E;
  FIO2CLR = LCD_E;
  WaitTimer1(800);
}
//Escreve Int
void LCDnum(int valor)
{
  int dezena=10,suporte;
  if(valor<0)
  {
    LCDputs("-");
    valor*=-1;
  }
  while(valor>=dezena)
    dezena*=10;
  dezena=dezena/10;
  while(dezena!=1)
  {
    LCDputchar((valor/dezena) + '0'); /* Escreve dezena  */
    suporte=valor/dezena;
    valor+=-suporte*dezena;
    dezena=dezena/10;
    
  }
  LCDputchar((valor%10) + '0');	/* Escreve unidade */
}
//Escreve String
void LCDputs(char *txt)
{
  while(*txt) LCDputchar(*txt++);
}
/* Escreve um comando para o LCD */
void LCDcomando(int c)
{
  FIO3PIN0 = c;
  FIO2CLR = LCD_RS;	/* RS=0: comando */
  FIO2SET = LCD_E;
  FIO2CLR = LCD_E;
  WaitTimer1(2000);
}
//Inicializa o LCD
void LCDinit(void)
{
  init_timer1_lcd();
  /* Configura portas I/O do LCD */
  FIO3DIR |= 0xff;	/* Dados do LCD como saidas */
  FIO2DIR  |= LCD_E | LCD_RS;	/* EN e RS do LCD como saidas */
  FIO2CLR = LCD_E;
  WaitTimer1(2000);
  LCDcomando(0x38);	/* Configura LCD para 2 linhas */
  LCDcomando(1);		/* Limpa display */
  LCDcomando(0x0c);	/* Apaga cursor */
}

