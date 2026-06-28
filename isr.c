#include "panic.h"
#include "keyboard.h"
#include <stdint.h>
#include "uart.h"
#include "paging.h"
#include "console.h"
extern uint32_t alloc_frame(void);
extern void map_page(uint32_t virt, uint32_t phys);

void isr_handler(void)
{
    kprint("ISR\n");
    while (1);
}
void page_fault_handler(uint32_t error)
{
    uint32_t cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2));

    uart_puts("PAGE FAULT\n");
    uart_puts("CR2 = ");
    uart_puthex(cr2);
    uart_puts("\n");

    uart_puts("ERR = ");
    uart_puthex(error);
    uart_puts("\n");

    while (1)
        asm volatile("cli; hlt");
}