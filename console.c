#include "console.h"
#include "uart.h"

#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_COLOR   0x0A

static volatile uint16_t *vga_buffer =
    (volatile uint16_t *)VGA_ADDRESS;

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static void console_newline(void)
{
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= VGA_HEIGHT)
        cursor_y = 0;
}

void vga_clear(void)
{
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
        {
            vga_buffer[y * VGA_WIDTH + x] =
                ((uint16_t)VGA_COLOR << 8) | ' ';
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}

void vga_putc(char c)
{
    if (c == '\n')
    {
        console_newline();
        return;
    }

    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] =
        ((uint16_t)VGA_COLOR << 8) | c;

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
        console_newline();
}

void kprint(const char *str)
{
    while (*str)
    {
        uart_putc(*str);
        vga_putc(*str);
        str++;
    }
}

void kprint_dec(uint32_t value)
{
    char buffer[11];
    int i = 0;

    if (value == 0)
    {
        kprint("0");
        return;
    }

    while (value)
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--)
        vga_putc(buffer[i]), uart_putc(buffer[i]);
}

void kprint_hex(uint32_t value)
{
    char hex_digits[] = "0123456789ABCDEF";
    
    // Dynamic text-mode hex rendering routine
    for (int i = 28; i >= 0; i -= 4) 
    {
        char c = hex_digits[(value >> i) & 0xF];
        char str[2] = {c, '\0'};
        kprint(str);
    }
}
