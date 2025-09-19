#include "../include/kernel.h"
#include "../include/vga.h"

// Constantes do jogo
#define BIRD_CHAR 'O'
#define PIPE_CHAR '#'
#define BIRD_START_Y 12

// Variaveis de estado do jogo
int bird_y;
int bird_velocity;
int pipe_x[2];
int pipe_gap_y[2];
int score;
bool game_over;
unsigned long ticks;

void update_game_state(void);
void render_game(void);
void reset_game(void);

void start_game(void) {
    reset_game();
    while(!game_over) {
        update_game_state();
        render_game();
        for(volatile int i = 0; i < 1000000; i++); 
        ticks++;
    }
    clear_screen();
    print_string("Game Over! Final Score: ");
}

void reset_game(void) {
    bird_y = BIRD_START_Y;
    bird_velocity = 0;
    pipe_x[0] = VGA_WIDTH;
    pipe_gap_y[0] = 5;
    pipe_x[1] = VGA_WIDTH + VGA_WIDTH/2;
    pipe_gap_y[1] = 15;
    score = 0;
    game_over = false;
    ticks = 0;
}

void update_game_state(void) {
    bird_velocity++;
    bird_y += bird_velocity;

    pipe_x[0]--;
    pipe_x[1]--;

    if (pipe_x[0] < 0) {
        pipe_x[0] = VGA_WIDTH;
        pipe_gap_y[0] = (ticks % (VGA_HEIGHT - 10)) + 5;
    }
    if (pipe_x[1] < 0) {
        pipe_x[1] = VGA_WIDTH;
        pipe_gap_y[1] = (ticks % (VGA_HEIGHT - 10)) + 5;
    }

    if (bird_y < 0 || bird_y >= VGA_HEIGHT) {
        game_over = true;
    }
}

void render_game(void) {
    clear_screen();
    vga_buffer[bird_y * VGA_WIDTH + 10] = (BIRD_CHAR | (VGA_COLOR << 8));
    for (int i = 0; i < VGA_HEIGHT; i++) {
        if (i < pipe_gap_y[0] || i > pipe_gap_y[0] + 5) {
            vga_buffer[i * VGA_WIDTH + pipe_x[0]] = (PIPE_CHAR | (VGA_COLOR << 8));
        }
        if (i < pipe_gap_y[1] || i > pipe_gap_y[1] + 5) {
            vga_buffer[i * VGA_WIDTH + pipe_x[1]] = (PIPE_CHAR | (VGA_COLOR << 8));
        }
    }
}

void handle_game_input(void) {
    bird_velocity = -2; // Salto
}