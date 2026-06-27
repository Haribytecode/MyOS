#include <stdint.h>
#include "uart.h"
#include "io.h"
#include "console.h"
#define PORT 0x3F8




void uart_init(void)
{
    outb(PORT + 1, 0x00);    // Disable interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB
    outb(PORT + 0, 0x03);    // Divisor low byte (38400 baud)
    outb(PORT + 1, 0x00);    // Divisor high byte
    outb(PORT + 3, 0x03);    // 8 bits, no parity, 1 stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO
    outb(PORT + 4, 0x0B);    // <-- THIS LINE
}
void uart_putc(char c)
{
    while ((inb(PORT + 5) & 0x20) == 0)
        ;

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


