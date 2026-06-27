#include "uart.h"
#include <stdint.h>
#include "console.h"
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}







uint8_t get_rtc_register(int reg) {
    outb(0x70, reg);
    return inb(0x71);
}

int bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}




unsigned char kbd_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char cmd_buffer[64];
int cmd_idx = 0;

int str_equal(char *s1, char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *s1 == *s2;
}

void run_shell_command(char *buf) {
    if (str_equal(buf, "help")) {
        kprint("\nCommands: help, version, clear, time, date, mem, tasks");
    } else if (str_equal(buf, "version")) {
        kprint("\nMY_OS v1.0 - Built by Hariharan");
    } else if (str_equal(buf, "time")) {
        int hour = bcd_to_bin(get_rtc_register(0x04));
        int min  = bcd_to_bin(get_rtc_register(0x02));
        int sec  = bcd_to_bin(get_rtc_register(0x00));

        kprint("\nHardware Time: ");
        if (hour < 10) { kprint("0"); } 
        kprint_dec(hour); kprint(":");
        if (min < 10) { kprint("0"); } 
        kprint_dec(min); kprint(":");
        if (sec < 10) { kprint("0"); } 
        kprint_dec(sec);

    } else if (str_equal(buf, "date")) {
        int day = bcd_to_bin(get_rtc_register(0x07));
        int mon = bcd_to_bin(get_rtc_register(0x08));
        int yr  = bcd_to_bin(get_rtc_register(0x09));
        
        kprint("\nDate: ");
        if (day < 10) { kprint("0"); } 
        kprint_dec(day); kprint("/");
        if (mon < 10) { kprint("0"); } 
        kprint_dec(mon); kprint("/20"); 
        kprint_dec(yr);

    } else if (str_equal(buf, "mem")) {
        kprint("\n--- Memory Info ---\nKernel: 1MB\nStack: 16KB\nStatus: OK");
    } else if (str_equal(buf, "tasks")) {
        kprint("\nTasks: Shell(0), TaskA(1), TaskB(2) Active.");
    } else if (str_equal(buf, "clear")) {
        vga_clear();
    } else if (buf[0] != '\0') {
        kprint("\nUnknown command.");
    }
    kprint("\nkernel_shell> ");
}

void keyboard_handler(void) {
    uint8_t sc = inb(0x60);
    kprint("SC=");
    kprint_dec(sc);
    kprint("\n");
    if (!(sc & 0x80)) {
        char c = kbd_map[sc];
        if (c == '\n') {
            cmd_buffer[cmd_idx] = '\0';
            run_shell_command(cmd_buffer);
            cmd_idx = 0;
        } else if (c == '\b' && cmd_idx > 0) {
            cmd_idx--;
            kprint("\b \b");
        } else if (c != 0 && cmd_idx < 63) {
            cmd_buffer[cmd_idx++] = c;
            char str[2] = {c, '\0'};
            kprint(str);
        }
    }
    outb(0x20, 0x20);
}
