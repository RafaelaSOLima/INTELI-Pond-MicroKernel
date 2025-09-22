[BITS 32]

; Kernel simples que inicia em 0x8000
ORG 0x8000

start:
    ; Configurar segmentos
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    
    ; Limpar tela
    mov edi, 0xB8000        ; Buffer VGA
    mov ecx, 80*25          ; 80 colunas x 25 linhas
    mov ax, 0x0F20          ; Espaço branco no fundo preto
    rep stosw
    
    ; Escrever mensagem de teste
    mov edi, 0xB8000        ; Início da tela
    mov esi, test_message
    mov ah, 0x0F            ; Texto branco no fundo preto
    
print_loop:
    lodsb                   ; Carregar próximo caractere
    test al, al             ; Verificar se é zero
    jz setup_game          ; Se zero, ir para jogo
    
    stosb                   ; Escrever caractere
    mov al, ah             ; Escrever atributo
    stosb
    jmp print_loop

setup_game:
    ; Inicializar variáveis do jogo
    mov dword [secret_number], 42  ; Número fixo para teste
    mov dword [attempts], 0
    mov dword [game_over], 0
    
    ; Definir posição inicial do cursor após o prompt
    mov edi, 0xB8000 + (3*80*2) + (27*2)  ; Linha 3 + 27 caracteres do prompt
    mov [cursor_pos], edi
    
    ; Escrever prompt inicial
    mov edi, 0xB8000 + (3*80*2)   ; Linha 3
    mov esi, game_prompt
    mov ah, 0x0E                   ; Texto amarelo
    
prompt_loop:
    lodsb
    test al, al
    jz main_loop
    stosb
    mov al, ah
    stosb
    jmp prompt_loop

main_loop:
    ; Verificar se há tecla pressionada (polling simples)
    in al, 0x64        ; Status do teclado
    test al, 1         ; Verificar se há dados
    jz main_loop       ; Se não, continuar loop
    
    ; Ler scancode do teclado
    in al, 0x60        ; Ler scancode
    
    ; Verificar se é tecla pressionada (não release)
    test al, 0x80      ; Bit 7 = 1 significa release
    jnz main_loop      ; Ignorar release
    
    ; Converter algumas teclas básicas
    cmp al, 0x1C       ; Enter
    je handle_enter
    
    ; Números 1-9 e 0
    cmp al, 0x02       ; '1'
    je handle_1
    cmp al, 0x03       ; '2'
    je handle_2
    cmp al, 0x04       ; '3'
    je handle_3
    cmp al, 0x05       ; '4'
    je handle_4
    cmp al, 0x06       ; '5'
    je handle_5
    cmp al, 0x07       ; '6'
    je handle_6
    cmp al, 0x08       ; '7'
    je handle_7
    cmp al, 0x09       ; '8'
    je handle_8
    cmp al, 0x0A       ; '9'
    je handle_9
    cmp al, 0x0B       ; '0'
    je handle_0
    
    cmp al, 0x0E       ; Backspace
    je handle_backspace
    
    ; Ignorar outras teclas
    jmp main_loop

handle_0:
    mov al, '0'
    call print_char
    jmp main_loop

handle_1:
    mov al, '1'
    call print_char
    jmp main_loop

handle_2:
    mov al, '2'
    call print_char
    jmp main_loop

handle_3:
    mov al, '3'
    call print_char
    jmp main_loop

handle_4:
    mov al, '4'
    call print_char
    jmp main_loop

handle_5:
    mov al, '5'
    call print_char
    jmp main_loop

handle_6:
    mov al, '6'
    call print_char
    jmp main_loop

handle_7:
    mov al, '7'
    call print_char
    jmp main_loop

handle_8:
    mov al, '8'
    call print_char
    jmp main_loop

handle_9:
    mov al, '9'
    call print_char
    jmp main_loop

handle_backspace:
    ; Voltar um caractere
    mov edi, [cursor_pos]
    cmp edi, 0xB8000 + (3*80*2) + (27*2)  ; Não apagar além do prompt
    jle main_loop
    sub edi, 2         ; Voltar um caractere (char + atributo)
    mov [cursor_pos], edi
    mov byte [edi], ' '    ; Escrever espaço
    mov byte [edi+1], 0x0F ; Atributo branco
    jmp main_loop

handle_enter:
    ; Quebrar linha
    mov al, 0x0A       ; Line Feed
    call print_char
    mov al, 0x0D       ; Carriage Return
    call print_char
    
    ; Reiniciar prompt
    mov esi, game_prompt
    mov ah, 0x0E
print_prompt:
    lodsb
    test al, al
    jz main_loop
    call print_char
    jmp print_prompt

; Função para imprimir um caractere
print_char:
    push edi
    push eax
    
    ; Encontrar posição atual do cursor
    mov edi, [cursor_pos]
    
    ; Verificar se é nova linha
    cmp al, 0x0A
    je newline
    cmp al, 0x0D
    je carriage_return
    
    ; Escrever caractere normal
    stosb              ; Escrever caractere
    mov al, 0x0F       ; Atributo (branco)
    stosb              ; Escrever atributo
    
    ; Atualizar posição do cursor
    mov [cursor_pos], edi
    jmp print_char_end

newline:
    ; Mover para próxima linha
    mov eax, edi
    sub eax, 0xB8000   ; Offset do buffer VGA
    shr eax, 1         ; Dividir por 2 (char + atributo)
    mov edx, 0
    mov ecx, 80        ; 80 colunas
    div ecx            ; EAX = linha atual
    inc eax            ; Próxima linha
    mul ecx            ; EAX = offset da nova linha
    shl eax, 1         ; Multiplicar por 2
    add eax, 0xB8000   ; Adicionar base VGA
    mov [cursor_pos], eax
    jmp print_char_end

carriage_return:
    ; Voltar ao início da linha
    mov eax, edi
    sub eax, 0xB8000
    shr eax, 1
    mov edx, 0
    mov ecx, 80
    div ecx            ; EAX = linha, EDX = coluna
    mul ecx            ; EAX = offset do início da linha
    shl eax, 1
    add eax, 0xB8000
    mov [cursor_pos], eax

print_char_end:
    pop eax
    pop edi
    ret

; Dados
test_message db '=== MICRO KERNEL FUNCIONANDO! ===', 0
game_prompt db 'Adivinhe o numero (1-100): ', 0

; Variáveis do jogo
secret_number dd 42
attempts dd 0
game_over dd 0
cursor_pos dd 0xB8000  ; Posição inicial do cursor

times 1024-($-$) db 0  ; Preencher até 1KB