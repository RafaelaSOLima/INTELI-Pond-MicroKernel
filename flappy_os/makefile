# Makefile FINAL E DEFINITIVO

# Ferramentas
CC = gcc
NASM = nasm
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-i386

# Flags
CFLAGS = -m16 -c -ffreestanding -nostdlib -nostartfiles -O2 -Wall
LDFLAGS = -T src/linker.ld -m elf_i386 --nmagic
NASM_ELF_FLAGS = -f elf

# Arquivos
IMAGE = build/flappy_os.img

# Alvos que não representam arquivos
.PHONY: all run clean

# O alvo padrão 'all' agora simplesmente chama o 'run'
all: run

# O alvo 'run' depende da imagem e então executa o QEMU
run: $(IMAGE)
	@echo "==> Executando Flappy OS..."
	@$(QEMU) -M isapc -drive format=raw,file=$(IMAGE),index=0,if=floppy

# Regra para construir a imagem final
$(IMAGE): build/boot.bin build/kernel.bin
	@echo "--> Criando imagem final do sistema..."
	@cat build/boot.bin build/kernel.bin > $(IMAGE)

build/kernel.bin: build/kernel_entry.o build/kernel.o
	@echo "--> Linkando kernel (ASM+C) para um ELF..."
	@$(LD) $(LDFLAGS) -o build/kernel.elf build/kernel_entry.o build/kernel.o
	@echo "--> Extraindo binário puro com objcopy..."
	@$(OBJCOPY) -O binary build/kernel.elf build/kernel.bin

build/kernel.o: src/kernel.c
	@echo "--> Compilando o kernel (C)..."
	@$(CC) $(CFLAGS) src/kernel.c -o build/kernel.o

build/kernel_entry.o: src/kernel_entry.asm
	@echo "--> Montando a entrada do kernel (ASM)..."
	@$(NASM) $(NASM_ELF_FLAGS) src/kernel_entry.asm -o build/kernel_entry.o

build/boot.bin: src/boot.asm
	@echo "--> Montando bootloader..."
	@mkdir -p build
	@$(NASM) -f bin src/boot.asm -o build/boot.bin

clean:
	@echo "==> Limpando a pasta de build..."
	@rm -rf build