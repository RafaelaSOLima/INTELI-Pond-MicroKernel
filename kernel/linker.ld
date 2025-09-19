ENTRY(_start)
SECTIONS {
    . = 0x1000;
    .text : {
        *(.boot)
        *(.text)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
}