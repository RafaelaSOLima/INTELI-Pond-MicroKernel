# Makefile para Kernel C com Jogo Adivinhe o Número

ASM = nasm
CC = gcc
LD = ld

# Flags do compilador
CFLAGS = -m32 -ffreestanding -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -fno-stack-protector -fno-pie -c
LDFLAGS = -m elf_i386 -T linker.ld

.PHONY: all clean run debug info

all: os.img

# Bootloader
bootloader.bin: boot.asm
	$(ASM) -f bin boot.asm -o bootloader.bin

# Entrada do kernel em assembly
kernel_entry.o: kernel_start.asm
	$(ASM) -f elf32 kernel_start.asm -o kernel_entry.o

# Arquivos C do kernel
kernel.o: kernel.c kernel.h
	$(CC) $(CFLAGS) kernel.c -o kernel.o

display.o: display.c kernel.h
	$(CC) $(CFLAGS) display.c -o display.o

interrupts.o: interrupts.c kernel.h
	$(CC) $(CFLAGS) interrupts.c -o interrupts.o

io.o: io.c kernel.h
	$(CC) $(CFLAGS) io.c -o io.o

# Handlers de interrupção em assembly
int_handlers.o: int_handlers.asm
	$(ASM) -f elf32 int_handlers.asm -o int_handlers.o

# Linkar o kernel
kernel.bin: kernel_entry.o kernel.o display.o interrupts.o io.o int_handlers.o
	$(LD) $(LDFLAGS) -o kernel.elf kernel_entry.o kernel.o display.o interrupts.o io.o int_handlers.o
	objcopy -O binary kernel.elf kernel.bin

# Criar imagem
os.img: bootloader.bin kernel.bin
	dd if=/dev/zero of=os.img bs=512 count=2880
	dd if=bootloader.bin of=os.img bs=512 count=1 conv=notrunc
	dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc

# Executar
run: os.img
	qemu-system-i386 -drive format=raw,file=os.img,if=floppy

# Debug detalhado
debug: os.img
	qemu-system-i386 -drive format=raw,file=os.img,if=floppy -d cpu_reset,int -monitor stdio

# Limpar
clean:
	rm -f *.o *.bin *.img *.elf

# Mostrar info
info: os.img
	ls -la *.bin *.img
	hexdump -C bootloader.bin | head -5
	hexdump -C kernel.bin | head -5