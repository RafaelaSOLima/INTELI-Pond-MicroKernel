[BITS 32]

extern keyboard_handler_c
extern timer_handler_c

global keyboard_handler
global timer_handler
global load_idt

; Handler de interrupção do teclado
keyboard_handler:
    pushad                  ; Salvar registradores
    call keyboard_handler_c ; Chamar função C
    mov al, 0x20           ; Enviar EOI (End of Interrupt)
    out 0x20, al           ; Para o PIC
    popad                  ; Restaurar registradores
    iret                   ; Retornar da interrupção

; Handler de interrupção do timer
timer_handler:
    pushad
    call timer_handler_c
    mov al, 0x20
    out 0x20, al
    popad
    iret

; Função para carregar IDT
load_idt:
    mov eax, [esp + 4]     ; Pegar ponteiro IDT do stack
    lidt [eax]             ; Carregar IDT
    ret