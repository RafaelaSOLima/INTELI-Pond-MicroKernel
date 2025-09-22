# Micro Kernel - Jogo Adivinhe o Número



## 🎮 O que é este projeto?

Este é um **micro kernel** educacional que demonstra os conceitos fundamentais de sistemas operacionais através de um jogo simples. O sistema:

- **Inicializa o hardware** desde o boot
- **Gerencia interrupções** de teclado e timer  
- **Controla o display** em modo texto VGA
- **Executa um jogo completo** de adivinhação de números

## 🔧 Requisitos do Sistema

### Linux/WSL:
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86
```

### Verificar versões:
- **GCC**: versão 7+ com suporte a 32-bit
- **NASM**: versão 2.13+
- **QEMU**: versão 2.11+

## 🚀 Como Executar

### 1. Compilar o sistema:
```bash
make clean
make all
```

### 2. Executar no emulador:
```bash
make run
```

### 3. Modo debug:
```bash
make debug
```

## 🎯 Como Jogar

1. **Tela inicial**: Pressione qualquer tecla para começar
2. **Digite um número** entre 1 e 100
3. **Pressione Enter** para confirmar seu palpite
4. **Use Backspace** para corrigir
5. **Siga as dicas**: "muito alto" ou "muito baixo"
6. **Você tem 7 tentativas** para acertar
7. **Pressione Enter** no final para jogar novamente

## 📚 O que é um Bootloader?

Um **bootloader** é o primeiro programa executado quando o computador liga. Suas responsabilidades:

- **Inicializar o hardware** básico (CPU, memória)
- **Configurar o modo protegido** (32-bit)
- **Carregar o kernel** da mídia de armazenamento
- **Transferir controle** para o kernel

### 📁 `boot.asm`:
- Configura segmentos de memória
- Carrega o kernel do disco
- Configura GDT (Global Descriptor Table)
- Entra em modo protegido
- Salta para o kernel

## 🧠 O que é um Kernel?

O **kernel** é o núcleo do sistema operacional. Responsabilidades:

- **Gerenciar hardware**: teclado, timer, display
- **Controlar interrupções**: responder a eventos
- **Fornecer serviços**: I/O, memória, processos
- **Executar programas**: no nosso caso, o jogo

## 🏗️ Arquitetura do Código

### Entrada do Sistema
```
boot.asm → kernel_start.asm → kernel_main()
```

### Componentes Principais:

#### 🔧 **Sistema Base**
- **`boot.asm`**: Bootloader que inicializa tudo
- **`kernel_start.asm`**: Ponte assembly→C
- **`linker.ld`**: Script de linkagem dos binários

#### 💻 **Kernel em C**
- **`kernel.c`**: Lógica principal e jogo
- **`kernel.h`**: Definições e protótipos
- **`display.c`**: Funções de tela e cursor
- **`io.c`**: Comunicação com hardware (portas)

#### ⚡ **Sistema de Interrupções**
- **`interrupts.c`**: Configuração IDT e PIC
- **`int_handlers.asm`**: Handlers de interrupção
- **Timer**: Conta tempo para números aleatórios
- **Teclado**: Captura teclas do usuário

## 🔍 Detalhes Técnicos

### Memory Layout:
```
0x7C00: Bootloader (carregado pelo BIOS)
0x8000: Kernel (carregado pelo bootloader)
0x90000: Stack do sistema
0xB8000: Buffer de vídeo VGA
```

### Interrupções Utilizadas:
- **IRQ0 (0x20)**: Timer do sistema
- **IRQ1 (0x21)**: Teclado

### Features Implementadas:
- ✅ Boot em modo protegido 32-bit
- ✅ Sistema de interrupções (IDT)
- ✅ Driver de teclado com scancode mapping
- ✅ Driver de display VGA em modo texto
- ✅ Gerador de números pseudo-aleatórios
- ✅ Interface de usuário interativa
- ✅ Gerenciamento de estado do jogo

## 📁 Estrutura dos Arquivos

```
/
├── boot.asm           # Bootloader principal
├── kernel_start.asm   # Entrada do kernel
├── kernel.c           # Código principal do jogo
├── kernel.h           # Headers e definições
├── display.c          # Funções de display
├── interrupts.c       # Sistema de interrupções
├── int_handlers.asm   # Handlers assembly
├── io.c               # Funções de I/O
├── linker.ld          # Script de linkagem
├── Makefile           # Automação da compilação
└── README.md          # Esta documentação
```

## 🛠️ Comandos do Makefile

```bash
make all      # Compilar tudo
make clean    # Limpar arquivos temporários
make run      # Executar no QEMU
make debug    # Executar com debug
make info     # Mostrar informações dos binários
```

## 🎓 Conceitos de SO Demonstrados

1. **Boot Process**: Como um OS inicializa
2. **Memory Management**: Organização da memória
3. **Interrupt Handling**: Resposta a eventos hardware
4. **Device Drivers**: Controle de teclado e display
5. **System Calls**: Interface entre user/kernel space
6. **Real-time Systems**: Resposta a eventos em tempo real



## 👨‍💻 Desenvolvimento



**Tecnologias**: Assembly x86, C, NASM, GCC, QEMU, Make

---

