#include <stdint.h>
#include "task.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "uart.h"

/* VGA helpers implemented in keyboard.c */
extern void vga_clear(void);
extern void kprint(const char* str);

/* This variable already exists in keyboard.c */
extern int vga_pos;

/* Hardware */
extern void outb(unsigned short port, unsigned char val);

static int in_irq = 0;

/* ===============================
   BINARY FACE (64 x 24)
   =============================== */
static const char *boot_face[] = {
"0000000000000000000000000000000000000000",
"0000000000000000000011111100000000000000",
"0000000000011111111111111111111111000000",
"0000000011111111111111111111111111100000",
"0000000111111111111111111111111111110000",
"0000011111111111111111111111111111110000",
"0000111111111111111111111111111111111000",
"0000111111111111111111111111111111110000",
"0000111111111000000000000000111111110000",
"0000111111000000000000000000000111110000",
"0000111111000000000000000000000011110000",
"0000011111000000000000000000000011110000",
"0000011110011111110000001111100001110000",
"0000001110001111110000001111010001110000",
"0000010110000000000000000000000001100000",
"0000010110000000000000000000000001000000",
"0000000010000000000000000000000001000000",
"0000000001000000000000000000000001000000",
"0000000001000001111000000111100010000000",
"0000000000100001000011110000100010000000",
"0000000000111001000000000000101111000000",
"0000000001110111000000000000101111100000",
"0000011111110001111111111100001111111000",
"0001111111111100000000000000011111111110",
"0000000000000000000000000000000000000000"
};


/* ===============================
   VGA CURSOR CONTROL (REAL FIX)
   =============================== */
static inline void vga_set_cursor(int row, int col) {
    vga_pos = row * 80 + col;
}

/* ===============================
   BOOT DELAY (SAFE)
   =============================== */
static void boot_delay(void) {
 for (volatile uint32_t i = 0; i < 400000000; i++)
 {
        __asm__ volatile("nop");
    }
}

/* ===============================
   SHOW BOOT FACE (CENTERED)
   =============================== */
static void show_boot_face(void) {
    vga_clear();

    const int face_width  = 64;
    const int face_height = 24;

    const int screen_width = 80;

    int start_col = (screen_width - face_width) / 2;  // = 8
    int start_row = 0;

    for (int i = 0; i < face_height; i++) {
        vga_set_cursor(start_row + i, start_col);
        kprint(boot_face[i]);
    }

    vga_set_cursor(start_row + face_height + 1, start_col);
    kprint("Booting Hariharan OS v1.0...");

    boot_delay();
}

/* ===============================
   TIMER IRQ
   =============================== */
void timer_handler(void) {
    if (in_irq) return;
    in_irq = 1;

    outb(0x20, 0x20);   // EOI

    task_t* old = current_task();
    task_t* next = next_task();

    in_irq = 0;
    switch_task(old, next);
}

/* ===============================
   KERNEL ENTRY
   =============================== */
void kernel_main(void) {
    gdt_init();
    idt_init();
    pic_init();
    uart_init();

    /* SIGNATURE */
    show_boot_face();
    vga_clear();              // ← bring shell to visible screen


   
    kprint("VGA MEMORY: OK\n");
    kprint("KEYBOARD: INITIALIZING...\n");

    schedule_init();

    kprint("SYSTEM READY.\n");
    kprint("kernel_shell> ");

    __asm__ volatile("sti");

    while (1) {
        __asm__ volatile("hlt");
    }
}
