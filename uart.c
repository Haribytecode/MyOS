#include <stdint.h>
#include "uart.h"
#include "sync.h"


#define PORT 0x3F8


static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void uart_init() {
    outb(PORT + 1, 0x00);    
    outb(PORT + 3, 0x80);    
    outb(PORT + 0, 0x03);    
    outb(PORT + 1, 0x00);   
    outb(PORT + 3, 0x03);    
    outb(PORT + 2, 0xC7);    
}

void uart_putc(char c) {
 
    outb(PORT, c);
}

void uart_puts(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        uart_putc(s[i]);
    }
}
void uart_puthex(uint32_t v) {
    const char hex[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 7; i >= 0; --i) {
        uint8_t nib = (v >> (i * 4)) & 0xF;
        uart_putc(hex[nib]);
    }
}


void safe_uart_puts(const char *s) {
    enter_critical();
    uart_puts(s);
    exit_critical();
}
