#ifndef UART_H
#define UART_H

#include <stdint.h>

// This tells other files that these functions exist somewhere else

void uart_init();
void uart_putc(char c);
void uart_puts(const char* s);
void uart_puthex(uint32_t v);
void safe_uart_puts(const char *s);

#endif