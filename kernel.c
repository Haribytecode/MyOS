#include <stdint.h>
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "uart.h"
#include "paging.h"
#include "heap.h"
#include "scheduler.h"
#include "timer.h"
#include "console.h"
extern void vga_clear(void);
extern void vga_putc(char c);
extern int vga_pos;
static int in_irq = 0;

static inline void vga_set_cursor(int row, int col)
{
    vga_pos = row * 80 + col;
}


/* ================= PRODUCTION KERNEL ENTRY ================= */
/* ========================================================================
   🚀 HARIHARAN-OS MAIN PRODUCTION KERNEL ENTRY POINT
   ======================================================================== */
void kernel_main(void)
{
    gdt_init();
    idt_init();
    pic_init();
    uart_init();

    heap_init();
    scheduler_init();

    scheduler_create_task();
    scheduler_create_task();

    asm volatile("sti");

    while (1)
        asm volatile("hlt");
}