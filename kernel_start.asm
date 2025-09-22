[BITS 32]

global _start
extern kernel_main

section .text
_start:
    ; Configurar segmentos
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Configurar stack
    mov esp, 0x90000
    
    ; Chamar função principal do kernel em C
    call kernel_main
    
    ; Loop infinito caso retorne
hang:
    cli
    hlt
    jmp hang