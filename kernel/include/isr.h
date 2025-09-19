#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Estrutura para a entrada da IDT
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

// Estrutura para o ponteiro da IDT
struct idt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_interrupts(void);
void set_idt_gate(int num, uint32_t base);

#endif