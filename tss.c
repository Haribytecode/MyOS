#include "tss.h"
extern void tss_flush(void);
tss_t tss;

void tss_init(void)
{
    for (uint32_t i = 0; i < sizeof(tss_t); i++)
        ((uint8_t *)&tss)[i] = 0;

    tss.ss0 = 0x10;
    tss.iomap_base = sizeof(tss_t);

    // tss_flush();   <-- COMMENT THIS OUT
}

void tss_set_kernel_stack(uint32_t stack)
{
    tss.esp0 = stack;
}