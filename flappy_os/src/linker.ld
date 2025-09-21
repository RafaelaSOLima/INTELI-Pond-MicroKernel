/* src/linker.ld (VERSÃO FINAL PARA GERAR ELF) */
ENTRY(_start)

SECTIONS
{
    . = 0x0;

    .text :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss  :
    {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
}
