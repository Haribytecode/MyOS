#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

void vga_clear(void);
void vga_putc(char c);

void kprint(const char *str);
void kprint_dec(uint32_t value);

#endif