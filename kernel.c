#include <stdint.h>
#include "task.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "uart.h"

extern void vga_clear(void);
extern void kprint(const char* str);
extern void outb(unsigned short port, unsigned char val);

static int in_irq = 0;

// IRQ0: PIT timer interrupt (task switching)
void timer_handler(void) {
    if (in_irq) {
        uart_puts("REENTRY"); 
        return; 
    }
    in_irq = 1;
    outb(0x20, 0x20); 

    task_t* old = current_task(); 
    task_t* next = next_task();

    in_irq = 0; 
    switch_task(old, next);
}

void kernel_main(void) {
    gdt_init();
    idt_init();
    pic_init();   
    uart_init();
    
    vga_clear(); 
    
    kprint("VGA MEMORY: OK\n");
    kprint("KEYBOARD: INITIALIZING...\n");
    
    schedule_init(); 
    
    kprint("SYSTEM READY.\n");
    kprint("kernel_shell> ");

    __asm__ volatile("sti"); 

    while(1) {
        __asm__ volatile("hlt"); 
    }
}