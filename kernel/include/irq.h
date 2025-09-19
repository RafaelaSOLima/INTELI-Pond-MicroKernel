#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

void keyboard_handler(void);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

#endif