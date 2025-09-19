.section .boot
.global _start
.global load_idt
.global irq1

_start:
    # Configura o modo protegido
    cli
    lgdt gdtr
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0
    ljmp $0x08, $flush

flush:
    .code32
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $0x90000, %esp # Stack do kernel

    # Chama a função principal do kernel
    call kernel_main

    # Loop infinito em caso de retorno
    jmp .

# Função para carregar a IDT
load_idt:
    movl 4(%esp), %eax # Pega o endereço do ponteiro da IDT
    lidt (%eax)        # Carrega a IDT usando a instrução 'lidt'
    ret

# Manipulador de interrupção IRQ1 (Teclado)
irq1:
    pusha                  # Salva o estado dos registradores
    call keyboard_handler  # Chama o manipulador em C
    popa                   # Restaura o estado dos registradores
    iret                   # Retorna da interrupção

.align 4
gdt_start:
    .quad 0 # Segmento nulo
gdt_code:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0x9a # Acesso, Read/Execute, Present
    .byte 0xcf # Granularidade 4K, 32-bit
    .byte 0x00
gdt_data:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0x92 # Acesso, Read/Write, Present
    .byte 0xcf # Granularidade 4K, 32-bit
    .byte 0x00
gdt_end:

gdtr:
    .word gdt_end - gdt_start - 1
    .long gdt_start