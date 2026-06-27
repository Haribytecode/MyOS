#include <stdint.h>
#include "uart.h"
#include "console.h"
__attribute__((noreturn))
void task_entry(void)
{
    while (1)
    {
        volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

        

        asm volatile("hlt");
    }
}