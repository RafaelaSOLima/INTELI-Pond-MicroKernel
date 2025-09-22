#include "kernel.h"

// Variáveis globais do jogo
static int secret_number;
static int attempts;
static int max_attempts = 7;
static int game_won = 0;
static int game_over = 0;
static char input_buffer[10];
static int input_index = 0;

// Função principal do kernel
void kernel_main() {
    // Teste básico de display primeiro
    clear_screen();
    print_string("=== MICRO KERNEL INICIALIZANDO ===\n\n");
    print_string("1. Display: OK\n");
    
    // Inicializar sistema de interrupções
    print_string("2. Configurando interrupcoes...\n");
    setup_idt();
    print_string("3. Interrupcoes: OK\n\n");
    
    // Mostrar tela de boas-vindas
    show_welcome_screen();
    
    // Inicializar jogo
    init_game();
    
    print_string("4. Jogo inicializado!\n");
    print_string("5. Sistema pronto!\n\n");
    
    // Loop principal do kernel
    while (1) {
        // O sistema responde às interrupções do teclado
        // A lógica do jogo é executada nas funções de tratamento de input
        asm volatile("hlt"); // Parar CPU até próxima interrupção
    }
}

void show_welcome_screen() {
    print_string("===============================================\n");
    print_string("       BEM-VINDO AO MICRO KERNEL v1.0\n");
    print_string("===============================================\n\n");
    print_string("JOGO: ADIVINHE O NUMERO\n\n");
    print_string("Regras:\n");
    print_string("- Vou pensar em um numero de 1 a 100\n");
    print_string("- Voce tem 7 tentativas para acertar\n");
    print_string("- Digite seu palpite e pressione ENTER\n");
    print_string("- Use BACKSPACE para corrigir\n\n");
    print_string("Pressione qualquer tecla para comecar...\n");
}

void init_game() {
    // Gerador de número pseudo-aleatório simples
    // Usa timestamp do timer para seed
    secret_number = (get_timer_tick() % 100) + 1;
    attempts = 0;
    game_won = 0;
    game_over = 0;
    input_index = 0;
    
    clear_input_buffer();
}

void start_new_game() {
    clear_screen();
    print_string("===============================================\n");
    print_string("           ADIVINHE O NUMERO!\n");
    print_string("===============================================\n\n");
    print_string("Pensei em um numero de 1 a 100!\n");
    print_string("Tentativas restantes: ");
    print_number(max_attempts - attempts);
    print_string("\n\nDigite seu palpite: ");
}

void process_input(char c) {
    if (game_over && c != '\n') {
        return;
    }
    
    if (c == '\n' || c == '\r') { // Enter
        if (input_index == 0) {
            if (!game_over) {
                start_new_game();
            }
            return;
        }
        
        if (game_over) {
            // Reiniciar jogo
            init_game();
            start_new_game();
            return;
        }
        
        process_guess();
        
    } else if (c == '\b') { // Backspace
        if (input_index > 0 && !game_over) {
            input_index--;
            input_buffer[input_index] = '\0';
            
            // Apagar último caractere na tela
            move_cursor_back();
            print_char(' ');
            move_cursor_back();
        }
        
    } else if (c >= '0' && c <= '9' && input_index < 3 && !game_over) {
        // Aceitar apenas dígitos
        input_buffer[input_index] = c;
        input_index++;
        input_buffer[input_index] = '\0';
        print_char(c);
    }
}

void process_guess() {
    int guess = string_to_int(input_buffer);
    
    if (guess < 1 || guess > 100) {
        print_string("\n\nNumero invalido! Digite entre 1 e 100.\n");
        print_string("Digite seu palpite: ");
        clear_input_buffer();
        return;
    }
    
    attempts++;
    clear_input_buffer();
    
    print_string("\n\nVoce digitou: ");
    print_number(guess);
    print_string("\n");
    
    if (guess == secret_number) {
        game_won = 1;
        game_over = 1;
        show_win_message();
    } else if (attempts >= max_attempts) {
        game_over = 1;
        show_lose_message();
    } else {
        if (guess < secret_number) {
            print_string("Muito baixo! Tente um numero maior.\n");
        } else {
            print_string("Muito alto! Tente um numero menor.\n");
        }
        
        print_string("\nTentativas restantes: ");
        print_number(max_attempts - attempts);
        print_string("\nDigite seu palpite: ");
    }
}

void show_win_message() {
    print_string("\n===============================================\n");
    print_string("          PARABENS! VOCE ACERTOU!\n");
    print_string("===============================================\n\n");
    print_string("O numero era: ");
    print_number(secret_number);
    print_string("\nTentativas usadas: ");
    print_number(attempts);
    print_string("/");
    print_number(max_attempts);
    print_string("\n\nPressione ENTER para jogar novamente...\n");
}

void show_lose_message() {
    print_string("\n===============================================\n");
    print_string("            GAME OVER!\n");
    print_string("===============================================\n\n");
    print_string("O numero era: ");
    print_number(secret_number);
    print_string("\nVoce usou todas as ");
    print_number(max_attempts);
    print_string(" tentativas.\n");
    print_string("\nPressione ENTER para tentar novamente...\n");
}

void clear_input_buffer() {
    for (int i = 0; i < 10; i++) {
        input_buffer[i] = '\0';
    }
    input_index = 0;
}

int string_to_int(char* str) {
    int result = 0;
    int i = 0;
    
    while (str[i] != '\0' && i < 10) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
        i++;
    }
    
    return result;
}