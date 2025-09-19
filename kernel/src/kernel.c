#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/isr.h"

void kernel_main(void) {
    clear_screen();
    print_string("INTELI-Pond-MicroKernel booted successfully!");

    // Inicializa as interrupcoes
    init_interrupts();

    // Inicia o jogo
    start_game();
}