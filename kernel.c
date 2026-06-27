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
#include "tss.h"
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
kprint("A\n");

idt_init();
kprint("B\n");

scheduler_init();
kprint("C\n");

scheduler_create_task();
kprint("D\n");

scheduler_create_task();
kprint("E\n");

pic_init();
asm volatile("sti");
kprint("F\n");

while (1)
    ;
}