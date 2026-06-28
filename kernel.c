#include <stdint.h>
#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "tss.h"
#include "uart.h"

extern void tss_flush(void);
extern void enter_user_mode_v2(void);

void kernel_main(void)
{
    gdt_init();
    tss_init();

   static uint8_t kernel_stack[4096] __attribute__((aligned(16)));
    tss.esp0 = (uint32_t)&kernel_stack[4096];

    tss.ss0 = 0x10;
    tss.iomap_base = sizeof(tss_t);

    tss_flush();
    idt_init();

    kprint("Dropping to safe Ring 3 baseline...\n");

    // Mask hardware interrupts completely to block background interferences
    asm volatile("cli");

    uint32_t target_eip = 0x00200000;
    uint32_t target_esp = 0x00201000;

    uint8_t *user_code_dest = (uint8_t *)target_eip;

    user_code_dest[0] = 0xCD;   // int
    user_code_dest[1] = 0x80;
    user_code_dest[2] = 0xEB;   // jmp $
    user_code_dest[3] = 0xFE;
    // FIX: Force target variables straight into hardware registers EAX and EDX.
    // This removes stack references and executes the privilege drop cleanly!
    asm volatile (
        "movl %0, %%eax\n\t"  // Force target_eip into EAX register
        "movl %1, %%edx\n\t"  // Force target_esp into EDX register
        "call enter_user_mode_v2\n\t"
        :
        : "r"(target_eip), "r"(target_esp)
        : "eax", "edx", "memory"
    );

    while (1)
        ;
}
