#include <stdint.h>
#include "scheduler.h"
extern void kprint(const char* str);
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

volatile uint32_t ticks = 0;

void timer_handler(void)
{
    schedule();
    outb(0x20, 0x20);
}