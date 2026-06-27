#include "interrupt.h"
#include "scheduler.h"
#include <stdint.h>
#include "console.h"
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

interrupt_frame_t *timer_handler(interrupt_frame_t *frame)
{
    kprint("IRQ\n");
    outb(0x20, 0x20);   // Send EOI

    return frame;       // No scheduling yet
}