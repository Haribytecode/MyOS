OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

SECTIONS
{
    /* Link at 1 MiB (common safe load address) */
    . = 0x00100000;

    .text : {
        *(.multiboot)
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data : { *(.data) }

    .bss : {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    }

    PROVIDE(end = .);
    PROVIDE(__stack_top = 0x00090000);
}

/* Explicit note so assembler/linker won't assume executable stack */
.note.GNU-stack : { *(.note.GNU-stack) }
