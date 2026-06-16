.set MAGIC,    0x1BADB002
.set FLAGS,    3
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
    .skip 16384 # 16 KB
stack_top:

.section .text
.global _start
_start:
    # Set up the low-memory execution stack pointer
    mov $stack_top, %esp
    
    # Branch into your high-level C code configuration
    call kernel_main
    
    cli
halt_loop:
    hlt
    jmp halt_loop


.section .note.GNU-stack,"",@progbits
