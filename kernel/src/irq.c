#include "../include/irq.h"
#include "../include/vga.h"

// Scancodes do teclado
#define KEYCODE_SPACE 0x39
void handle_game_input(void);

// Funcao de manipulador de IRQ1 (teclado)
void keyboard_handler(void) {
    unsigned char scancode;
    
    // Le o scancode do teclado
    scancode = inb(0x60);

    // Se a tecla for a barra de espaco
    if (scancode == KEYCODE_SPACE) {
        handle_game_input();
    }

    // Acknowledge para o PIC
    outb(0x20, 0x20); // Master PIC
}