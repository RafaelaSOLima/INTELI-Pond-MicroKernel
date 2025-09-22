; bootloader.asm
; Bootloader de 16-bit para carregar e iniciar um kernel de 32-bit

org 0x7c00
bits 16

start:
    ; Configura os registradores de segmento
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    
    ; Desabilita interrupções
    cli

    ; Carrega o GDT (tabela de descritores globais)
    lgdt [gdt_descriptor]

    ; Habilita a linha de endereço A20 (importante para memória acima de 1MB)
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Habilita o modo protegido
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Agora em modo protegido, salta para o código de 32 bits
    jmp 0x8:init_32bit

bits 32

init_32bit:
    ; Configura os registradores de segmento para 32 bits
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000

    ; Carrega o kernel do disco (a partir do setor 2)
    mov ah, 0x02        ; Função de leitura do disco
    mov al, 4           ; Número de setores para ler
    mov ch, 0           ; Cilindro 0
    mov cl, 2           ; Começa a ler do setor 2
    mov dh, 0           ; Cabeçote 0
    mov dl, 0x80        ; Drive de disco (primeiro disco rígido)
    mov bx, 0x10000     ; Endereço de destino
    int 0x13            ; Chamada de interrupção da BIOS para leitura do disco
    
    ; Salta para a função principal do kernel
    jmp 0x10000
    
; GDT e descritores
gdt_start:
    ; Descritor nulo
    dq 0

    ; Descritor para o segmento de código (CS)
    dw 0xffff, 0, 0x9a00, 0xcf
    
    ; Descritor para o segmento de dados (DS, ES, SS, etc.)
    dw 0xffff, 0, 0x9200, 0xcf

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55