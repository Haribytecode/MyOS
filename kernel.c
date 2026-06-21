#include <stdint.h>
#include "task.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "uart.h"
#include "paging.h"
#include "heap.h"

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
/* ========================================================================
   🚀 HARIHARAN-OS MAIN PRODUCTION KERNEL ENTRY POINT
   ======================================================================== */
void kernel_main(void)
{
    /* 1. Initialize core CPU security and segment descriptors */
    gdt_init();
    
    /* 2. Initialize the Interrupt Descriptor Table and hardware gates */
    idt_init();
    
    /* 3. Initialize the Programmable Interrupt Controller (Remap IRQs) */
    pic_init();

    /* 4. Reset video matrix display buffer */
    vga_clear();
    
    kprint("====================================================\n");
    kprint("👑 Welcome to HariharanOS Production Kernel Environment 👑\n");
    kprint("====================================================\n");
    kprint("[OK] Global Descriptor Table Operational.\n");
    kprint("[OK] Interrupt Descriptor Table Initialized.\n");
    kprint("[OK] PIC Subsystem Remapped.\n");

    /* 5. Initialize the Bidirectional Virtual Memory Heap Infrastructure */
    heap_init();
    kprint("[OK] Dynamic Kernel Heap Manager Loaded Stably at 3GB virtual highway.\n");
    kprint("====================================================\n");
    kprint("SYSTEM STATUS: RUNNING AND IDLE. AWAITING INTERRUPTS...\n");

    /* 6. 💤 Infinite low-power execution loop */
    while (1) {
        /* If your assembly wrapper allows it, you can place an 'asm volatile("hlt");' here */
    }
}
