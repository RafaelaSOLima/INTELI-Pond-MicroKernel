#ifndef KERNEL_H
#define KERNEL_H

// Definições de cores VGA
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// Constantes do sistema
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Estrutura para entrada da IDT
struct idt_entry {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed));

// Ponteiro para IDT
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Variáveis globais do sistema
extern struct idt_entry idt[256];
extern struct idt_ptr idtp;
extern unsigned int timer_tick;

// Funções principais do kernel
void kernel_main();
void show_welcome_screen();
void init_game();
void start_new_game();
void process_input(char c);
void process_guess();
void show_win_message();
void show_lose_message();
void clear_input_buffer();
int string_to_int(char* str);

// Funções de sistema
void setup_idt();
void set_idt_gate(int n, unsigned int handler, unsigned short sel, unsigned char flags);
void setup_pic();
void keyboard_handler_c();
void timer_handler_c();
unsigned int get_timer_tick();

// Funções de display
void clear_screen();
void print_char(char c);
void print_string(char* str);
void print_number(int num);
void move_cursor_back();
void set_cursor_position(int x, int y);
void scroll_screen();

// Funções de utilidade
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

#endif