/*
Programa terminal com comandos para transferir arquivos
escrito por Marcos A. stemmer
Aula de Sistemas Embarcados 2012/1
Programa para sistemas POSIX (Linux)

modificado por Lucas Murliky
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <time.h>
#define DEBUG2

int sd;    /* Serial port handle */
struct termios mytty, kbdios;   /* Console settings */
char *errmsg[]={"OK", "File read error","Checksum error", "No answer"};

/* This function is called whenever the program terminates */
void back_to_normal(void)
{
/* Set console back to normal */
fcntl(STDIN_FILENO, 0, FNDELAY);
tcsetattr(STDIN_FILENO, TCSANOW, &kbdios);
}

void kbd_rawmode()
{
/* Set keyboard to raw mode */
tcgetattr(STDIN_FILENO, &kbdios);
atexit(back_to_normal);         /* Restore tty mode when exiting */
memcpy(&mytty, &kbdios, sizeof(struct termios));
mytty.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
tcsetattr(STDIN_FILENO, TCSANOW, &mytty);
/* These calls make read to return imedately even when there is
   no data in the input stream */
fcntl(sd, F_SETFL, FNDELAY);
fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
}

int OpenSerial(char *serial_dev, int baudrate)
{
char msg[80];
int baudflag;
sd=open(serial_dev, O_RDWR | O_NDELAY);
if(sd==-1) {
	sprintf(msg, "\nUnable to open \"%s\"", serial_dev);
	perror(msg);
	return -1;
	}
/* RESET ligado em DTR
   TST ligado em RTS ambos normalmente em nivel baixo*/
baudflag = TIOCM_DTR;
if(ioctl(sd, TIOCMBIC, &baudflag) < 0) perror("ioctl failed");
baudflag = TIOCM_RTS;
if(ioctl(sd, TIOCMBIC, &baudflag) < 0) perror("ioctl failed");
/* Set attributes of the serial interface to raw mode */
if(tcgetattr(sd, &mytty)==-1) {
	perror(NULL);
	close(sd);
	return -1;
	}
mytty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
mytty.c_oflag &= ~OPOST;
mytty.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
mytty.c_cflag &= ~(CSIZE|PARENB);
mytty.c_cflag |= CS8;
/* Default Baud Rate: 19200 baud */
switch(baudrate) {
	case 115200: baudflag=B115200; break;
	case 57600: baudflag=B57600; break;
	case 38400: baudflag=B38400; break;
	case 9600: baudflag=B9600; break;
	case 4800: baudflag=B4800; break;
	default: baudrate=19200; baudflag=B19200; break;
	}
cfsetispeed(&mytty, baudflag);
cfsetospeed(&mytty, baudflag);
if(tcsetattr(sd, TCSANOW, &mytty)==-1) {
	perror(NULL);
	close(sd);
	return -1;
	}
fprintf(stderr, 
"PaNKE-Terminal Opened \"%s\" @ %d baud\n"
"Ctrl-C to exit\n"
"Ctrl-R RESET remote device\n", serial_dev, baudrate);
return 0;
}

/* Retorna apontador para a extencao */
char *extencao(char *nome)
{
char *p;
for(p=nome; *nome; nome++) if(*nome=='.') p=nome;
return p;
}

/* Envia arquivo em formato HEX da intel */
void envia_hex(char *nomearq)
{
FILE *arq;
char bloco[256];
int n,ack, erro;
fd_set descritores;     /* Set of i/o handles */
struct timeval timeout;
arq=fopen(nomearq,"rb");
erro=0;
if(!arq) { perror(NULL); return; }
do	{
	fgets(bloco,80,arq);
	write(sd, bloco, strlen(bloco));
	FD_ZERO(&descritores);  /* Inicializa a lista de handles */
	FD_SET(sd,  &descritores);
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	select(FD_SETSIZE, &descritores, NULL, NULL, &timeout);
	ack=16*bloco[7]+bloco[8];
	if((n=read(sd,bloco, 80))<0) { erro=3; break; }
	write(STDIN_FILENO, bloco, n);
	if(*bloco!='.' && *bloco!='E') { erro=3; break; }
	} while(!feof(arq) && ack!=(16*'0'+'1'));
fprintf(stderr, "%s\n", errmsg[erro]);
fclose(arq);
}

