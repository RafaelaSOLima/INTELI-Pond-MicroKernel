; src/boot.asm (VERSÃO FINAL)
org 0x7c00
bits 16

start:
    jmp main

main:
    ; Configura ambiente seguro
    xor ax, ax
    mov ds, ax
    mov es, ax
    cli
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov [boot_drive], dl

    mov ah, 0x00 ; Reseta o disco
    int 0x13

    mov cx, 3
read_loop:
    push cx

    mov ax, 0x1000      ; Segmento de destino
    mov es, ax
    xor bx, bx

    mov ah, 0x02        ; Função de leitura
    mov al, 15          ; Ler 15 setores
    mov ch, 0           ; Cilindro 0
    mov cl, 2           ; Começando no setor 2
    mov dh, 0           ; Cabeça 0
    mov dl, [boot_drive]
    int 0x13
    jnc read_success

    pop cx
    loop read_loop
    jmp error_hang

read_success:
    pop cx
    jmp 0x1000:0x0000

error_hang:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
hang:
    cli
    hlt

boot_drive: db 0
times 510-($-$$) db 0
dw 0xAA55