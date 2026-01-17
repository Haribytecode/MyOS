#include "panic.h"
extern void uart_puts(const char *);
void panic(const char *msg){
    uart_puts("KERNEL PANIC\n");
    uart_puts(msg);
    uart_puts("SYSTEM HALTED\n");
    while(1){
        __asm__ volatile("cli;hlt");
    }
}