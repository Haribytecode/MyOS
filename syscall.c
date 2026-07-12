#include "console.h"
#include "uart.h"
uint32_t *syscall_handler(uint32_t *esp)
{
    uart_puts(">>> SYSCALL FROM RING 3 <<<\n");
    return esp;   // unchanged stack pointer
}