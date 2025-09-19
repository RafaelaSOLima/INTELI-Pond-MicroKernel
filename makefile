MAKE = make
QEMU = qemu-system-x86_64
NASM = nasm
GCC = gcc
LD = ld

BOOT_DIR = boot
KERNEL_DIR = kernel
TOOLS_DIR = tools
BUILD_DIR = build

# Flags de compilação
GCC_FLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I$(KERNEL_DIR)/include

# Arquivos
BOOT_BIN = $(BOOT_DIR)/boot.bin
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMG = $(BUILD_DIR)/os.img

.PHONY: all run clean

all: $(OS_IMG)

# Compila o bootloader e o kernel
$(BOOT_BIN):
	$(MAKE) -C $(BOOT_DIR)

$(KERNEL_BIN): $(BOOT_BIN)
	@echo "Compiling kernel..."
	mkdir -p $(BUILD_DIR)
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/entry.S -o $(BUILD_DIR)/entry.o
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/src/kernel.c -o $(BUILD_DIR)/kernel.o
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/src/vga.c -o $(BUILD_DIR)/vga.o
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/src/isr.c -o $(BUILD_DIR)/isr.o
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/src/irq.c -o $(BUILD_DIR)/irq.o
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_DIR)/src/game.c -o $(BUILD_DIR)/game.o
	$(LD) -melf_i386 -T $(KERNEL_DIR)/linker.ld -o $(KERNEL_BIN) \
	  $(BUILD_DIR)/entry.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/vga.o \
	  $(BUILD_DIR)/isr.o $(BUILD_DIR)/irq.o $(BUILD_DIR)/game.o

# Cria a imagem do sistema
$(OS_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Creating disk image..."
	$(TOOLS_DIR)/mkimage.sh $(BOOT_BIN) $(KERNEL_BIN) $(OS_IMG)

run:
	@echo "Running on QEMU..."
	$(MAKE) all
	$(TOOLS_DIR)/run_qemu.sh $(OS_IMG)
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(BOOT_DIR) clean