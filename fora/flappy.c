#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Bloco de código para compatibilidade e gotoxy ---
#ifdef _WIN32
#include <windows.h>
#include <conio.h>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void hideCursor() {
    printf("\033[?25l");
}

// Funções para simular kbhit() e getch() no Linux/macOS (mesmas de antes)
int kbhit(void) {
    struct termios oldt, newt; int ch; int oldf;
    tcgetattr(STDIN_FILENO, &oldt); newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) { ungetc(ch, stdin); return 1; }
    return 0;
}
int getch() {
    struct termios oldt, newt; int ch;
    tcgetattr(STDIN_FILENO, &oldt); newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Funções universais
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}
void sleepMs(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}
// --- Fim do bloco de compatibilidade ---


// --- Constantes do Jogo ---
#define LARGURA 40
#define ALTURA 20
#define GAP_CANO 6
#define POSICAO_X_PASSARO 5

// --- Estruturas e Variáveis Globais ---
typedef struct { double y, velocidade; } Passaro;
typedef struct { int x, buraco_y; } Cano;

Passaro passaro;
Cano canos[2];
int pontuacao;
int gameOver;

// Buffer da tela para desenho inteligente
char tela_atual[ALTURA][LARGURA + 1];
char tela_anterior[ALTURA][LARGURA + 1];

// --- Funções do Jogo ---
void setup() {
    passaro.y = ALTURA / 2.0;
    passaro.velocidade = 0;
    pontuacao = 0;
    gameOver = 0;
    srand(time(NULL));

    for (int i = 0; i < 2; i++) {
        canos[i].x = LARGURA + i * (LARGURA / 2);
        canos[i].buraco_y = rand() % (ALTURA - GAP_CANO - 4) + 2;
    }
    
    // Inicializa os buffers da tela
    for(int y=0; y < ALTURA; y++) {
        for (int x=0; x < LARGURA; x++) {
            tela_atual[y][x] = ' ';
            tela_anterior[y][x] = ' ';
        }
        tela_atual[y][LARGURA] = '\0';
        tela_anterior[y][LARGURA] = '\0';
    }
}

void draw() {
    // 1. Constrói o estado do próximo frame no buffer `tela_atual`
    // Limpa o buffer atual
    for(int y=0; y < ALTURA; y++) for(int x=0; x < LARGURA; x++) tela_atual[y][x] = ' ';

    // Desenha o pássaro no buffer
    int passaro_y_int = (int)passaro.y;
    if (passaro_y_int >= 0 && passaro_y_int < ALTURA) {
        tela_atual[passaro_y_int][POSICAO_X_PASSARO] = 'O';
        tela_atual[passaro_y_int][POSICAO_X_PASSARO+1] = '>';
    }

    // Desenha os canos no buffer
    for (int i = 0; i < 2; i++) {
        for (int y = 0; y < ALTURA; y++) {
            if (canos[i].x >= 0 && canos[i].x < LARGURA) {
                if (y < canos[i].buraco_y || y >= canos[i].buraco_y + GAP_CANO) {
                    tela_atual[y][canos[i].x] = '#';
                }
            }
        }
    }

    // 2. Compara `tela_atual` com `tela_anterior` e desenha apenas as diferenças
    for(int y=0; y < ALTURA; y++) {
        for(int x=0; x < LARGURA; x++) {
            if(tela_atual[y][x] != tela_anterior[y][x]) {
                gotoxy(x, y);
                putchar(tela_atual[y][x]);
            }
        }
    }

    // 3. Atualiza a pontuação
    gotoxy(0, ALTURA);
    printf("Pontuacao: %d\n", pontuacao);

    // 4. Copia o buffer atual para o anterior para o próximo ciclo
    for(int y=0; y < ALTURA; y++) {
        for(int x=0; x < LARGURA; x++) {
            tela_anterior[y][x] = tela_atual[y][x];
        }
    }
}

void input() {
    if (kbhit()) {
        char tecla = getch();
        if (tecla == ' ' || tecla == 'w' || tecla == 'W') {
            passaro.velocidade = -0.75;
        } else if (tecla == 'q' || tecla == 'Q') { // Adiciona uma tecla para sair
            gameOver = 1;
        }
    }
}

void update() {
    passaro.velocidade += 0.15;
    passaro.y += passaro.velocidade;

    for (int i = 0; i < 2; i++) {
        canos[i].x--;
        if (canos[i].x < -1) {
            canos[i].x = LARGURA - 1;
            canos[i].buraco_y = rand() % (ALTURA - GAP_CANO - 4) + 2;
        }
        if (canos[i].x == POSICAO_X_PASSARO) {
            pontuacao++;
        }
    }
    
    if (passaro.y >= ALTURA - 1 || passaro.y < 0) {
        gameOver = 1;
    }
    
    for (int i = 0; i < 2; i++) {
        if (canos[i].x == POSICAO_X_PASSARO || canos[i].x == POSICAO_X_PASSARO - 1) {
            if ((int)passaro.y < canos[i].buraco_y || (int)passaro.y >= canos[i].buraco_y + GAP_CANO) {
                gameOver = 1;
            }
        }
    }
}

int main() {
    clearScreen();
    hideCursor();
    setup();

    while (!gameOver) {
        input();
        update();
        draw();
        // Aumentei o tempo de sleep, pois o desenho mais rápido faz o jogo rodar mais rápido
        sleepMs(30); 
    }
    
    // Posiciona o cursor no final para a mensagem de Game Over
    gotoxy(0, ALTURA + 2);
    printf("\n--- GAME OVER ---\n");
    printf("Pontuacao Final: %d\n\n", pontuacao);
    
    // Restaura o cursor (importante no linux)
    #ifndef _WIN32
        printf("\033[?25h"); 
    #endif

    return 0;
}