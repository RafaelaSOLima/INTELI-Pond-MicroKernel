#include "kernel.h"

// Variáveis para controle do terminal
static unsigned char* video_memory = (unsigned char*)VGA_MEMORY;
static int terminal_row = 0;
static int terminal_column = 0;
static unsigned char terminal_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);

// Declaração antecipada
void scroll_screen();

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = terminal_color;
    }
    terminal_row = 0;
    terminal_column = 0;
    set_cursor_position(0, 0);
}

void print_char(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else if (c == '\r') {
        terminal_column = 0;
    } else if (c == '\t') {
        terminal_column = (terminal_column + 8) & ~(8 - 1);
    } else if (c >= 32) {
        int index = (terminal_row * VGA_WIDTH + terminal_column) * 2;
        video_memory[index] = c;
        video_memory[index + 1] = terminal_color;
        terminal_column++;
    }
    
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    
    if (terminal_row >= VGA_HEIGHT) {
        scroll_screen();
        terminal_row = VGA_HEIGHT - 1;
    }
    
    set_cursor_position(terminal_column, terminal_row);
}

void print_string(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}

void print_number(int num) {
    if (num == 0) {
        print_char('0');
        return;
    }
    
    if (num < 0) {
        print_char('-');
        num = -num;
    }
    
    char buffer[32];
    int i = 0;
    
    while (num > 0) {
        buffer[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }
    
    // Imprimir em ordem reversa
    for (int j = i - 1; j >= 0; j--) {
        print_char(buffer[j]);
    }
}

void move_cursor_back() {
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    }
    set_cursor_position(terminal_column, terminal_row);
}

void set_cursor_position(int x, int y) {
    unsigned short position = y * VGA_WIDTH + x;
    
    // Enviar byte baixo
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    
    // Enviar byte alto
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void scroll_screen() {
    // Mover todas as linhas uma posição para cima
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++) {
        video_memory[i] = video_memory[i + VGA_WIDTH * 2];
    }
    
    // Limpar última linha
    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i < VGA_HEIGHT * VGA_WIDTH * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = terminal_color;
    }
}

void set_terminal_color(unsigned char color) {
    terminal_color = color;
}

// Função para testar se display está funcionando
void test_display() {
    print_string("TESTE: Display funcionando!\n");
}