/* Envia arquivo binario */
void envia_arquivo(char *nomearq, int endini)
{
FILE *arq;
char chksum, ack, ff=0xff, bloco[256];
int tamanho, k, nrep, erro;
fd_set descritores;     /* Set of i/o handles */
struct timeval timeout;
arq=fopen(nomearq,"rb");
erro=ack=0;
if(!arq) { perror(NULL); return; }
do	{
	if(!ack) { tamanho = fread(bloco, 1, 256, arq); nrep=3; }
	if(!tamanho) { erro=1; break; }
	write(sd, &ff,1);
	write(sd, &endini,2);
	write(sd, &tamanho,2);
	write(sd, bloco, tamanho);
	for(k=0, chksum=0; k<tamanho; k++) chksum -= bloco[k];
	write(sd, &chksum, 1);
	FD_ZERO(&descritores);  /* Inicializa a lista de handles */
	FD_SET(sd,  &descritores);
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	select(FD_SETSIZE, &descritores, NULL, NULL, &timeout);
	if(read(sd,&ack, 1)<0) { erro=3; break; }
	if(!ack) { endini += tamanho; write(STDIN_FILENO, ".",1); }
	else nrep--;
	if(!nrep) { erro=2; break; }
	} while(!feof(arq));
fprintf(stderr, "%s\n", errmsg[erro]);
fclose(arq);
}

int sd_getchar(int to)
{
int c=0;
fd_set descritores;     /* Set of i/o handles */
struct timeval timeout;
if(read(sd,&c, 1) > 0) return c;
FD_ZERO(&descritores);  /* Inicializa a lista de handles */
FD_SET(sd,  &descritores);
timeout.tv_sec=5;
timeout.tv_usec=0;
if(select(FD_SETSIZE, &descritores, NULL, NULL, &timeout)<=0) return -1;
if(read(sd,&c, 1) < 0) c = -1;
return c;
}

void transfere(int comando)
{
char buf[512];
char modo[4];
int i, n;
static FILE *arq;
static int ntotal;
#ifdef DEBUG
fprintf(stderr,"Comando %c\n", comando);
#endif
modo[1]='\0'; modo[0]='r';
n = sd_getchar(5);
#ifdef DEBUG
fprintf(stderr,"n=%d\n", n);
#endif
if(comando!='R'){
	for(i=0; i < n; i++) {
		buf[i] = sd_getchar(5);
		//if(buf[i] <= 0) break;
		}
	buf[i] = '\0';
	}
i=0;
switch(comando) {
	case 'B': modo[0]='w';
	case 'A':
		ntotal = 0;
		arq = fopen(buf, modo);
		if(arq==NULL) i = errno;
#ifdef DEBUG
		fprintf(stderr,"Abrindo %02x \"%s\" como \"%s\"\n", buf[0] & 0xff, buf,modo);
#endif
		n=0;
		break;
	case 'R':
		//putc('.',stderr);
		n=fread(buf,1,n,arq);
		ntotal += n;
#ifdef DEBUG2
		fprintf(stderr,"\rLeu %d", ntotal);
#endif
		break;
	case 'W':
		//putc('.',stderr);
		n=fwrite(buf,1,n,arq);
		ntotal += n;
#ifdef DEBUG2
		fprintf(stderr,"\rN = %d", ntotal);
#endif
		break;
	case 'F':
#ifdef DEBUG
		fprintf(stderr,"Fechou\n");
#endif
		fclose(arq);
		putc('\n',stderr);
		n=0;
		break;
	default: n=0;
		break;
	}
modo[0]=i;
modo[1]=n;
#ifdef DEBUG
fprintf(stderr,"Respondeu: erro=%d n=%d\n", modo[0] & 0xff, modo[1] & 0xff);
#endif
write(sd, modo,2);
if(comando == 'R'){
	write(sd, buf, n);
	}
}

