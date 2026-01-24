#ifndef UART_H
#define UART_H

#include <stdint.h>



void uart_init();
void uart_putc(char c);
void uart_puts(const char* s);
void uart_puthex(uint32_t v);
void safe_uart_puts(const char *s);

#endif
