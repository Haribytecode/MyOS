ENTRY(_start)

KERNEL_VMA = 0xC0000000;
KERNEL_LMA = 0x00100000;

SECTIONS
{
    /* Bootstrap code loaded at 1 MB */
    . = KERNEL_LMA;

    .bootstrap :
    {
        *(.multiboot)
        *(.bootstrap)
    }

    _bootstrap_end = ALIGN(LOADADDR(.bootstrap) + SIZEOF(.bootstrap), 4096);

    /* Higher-half kernel virtual addresses */
    . = KERNEL_VMA + KERNEL_LMA;

    .text : AT(_bootstrap_end)
    {
        *(.text)
    }

    .rodata : AT(LOADADDR(.text) + SIZEOF(.text))
    {
        *(.rodata*)
    }

    .data : AT(LOADADDR(.rodata) + SIZEOF(.rodata))
    {
    *(.data)
    }

    /* 4 KiB align virtual address */
    . = ALIGN(4096);

    .bss : AT(ALIGN(LOADADDR(.data) + SIZEOF(.data), 4096))
    {
        . = ALIGN(4096);
    *(COMMON)
    *(.bss)
    }
    __phys_page_directory = LOADADDR(.bss) + (page_directory - ADDR(.bss));
    __phys_first_pt       = LOADADDR(.bss) + (first_page_table - ADDR(.bss));
    __phys_second_pt      = LOADADDR(.bss) + (second_page_table - ADDR(.bss));
    __phys_kernel_pt =
    LOADADDR(.bss) + (kernel_page_table - ADDR(.bss));
    

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}