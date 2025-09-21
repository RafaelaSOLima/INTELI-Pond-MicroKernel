; src/kernel_entry.asm
bits 16
extern kmain
global _start

_start:
    ; Configura a máquina
    cli
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0xFFFE
    sti

    ; Chama a função principal do jogo em C
    call kmain

    ; Se o jogo terminar, trava a CPU
    cli
    hlt