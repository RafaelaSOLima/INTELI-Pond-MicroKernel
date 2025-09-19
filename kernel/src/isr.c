#include "../include/isr.h"
#include "../include/irq.h"
#include "../include/vga.h"

struct idt_entry idt_entries[256];
struct idt_pointer idt_ptr;

// Declarações de funções externas (em entry.S)
extern void isr0();
extern void irq0();
extern void irq1();
extern void load_idt(uint32_t idt_ptr);

// Funções de I/O
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "dN"(port));
}

// Configura uma entrada na IDT
void set_idt_gate(int num, uint32_t base) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].selector = 0x08; // Seletor de segmento de código
    idt_entries[num].zero = 0;
    idt_entries[num].flags = 0x8E; // Flags para interrupção de 32-bit
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
}

// Inicializa o PIC e a IDT
void init_interrupts(void) {
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Reprograma o PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // Mapeia IRQ0-7 para 0x20-0x27
    outb(0xA1, 0x28); // Mapeia IRQ8-15 para 0x28-0x2F
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
    
    // Configura a entrada do teclado (IRQ1)
    set_idt_gate(33, (uint32_t)irq1);

    // Carrega a IDT
    load_idt((uint32_t)&idt_ptr);
    
    // Habilita as interrupções
    asm volatile("sti");
}