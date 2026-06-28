#include "console.h"

void syscall_handler(void)
{
    kprint(">>> SYSCALL ENTERED <<<\n");
}