#include <stdint.h>
#include "scheduler.h"
#include "console.h"
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint32_t *timer_handler(uint32_t *esp)
{
    esp = schedule(esp);

    outb(0x20, 0x20);

    return esp;
}