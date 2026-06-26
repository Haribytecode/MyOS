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

extern void vga_clear(void);
extern void kprint(const char* str);

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

    vga_clear();

    kprint("====================================================\n");
    kprint("👑 Welcome to HariharanOS Production Kernel Environment 👑\n");
    kprint("====================================================\n");
    kprint("[OK] Global Descriptor Table Operational.\n");
    kprint("[OK] Interrupt Descriptor Table Initialized.\n");
    kprint("[OK] PIC Subsystem Remapped.\n");

    heap_init();
    kprint("[OK] Dynamic Kernel Heap Manager Loaded Stably at 3GB virtual highway.\n");

    scheduler_init();
    kprint("[OK] SCHEDULER INITIALIZED.\n");
    task_t *t1 = scheduler_create_task();
    task_t *t2 = scheduler_create_task();
    if (t1->kernel_stack_top != t2->kernel_stack_top)
    kprint("[OK] UNIQUE KERNEL STACKS.\n");
else
    kprint("[FAIL] STACK COLLISION.\n");
task_t *next = scheduler_next_task();

if (next == t1)
    kprint("[OK] NEXT TASK SELECTOR PASSED.\n");
else
    kprint("[FAIL] NEXT TASK SELECTOR FAILED.\n");

if (t1 && t2)
{
    kprint("[OK] TASK CREATION PASSED.\n");
}
else
{
    kprint("[FAIL] TASK CREATION FAILED.\n");
}

    kprint("====================================================\n");
    kprint("SYSTEM STATUS: RUNNING AND IDLE. AWAITING INTERRUPTS...\n");

    asm volatile("sti");
    kprint("INTERRUPTS ENABLED\n");

    while (1)
    {
        asm volatile("hlt");
    }
}