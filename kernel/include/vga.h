#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0F // Branco em fundo preto

extern unsigned short* vga_buffer;

void print_char(char c);
void print_string(const char* str);
void clear_screen(void);

#endif