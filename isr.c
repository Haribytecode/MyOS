#include "panic.h"
#include "keyboard.h"

// Declaration of our handler
extern void timer_handler(void);

void isr_handler(void) {
    // For Day 21, since we don't have "Interrupt Numbers" yet,
    // we just call timer_handler. 
    // BUT: Comment out the divide-by-zero in kernel.c for now!
    timer_handler();
}