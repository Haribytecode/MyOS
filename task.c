#include <stdint.h>
#include "uart.h"
#include "console.h"
__attribute__((noreturn))
void task_entry(void)
{
    while (1)
    {
        volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

        vga[80] = 'A' | (0x0A << 8);
        vga[81] = 'B' | (0x0A << 8);
        vga[82] = 'C' | (0x0A << 8);

        asm volatile("hlt");
    }
}