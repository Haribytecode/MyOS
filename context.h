#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

/*
 * Saved CPU context.
 * This structure is ONLY used by context_switch().
 * It is NOT the interrupt frame.
 */

typedef struct context
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t eip;

} context_t;

#endif