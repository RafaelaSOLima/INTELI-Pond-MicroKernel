NASM = nasm

all: boot.bin

boot.bin: boot.asm
	$(NASM) -f bin $< -o $@

clean:
	rm -f *.bin