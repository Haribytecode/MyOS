#include <stdint.h>
#include "console.h"

void user_main(void)
{
    while (1)
    {
        asm volatile("" ::: "memory");
    }
}