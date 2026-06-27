#include <stdint.h>
#include "user.h"

static uint8_t user_stack[4096] __attribute__((aligned(16)));

void enter_user_mode(void)
{
    uint32_t user_stack_top = (uint32_t)&user_stack[4096];

    uint32_t user_ss = 0x23;       // User Data Selector (RPL=3)
    uint32_t user_cs = 0x1B;       // User Code Selector (RPL=3)
    uint32_t user_eflags = 0x202;  // IF=1

    (void)user_stack_top;
    (void)user_ss;
    (void)user_cs;
    (void)user_eflags;
}