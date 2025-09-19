[org 0x7c00]
[bits 16]

KERNEL_START_SECTOR equ 1     ; O primeiro setor após o bootloader é o setor 1 (LBA 1).
KERNEL_SECTORS equ 200        ; Aumenta o número de setores para garantir que o kernel inteiro seja carregado.

; Mensagem de boas-vindas
msg db "Booting INTELI-Pond-MicroKernel...", 0x0d, 0x0a, 0

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Exibe a mensagem de boot
    mov si, msg
    call print_string

    ; Carrega o kernel
    mov al, KERNEL_SECTORS
    mov cl, KERNEL_START_SECTOR
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x1000
    mov ah, 0x02
    int 0x13
    
    ; Chama o ponto de entrada do kernel (em 0x1000:0)
    jmp 0x1000:0

print_string:
    mov ah, 0x0e
    .loop:
        lodsb
        or al, al
        jz .done
        int 0x10
        jmp .loop
    .done:
    ret

times 510-($-$$) db 0
dw 0xaa55