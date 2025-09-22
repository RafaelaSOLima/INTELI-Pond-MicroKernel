# Makefile Simplificado para Debug

ASM = nasm
CC = gcc
LD = ld

.PHONY: all clean run debug

all: os.img

# Bootloader
bootloader.bin: boot.asm
	$(ASM) -f bin boot.asm -o bootloader.bin

# Kernel simples (apenas assembly)
kernel.bin: kernel_entry.asm
	$(ASM) -f bin kernel_entry.asm -o kernel.bin

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
	rm -f *.bin *.img *.o

# Mostrar info
info: os.img
	ls -la *.bin *.img
	hexdump -C bootloader.bin | head -5
	hexdump -C kernel.bin | head -5