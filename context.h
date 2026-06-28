#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

typedef struct context
{
    uint32_t ebp;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t eip;
} context_t;

#endif
