#include "sync.h"

void enter_critical(void) {
    __asm__ volatile ("cli" : : : "memory");
}

void exit_critical(void) {
    __asm__ volatile ("sti" : : : "memory");
}