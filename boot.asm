; boot.asm - Bootloader simplificado
[BITS 16]
[ORG 0x7C00]

start:
    ; Configurar segmentos
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Carregar kernel (mais setores)
    mov bx, 0x8000          ; Carregar em 0x8000 (endereço mais alto)
    mov ah, 0x02            ; Função leitura
    mov al, 20              ; 20 setores (mais espaço)
    mov ch, 0               ; Cilindro 0
    mov cl, 2               ; Setor 2
    mov dh, 0               ; Cabeça 0
    mov dl, 0x00            ; Drive 0
    int 0x13
    
    jc disk_error

    ; Entrar em modo protegido
    lgdt [gdt_descriptor]
    
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Salto para modo protegido
    jmp 0x08:protected_mode

disk_error:
    mov si, error_msg
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    cli
    hlt

[BITS 32]
protected_mode:
    ; Configurar segmentos
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Configurar stack
    mov esp, 0x90000
    
    ; Saltar para kernel
    jmp 0x8000

; GDT
gdt_start:
gdt_null:
    dd 0x0, 0x0

gdt_code:
    dw 0xFFFF       ; Limite baixo
    dw 0x0          ; Base baixa
    db 0x0          ; Base média
    db 10011010b    ; Flags acesso
    db 11001111b    ; Granularidade
    db 0x0          ; Base alta

gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

error_msg db 'Boot Error!', 0

times 510-($-$$) db 0
dw 0xAA55