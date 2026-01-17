#include <stdint.h>
#include "uart.h"
#include "sync.h"

// We use the COM1 port address
#define PORT 0x3F8

// Helper to send data to hardware
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void uart_init() {
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (38400 baud)
    outb(PORT + 1, 0x00);    // (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
}

void uart_putc(char c) {
    // We should technically check if the transmit holding register is empty, 
    // but for simple debug, we just blast it out.
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