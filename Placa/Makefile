
#Configuracoes especificas deste projeto
SERIALDEV = /dev/ttyS0
CLOCKFREQ = 12000
TARGET  = PaNKE
#Para Alterar a aplicação mude o primeiro arquivo .c
CSOURCES = ../Aplicacoes/smallocteste3.c $(wildcard ../Aplicacoes/bcp/*.c) $(wildcard ../Kernel/*.c) $(wildcard ../Placa/*.c)
ASOURCES = $(wildcard ../Kernel/*.S) $(wildcard ../Placa/*.S)
#Para Alterar a aplicação mude o primeiro arquivo .h
HEADERS = $(wildcard ../Aplicacoes/bcp/*.h) $(wildcard ../Kernel/*.h) $(wildcard ../Placa/*.h)
MODELO = 2011
TERMINAL = ltser
BAUDRATE = 19200

#Nome do compilador C, assembler e linker
CC      = arm-elf-gcc
LD      = arm-elf-gcc
AS	= arm-elf-as
ISPPROG = lpc21isp -control
AFLAGS  = -mapcs-32 -mcpu=arm7tdmi
CFLAGS  = -Wall -O2 -mcpu=arm7tdmi -D BAUDRATE=$(BAUDRATE) -D MODELO=$(MODELO)
LFLAGS  = -nostartfiles

# Define all object files.
COBJ = $(CSOURCES:.c=.o)
AOBJ = $(ASOURCES:.S=.o)
OBJECTS = $(COBJ) $(AOBJ)

all: $(TARGET).hex

#Converte arquivo elf para hex
%.hex: %.elf
	arm-elf-objcopy -O ihex $< $@

#Chama o compilador c
%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $<

#Compila os modulos em assembly
%.o: %.S
	$(AS) $(AFLAGS) -o $@ $< 

#Chama o linker/loader para juntar os módulos gerando o arquivo elf
$(TARGET).elf: $(OBJECTS)
	$(LD) $(LFLAGS) -Tlpc2378_flash.ld -o $(TARGET).elf $(OBJECTS)

$(TARGET)r.elf: $(OBJECTS)
	$(LD) $(LFLAGS) -Tlpc2378_ram.ld -o $(TARGET)r.elf $(OBJECTS)

tser: $(TARGET)r.hex
	$(TERMINAL) $(SERIALDEV) b=$(BAUDRATE) $(TARGET)r.hex
tseru: $(TARGET)r.hex
	$(TERMINAL) /dev/ttyUSB0 b=$(BAUDRATE) $(TARGET)r.hex

32: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ) && ./../terminal32

64: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ) && ./../terminal64

placa1: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ) && ./../terminal64 /dev/ttyUSB0

placa2: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB1 $(BAUDRATE) $(CLOCKFREQ) && ./../terminal64 /dev/ttyUSB1


# Use 'make isp' para programar a memoria flash
isp: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex $(SERIALDEV) $(BAUDRATE) $(CLOCKFREQ)
ispu: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ)
ispu0: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB0 $(BAUDRATE) $(CLOCKFREQ)
ispu1: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB1 $(BAUDRATE) $(CLOCKFREQ)
ispu2: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex /dev/ttyUSB2 $(BAUDRATE) $(CLOCKFREQ)
isp1: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com1 $(BAUDRATE) $(CLOCKFREQ)
isp2: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com2 $(BAUDRATE) $(CLOCKFREQ)
isp3: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com3 $(BAUDRATE) $(CLOCKFREQ)
isp4: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com4 $(BAUDRATE) $(CLOCKFREQ)
isp5: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com5 $(BAUDRATE) $(CLOCKFREQ)
isp6: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com6 $(BAUDRATE) $(CLOCKFREQ)
isp7: $(TARGET).hex
	$(ISPPROG) $(TARGET).hex com7 $(BAUDRATE) $(CLOCKFREQ)

#Limpa, apagando os arquivos gerados pela compilacao
clean:
	-rm -f ../*/*/*.o ../*/*.o ../*/*.elf ../*/*~ ../*/*.bin ../*/*.map ../*/*.hex
.PHONY: all clean tser tseru isp ispu isp1 isp2 isp3 isp4 isp5 isp6 isp7