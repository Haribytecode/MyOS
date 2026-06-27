#include "pic.h"
#include <stdint.h>
#include "console.h"
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void pic_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFE);  //ENABLE IRQ0 ONLY
    outb(0xA1, 0xFF);  //KEEP SLAVE PIC MASKED
}

void pit_init(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36); 
    outb(0x40, (uint8_t)(divisor & 0xFF));       
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); 
}

void pic_init(void)
{
    pic_remap();
    kprint("PIC OK\n");

    pit_init(100);
    kprint("PIT OK\n");
}
