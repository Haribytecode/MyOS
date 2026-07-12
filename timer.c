#include "interrupt.h"
#include "scheduler.h"
#include <stdint.h>
#include "console.h"
#include "uart.h"
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

interrupt_frame_t *timer_handler(interrupt_frame_t *frame)
{
    outb(0x20, 0x20);              // Send EOI
    return (interrupt_frame_t *)schedule((uint32_t *)frame);
}