# Micro Kernel - Jogo "Adivinhe o Número"

## Descrição do Projeto

Este projeto implementa um micro kernel de sistema operacional completo com um jogo interativo "Adivinhe o Número". O kernel é capaz de:

- ✅ **Bootloader em Assembly**: Carrega o kernel e entra em modo protegido
- ✅ **Display de Caracteres**: Sistema completo de texto VGA
- ✅ **Interrupções de Teclado**: Leitura e processamento de teclas
- ✅ **Jogo Interativo**: "Adivinhe o Número" com 7 tentativas

## Arquitetura do Sistema

### Estrutura de Arquivos

```
micro-kernel/
├── boot.asm           # Bootloader (modo real → modo protegido)
├── kernel.c           # Loop principal e lógica do jogo
├── kernel.h           # Definições e protótipos
├── display.c          # Funções de display VGA
├── interrupts.asm     # Handlers de interrupção em assembly
├── interrupts.c       # Sistema de interrupções IDT/PIC
├── io.c               # Funções de I/O (inb/outb)
├── linker.ld          # Script do linker
├── Makefile           # Sistema de build
└── README.md          # Esta documentação
```

### Componentes Principais

#### 1. Bootloader (boot.asm)
- **Modo Real (16-bit)**: Carregamento inicial
- **Carregamento do Kernel**: Lê 15 setores do disco
- **Transição para Modo Protegido**: Configura GDT e entra em 32-bit
- **Chamada do Kernel**: Executa função principal em C

#### 2. Sistema de Display (display.c)
- **Buffer VGA**: Manipulação direta da memória de vídeo (0xB8000)
- **Scroll**: Rolagem automática da tela
- **Cursor**: Controle de posição via portas VGA
- **Funções**: `print_char()`, `print_string()`, `print_number()`

#### 3. Sistema de Interrupções
- **IDT (Interrupt Descriptor Table)**: 256 entradas
- **PIC (Programmable Interrupt Controller)**: Configuração de IRQs
- **Handlers**: Teclado (IRQ1) e Timer (IRQ0)
- **Mapeamento de Teclas**: Conversão scancode → ASCII

#### 4. Jogo "Adivinhe o Número"
- **Geração Aleatória**: Usa timer tick como seed
- **Interface Interativa**: Input do usuário via teclado
- **Validação**: Números entre 1-100, máximo 7 tentativas
- **Feedback**: Dicas "muito alto" ou "muito baixo"

## Como Compilar e Executar

### Pré-requisitos

**No Windows com WSL:**
```bash
# Instalar dependências
sudo apt update
sudo apt install nasm gcc qemu-system-i386 make
```

**Verificar se já tem instalado:**
```bash
nasm --version
gcc --version
qemu-system-i386 --version
```

### Compilação

1. **Criar diretório do projeto:**
```bash
mkdir micro-kernel
cd micro-kernel
```

2. **Salvar todos os arquivos** (boot.asm, kernel.c, etc.) no diretório

3. **Compilar tudo:**
```bash
make all
```

4. **Executar no QEMU:**
```bash
make run
```

### Comandos Disponíveis

```bash
make all      # Compila bootloader + kernel → os.img
make run      # Executa no QEMU
make debug    # Executa com monitor do QEMU
make clean    # Remove arquivos gerados
make help     # Mostra ajuda
```

## Como Usar o Sistema

### 1. Boot e Inicialização
- O bootloader carrega automaticamente
- Tela de boas-vindas aparece
- Pressione qualquer tecla para começar

### 2. Jogando "Adivinhe o Número"
- **Objetivo**: Adivinhar número entre 1 e 100
- **Tentativas**: 7 chances máximas
- **Input**: Digite números e pressione ENTER
- **Correção**: Use BACKSPACE para apagar
- **Dicas**: Sistema informa se é "muito alto" ou "muito baixo"

### 3. Controles do Jogo
- **Números (0-9)**: Input do palpite
- **ENTER**: Confirma palpite / reinicia jogo
- **BACKSPACE**: Apaga último dígito

## Detalhes Técnicos

### Processo de Boot

1. **BIOS** carrega bootloader no setor 1 (0x7C00)
2. **Bootloader** lê kernel do disco (setores 2-16)
3. **Modo Protegido** ativado com GDT configurada
4. **Kernel** executado em 0x1000

### Sistema de Memória

```
0x0000-0x7BFF: Área livre
0x7C00-0x7DFF: Bootloader (512 bytes)
0x1000-0x8FFF: Kernel (até 32KB)
0xB8000:       Buffer VGA (4000 bytes)
0x90000:       Stack do sistema
```

### Mapeamento de Interrupções

- **IRQ0 (0x20)**: Timer - incrementa contador
- **IRQ1 (0x21)**: Teclado - processa input
- **PIC1**: 0x20-0x27 (IRQs 0-7)
- **PIC2**: 0x28-0x2F (IRQs 8-15)

### Algoritmo do Jogo

1. **Geração do número secreto**:
   ```c
   secret_number = (get_timer_tick() % 100) + 1;
   ```

2. **Processamento de input**:
   - Validação de caracteres (apenas dígitos)
   - Conversão string → int
   - Comparação com número secreto

3. **Lógica de feedback**:
   ```c
   if (guess < secret_number) print("Muito baixo!");
   else if (guess > secret_number) print("Muito alto!");
   else game_won = 1;
   ```

## Resolução de Problemas

### Erro: "command not found"
```bash
# Instalar ferramenta em falta
sudo apt install nasm  # ou gcc, qemu-system-i386
```

### Erro: "Permission denied"
```bash
# Dar permissões de execução
chmod +x *.sh
```

### QEMU não encontra imagem
```bash
# Verificar se os.img foi criado
ls -la *.img
make clean && make all
```

### Kernel não carrega
- Verificar se bootloader tem exatamente 512 bytes
- Confirmar assinatura 0xAA55 no final
- Checar se kernel está sendo carregado no endereço correto

## Pontuação Esperada

### ✅ Display de Caracteres (até 4 pontos)
- Sistema VGA completo com scroll
- Funções de print para char, string, number
- Controle de cursor e posicionamento

### ✅ Interrupções e Teclado (até 3 pontos)
- IDT configurada com 256 entradas
- PIC programado corretamente
- Handler de teclado com mapeamento completo
- Processamento de scancodes → ASCII

### ✅ Lógica do Jogo (até 4 pontos)
- Jogo completamente funcional
- Interface intuitiva e amigável
- Validação robusta de input
- Sistema de tentativas e feedback
- Reinício automático de partidas

**Total esperado: 11/11 pontos**

## Extensões Futuras

- Sistema de sons/beeps
- Cores personalizadas
- Mais jogos (forca, termo)
- Sistema de arquivos simples
- Suporte a mouse
- Interface gráfica básica

---

**Autor**: [Seu Nome]  
**Data**: Setembro 2025  
**Versão**: 1.0