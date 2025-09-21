/**
 * src/kernel.c - FLAPPY BIRD OS - VERSÃO FINAL CORRIGIDA
 */
volatile unsigned char* video_memory = (unsigned char*)0xb8000;
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define ATTR_BIRD 0x0E
#define ATTR_PIPE 0x0A
#define ATTR_TEXT 0x0F

void print_char(char c, int x, int y, char attr) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    unsigned int offset = y;
    offset *= SCREEN_WIDTH; // Esta multiplicação por 80 (constante) é otimizada pelo GCC para shifts/adds seguros
    offset += x;
    offset *= 2;
    video_memory[offset] = c;
    video_memory[offset + 1] = attr;
}
void clear_screen() { for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) { video_memory[i * 2] = ' '; video_memory[i * 2 + 1] = ATTR_TEXT; } }
void print_string(const char* str, int x, int y, char attr) { int i = 0; while (str[i] != 0) { print_char(str[i], x + i, y, attr); i++; } }
char get_key() { char key; __asm__ __volatile__("mov $0x00, %%ah\n\t int $0x16\n\t mov %%al, %0\n\t" : "=r"(key) : : "ax"); return key; }
void delay() { for (volatile long i = 0; i < 4000000L; i++); }

int bird_y, bird_velocity, pipe_x[2], pipe_gap_y[2], score, game_over;
unsigned int rand_seed = 1;

#define BIRD_X 10
#define JUMP_VELOCITY -2
#define GRAVITY 1
#define PIPE_GAP 7
#define NUM_PIPES 2

unsigned int rand() {
    unsigned short lsb = rand_seed & 1; rand_seed >>= 1; if (lsb) { rand_seed ^= 0xB400u; } return rand_seed;
}
void init_pipes() {
    pipe_x[0] = SCREEN_WIDTH + 5; pipe_gap_y[0] = 5 + (rand() & 7);
    pipe_x[1] = SCREEN_WIDTH + 45; pipe_gap_y[1] = 5 + (rand() & 7);
}
void draw() {
    clear_screen();
    print_char('>', BIRD_X, bird_y, ATTR_BIRD);
    for (int i = 0; i < NUM_PIPES; i++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) { if (y < pipe_gap_y[i] || y > pipe_gap_y[i] + PIPE_GAP) { print_char('#', pipe_x[i], y, ATTR_PIPE); } }
    }
    char score_str[12] = "Score: ";
    if (score >= 0 && score <= 9) { score_str[7] = '0' + score; score_str[8] = '\0'; }
    else { score_str[7] = '9'; score_str[8] = '+'; score_str[9] = '\0'; }
    print_string(score_str, 0, 0, ATTR_TEXT);
}
void update() { __asm__ __volatile__ ( "mov $0x01, %%ah\n\t int $0x16\n\t jz 1f\n\t mov $0x00, %%ah\n\t int $0x16\n\t" "cmp $0x39, %%al\n\t jne 1f\n\t movw %1, %0\n\t 1:" : "=m"(bird_velocity) : "i"(JUMP_VELOCITY) : "ax" ); bird_velocity += GRAVITY; bird_y += bird_velocity; for (int i = 0; i < NUM_PIPES; i++) { pipe_x[i]--; if (pipe_x[i] < -1) { pipe_x[i] = SCREEN_WIDTH; pipe_gap_y[i] = 5 + (rand() & 7); } if (pipe_x[i] == BIRD_X - 1) { score++; } } if (bird_y < 0 || bird_y >= SCREEN_HEIGHT) { game_over = 1; } for (int i = 0; i < NUM_PIPES; i++) { if (pipe_x[i] == BIRD_X) { if (bird_y < pipe_gap_y[i] || bird_y > pipe_gap_y[i] + PIPE_GAP) { game_over = 1; } } } }

void kmain() {
game_start:
    bird_y = SCREEN_HEIGHT / 2; bird_velocity = 0; score = 0; game_over = 0; rand_seed = 1;
    init_pipes();
    while (!game_over) { draw(); update(); delay(); }
    clear_screen();
    print_string("Game Over", 35, 11, ATTR_TEXT);
    char final_score_str[16] = "Score Final: ";
    if (score >= 0 && score <= 9) { final_score_str[13] = '0' + score; final_score_str[14] = '\0'; }
    else { final_score_str[13] = '9'; final_score_str[14] = '+'; final_score_str[15] = '\0'; }
    print_string(final_score_str, 32, 12, ATTR_TEXT);
    print_string("Pressione qualquer tecla para reiniciar.", 24, 14, ATTR_TEXT);
    get_key();
    goto game_start;
}