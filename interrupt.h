#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

/*
 * Layout of the interrupt frame after:
 *
 *   CPU pushes:
 *       EIP
 *       CS
 *       EFLAGS
 *
 *   timer_stub executes:
 *       pusha
 *
 * This structure MUST exactly match the stack layout.
 */

typedef struct interrupt_frame
{
    /* pusha (reverse pop order) */

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /* CPU interrupt frame */

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

} interrupt_frame_t;

#endif