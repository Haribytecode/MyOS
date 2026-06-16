#include "panic.h"
#include "keyboard.h"
#include <stdint.h>
#include "uart.h"
#include "paging.h"

extern void timer_handler(void);
extern void kprint(const char* str);
extern uint32_t alloc_frame(void);
extern void map_page(uint32_t virt, uint32_t phys);

void isr_handler(void) {
     while(1);
}

void page_fault_handler(void)
{
    uint32_t fault_addr;
    
    // 1. Capture the exact exploding address from the CR2 register [Swarthmore College]
    asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

    kprint("🚨 HARDWARE PAGE FAULT TRAPPED AT VIRTUAL: ");
    
    // Dynamic text-mode hex rendering routine
    char hex[] = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4) {
        char c = hex[(fault_addr >> i) & 0xF];
        char str[2] = {c, '\0'};
        kprint(str);
    }
    kprint("\n");

    // 2. Request an isolated physical frame block [Swarthmore College, OSDev]
    uint32_t frame = alloc_frame();
    if(frame == 0){
        kprint("KERNEL PANIC: OUT OF PHYSICAL MEMORY!\n");
        while(1);
    }
    
    // 3. Heal the page tables in real-time [Swarthmore College, OSDev]
    map_page(fault_addr, frame);
    
    kprint("🔧 DEMAND PAGING ENGINE SUCCESSFULLY REPAIRED AND ROUTED PATHWAY!\n");
    return;
}
