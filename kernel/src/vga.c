#include "../include/vga.h"

unsigned short* vga_buffer = (unsigned short*) 0xB8000;
unsigned int vga_cursor_x = 0;
unsigned int vga_cursor_y = 0;

void print_char(char c) {
    if (c == '\n') {
        vga_cursor_x = 0;
        vga_cursor_y++;
        return;
    }
    vga_buffer[vga_cursor_y * VGA_WIDTH + vga_cursor_x] = (c | (VGA_COLOR << 8));
    vga_cursor_x++;
    if (vga_cursor_x >= VGA_WIDTH) {
        vga_cursor_x = 0;
        vga_cursor_y++;
    }
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}

void clear_screen(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (' ' | (VGA_COLOR << 8));
        }
    }
    vga_cursor_x = 0;
    vga_cursor_y = 0;
}