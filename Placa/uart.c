#include "uart.h"
/* Configuracao da Porta Serial 0 */
void U0init(void)
{
  PCLKSEL0 = (PCLKSEL0 & (~0xc0)) | 0x40; /* divide por 1 no PCLK da UART0 */
  PCONP |= 8;		/* Liga energia da UART0 */
  PINSEL0 = (PINSEL0 & (~0xf0)) | 0x50;	/* Seleciona pinos TxD0 e RxD0 */
  U0FCR   = 0x7;		/* Habilita as FIFOs e reset */
  U0LCR   = 0x83;		/* Habilita acesso ao divisor de baud-rate (DLAB) */
  U0DLL   = ((SYSCLK/BAUDRATE+8) >> 4) & 0xff;
  U0DLM   = ((SYSCLK/BAUDRATE) >> 12) & 0xff;
  U0LCR    = 0x03;	/* Configura UART0 como 8N1 */
}

/* Recebe um caractere da porta serial */
int U0getchar(void)
{
  while((U0LSR & 1) == 0);	/* Espera ate receber algo */
  return U0RBR;
}

/* Envia um caractere */
void U0putchar(int c)
{
  while((U0LSR & 0x20) == 0);	/* Tespera transmissor disponivel */
  U0THR = c;	/* Transmite */
}

/* Envia uma mensagem pela UART0 */
void U0puts(char *txt)
{
  while(*txt) U0putchar(*txt++);
}

static inline void putk(int c)
{
  U0putchar(c);
}

static inline void printfloat(double val,int size)
{
  int chr[64];
  int precisao=1;
  int i = 0;
  for(i=0;i<size;i++)
    precisao*=10;
  val*=precisao;
  if(val==0)
    putk('0');
  for(i=0;!(val<1);i++)
  {
    chr[i] = 0x30 + ((int)val % 10);
    val = val/10;
  }
  i--;
  while (i >= 0)
  {
    if(i==size-1 && size!=0)
      putk('.');
    putk(chr[i]);
    i--;
  }
  
}

static inline void printhexU(int val)
{
  static const char *hex="0123456789ABCDEF";
  int i=32;
  int k=7;
  while (i >= 4) 
  {
    putk(hex[(val >> (k * 4)) & 0xF]);
    k--;
    i-=4;
  }
}

static inline void printhexL(int val)
{
  static const char *hex="0123456789abcdef";
  int i=32;
  int k=7;
  while (i >= 4) 
  {
    putk(hex[(val >> (k * 4)) & 0xF]);
    k--;
    i -= 4;
  }
}

static inline void printbinary(int val, int size)
{
  if (!size)
    return;
  size -= 1;
  if (size > 32)
    size = 32;
  int i = size;
  while (i >= 0)
  {
    putk(0x30 + ((val >> i) & 1));
    i--;
  }
}

void sys_nkprint(char *fmt,void *number)
{
//  va_list va;

  int size=0;
  int accuracy=1;

  while (*fmt)
  {
    if (*fmt != '%')
    {
      putk(*fmt);
//      if (*fmt == '\n')
//	      putk('\r');
      fmt++;
    }
    else
    {
      fmt++;
      switch(*fmt)
      {
	case '%':
	  putk(*fmt);
	  break;
	case 'd':
	  printfloat((int)number,0);
	  break;
	case 'f':
	  printfloat(0,6);
	  break;
	case '.':
	  fmt++;
	  while(*fmt != 'f')
	  {
	    size*=accuracy;
	    size+= (*fmt - '0');
	    accuracy*=10;
	    fmt++;
	  }
	  printfloat(0,size);
	  break;
	case 'x':
	  printhexL(0);
	  break;
	case 'X':
	  printhexU(0);
	  break;
	case 's':
	  sys_nkprint((char*)number,0);
	  break;
	case 'b':
	  fmt++;
	  switch(*fmt)
	  {
	    case 'b':
	      size=8;
	      break;
	    case 'w':
	      size=16;
	      break;
	    case 'd':
	      size=32;
	      break;
	    default:
	      fmt -= 2;
	      size = 32;
	      break;
	  }
	  printbinary(0, size);
	  break;
	default:
	  break;
      }
    fmt++;
    }
  }
//  va_end(va);
}

#ifdef PRINTK
void printk(char *fmt, ...)
{
  va_list va;
  int size=0;
  int accuracy=1;

  va_start(va, fmt);

  while (*fmt)
  {
    if (*fmt != '%')
    {
      putk(*fmt);
//      if (*fmt == '\n')
//	      putk('\r');
      fmt++;
    }
    else
    {
      fmt++;
      switch(*fmt)
      {
	case '%':
	  putk(*fmt);
	  break;
	case 'd':
	  printfloat(va_arg(va, int),0);
	  break;
	case 'f':
	  printfloat(va_arg(va, double),6);
	  break;
	case '.':
	  fmt++;
	  while(*fmt != 'f')
	  {
	    size*=accuracy;
	    size+= (*fmt - '0');
	    accuracy*=10;
	    fmt++;
	  }
	  printfloat(va_arg(va, double),size);
	  break;
	case 'x':
	  printhexL(va_arg(va, int));
	  break;
	case 'X':
	  printhexU(va_arg(va, int));
	  break;
	case 's':
	  printk(va_arg(va, char *));
	  break;
	case 'b':
	  fmt++;
	  switch(*fmt)
	  {
	    case 'b':
	      size=8;
	      break;
	    case 'w':
	      size=16;
	      break;
	    case 'd':
	      size=32;
	      break;
	    default:
	      fmt -= 2;
	      size = 32;
	      break;
	  }
	  printbinary(va_arg(va, int), size);
	  break;
	default:
	  break;
      }
    fmt++;
    }
  }
  va_end(va);
}
#endif