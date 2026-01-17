#include "pic.h"
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void pic_remap(void) {
    // ICW1: Initialize PICs
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Remap offsets 32 (master) and 40 (slave)
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3: Master has slave on IRQ2, Slave cascade identity
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask interrupts: enable only IRQ0 (timer) and IRQ1 (keyboard)
    outb(0x21, 0xFC); // 11111100 → enable IRQ0, IRQ1
    outb(0xA1, 0xFF); // Mask all slave IRQs
}

void pit_init(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36); // Command port
    outb(0x40, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
}
// PIC remap: move hardware IRQs from 0–15 to IDT vectors 32–47
void pic_init(void) {
    pic_remap();   // Must call this first
    pit_init(100); // 100 Hz, for managing interrupt smoothly 
}
//pit is the one which pushes to print B after A and so on even though Task A does not want to stop it forces task A to stop and execute Task B to execute which prints B by context switching ROUND ROBIN //
