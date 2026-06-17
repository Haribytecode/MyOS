#include <stdint.h>
#include "task.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "uart.h"
#include "paging.h"

extern void vga_clear(void);
extern void kprint(const char* str);

extern int vga_pos;
extern void outb(unsigned short port, unsigned char val);

static int in_irq = 0;

static inline void vga_set_cursor(int row, int col)
{
    vga_pos = row * 80 + col;
}

/* ================= IRQ TIMING CONFIG ================= */
void timer_handler(void)
{
    if (in_irq)
        return;
    in_irq = 1;

    outb(0x20, 0x20);

    task_t *old = current_task();
    task_t *next = next_task();

    in_irq = 0;
    switch_task(old, next);
}

/* ================= PRODUCTION KERNEL ENTRY ================= */
void kernel_main(void)
{
    gdt_init();
    idt_init();
    pic_init();

    vga_clear();

    kprint("HIGHER HALF KERNEL ONLINE!\n");

    while (1);
}