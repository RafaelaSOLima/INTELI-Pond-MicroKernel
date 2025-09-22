#include "kernel.h"

// Tabela IDT e ponteiro
struct idt_entry idt[256];
struct idt_ptr idtp;

// Contador do timer
unsigned int timer_tick = 0;

// Mapa de teclas do teclado US
char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// Funções externas em assembly
extern void keyboard_handler();
extern void timer_handler();
extern void load_idt();

void setup_idt() {
    // Configurar ponteiro IDT
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (unsigned int)&idt;
    
    // Limpar IDT
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0, 0, 0);
    }
    
    // Configurar handlers
    set_idt_gate(0x21, (unsigned int)keyboard_handler, 0x08, 0x8E); // Teclado
    set_idt_gate(0x20, (unsigned int)timer_handler, 0x08, 0x8E);    // Timer
    
    // Carregar IDT
    load_idt(&idtp);
    
    // Configurar PIC (Programmable Interrupt Controller)
    setup_pic();
    
    // Habilitar interrupções
    asm volatile("sti");
}

void set_idt_gate(int n, unsigned int handler, unsigned short sel, unsigned char flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = sel;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void setup_pic() {
    // Reinicializar PICs
    outb(0x20, 0x11); // Inicializar PIC1
    outb(0xA0, 0x11); // Inicializar PIC2
    
    // Configurar offsets
    outb(0x21, 0x20); // PIC1 offset = 0x20
    outb(0xA1, 0x28); // PIC2 offset = 0x28
    
    // Configurar cascateamento
    outb(0x21, 0x04); // PIC1 tem escravo na IRQ2
    outb(0xA1, 0x02); // PIC2 é escravo do PIC1
    
    // Configurar modo 8086
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // Habilitar apenas timer e teclado
    outb(0x21, 0xFC); // Mask = 11111100 (habilita IRQ0 e IRQ1)
    outb(0xA1, 0xFF); // Desabilita todas IRQs do PIC2
}

void keyboard_handler_c() {
    unsigned char scancode = inb(0x60);
    
    // Verificar se é key press (bit 7 = 0)
    if (!(scancode & 0x80)) {
        char key = keymap[scancode];
        if (key != 0) {
            process_input(key);
        }
    }
}

void timer_handler_c() {
    timer_tick++;
}

unsigned int get_timer_tick() {
    return timer_tick;
}