void reset(void)
{
int n;
n = TIOCM_DTR;
if(ioctl(sd, TIOCMBIS, &n) < 0) perror("ioctl");
usleep(500000);	/* 500 ms */
ioctl(sd, TIOCMBIC, &n);
}

int main(int argc, char **argv)
{
  char letra[40], *portname, *filename, *ext;
  FILE *lista;
  int endini, i, n;
  int baudrate;
  int arq;
  int plot=0;
  time_t result = time(NULL);
  fd_set descritores;     /* Set of i/o handles */
  FILE *fp;
  /* Make shure that input is a keybord */
  if(!isatty(STDIN_FILENO)) {
    fputs("\nInput must be from a keyboard.\n", stderr);
    return -1;
  }
  lista=NULL;
  filename=NULL;
  fp=NULL;
  endini=0;
  /* Open serial port */
  portname="/dev/ttyUSB0";
  baudrate=-1;
  for(i=1; i<argc; i++) 
  {
    if(*argv[i]=='b' && argv[i][1]=='=') { 
      baudrate=atoi(argv[i]+2); continue; 
    }
    else if(!(memcmp("/dev/tty",argv[i],8))) 
      portname=argv[i];
    else if(!(memcmp("/dev/pts/",argv[i],9))) 
      lista = fopen (argv[i], "a+" ) ;
    else if(!(memcmp("-f",argv[i],2)))
      fp = fopen(asctime(localtime(&result)),"w");
    else if(!(memcmp("--plot",argv[i],6)))
    {
      fp = fopen("plot.txt","w");
      plot = 1;
    }
    else
      filename=argv[i];
//     else portname=argv[i];
  }
  puts("\nSerial terminal/uploader for Linux: Command line:\n"
  "\tterminal64 [-f] [serial_device] [tty_out] [b=baudrate] [file.hex]\n");

  if(OpenSerial(portname, baudrate)) return 1;
  kbd_rawmode();
  /* Now we may use the serial interface and keyboard for sending
  just one character at a time */
  usleep(500000);	/* 500 ms */
  reset();
  usleep(500000);	/* 500 ms */
  if(filename!=NULL) 
  {
    ext=extencao(filename);
    if(!strcmp(ext,".ihx") || !strcmp(ext,".hex")) 
    {
      while ((n=read(sd, letra, 4)) > 0);
      usleep(100);
      envia_hex(filename);
    }
  }
  
  do
  {
    /* Espera receber algo na porta serial (sd) 
    ou no teclado (STDIN_FILENO) */
    FD_ZERO(&descritores);  /* Inicializa a lista de handles */
    FD_SET(sd,  &descritores);
    FD_SET(STDIN_FILENO, &descritores);
    select(FD_SETSIZE, &descritores, NULL, NULL, NULL);
    if ((n=read(sd, letra, 1)) > 0)
    {
      if((unsigned char)letra[0] == 0xe0) 
      {
	    transfere(sd_getchar(5));
      }
      else //recebe dado da serial
      { 
//	write(STDIN_FILENO, letra, n); 
	    if(lista!=NULL)
	      fprintf(lista,"%c",letra[0]);
	    else
	      write(STDIN_FILENO, letra, n);
        if(letra[0]!='\0' && fp!=NULL)
        {
 //       	if(!(letra[0]=='\n'))
        	  fprintf(fp,"%c",letra[0]);
        }
      }
    }
    *letra='\0';
    while((n=read(STDIN_FILENO, letra,4)) > 0) write(sd, letra, n); //captura tecla no terminal
    /* CTRL-R Reset msp430 board */
    if(*letra == 'R'-64) reset();
  } while(*letra!='C'-64);
  if(fp!=NULL)
    fclose(fp);
  if(plot==1)
  {
	printf("Gerando Grafico\n");
	system("R CMD BATCH plot.R");
	printf("plot.png criado!\n");
  }
  close(sd);
  return 0;
}