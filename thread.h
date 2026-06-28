#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

typedef enum
{
    THREAD_UNUSED = 0,
    THREAD_READY,
    THREAD_RUNNING
} thread_state_t;

struct thread;

typedef struct thread
{
    uint32_t tid;               // Offset 0
    uint32_t kernel_stack_top;  // Offset 4
    uint32_t kernel_esp;        // Offset 8 -> GUARANTEED BY PACKING
    thread_state_t state;       // Offset 12
} __attribute__((packed)) thread_t;

void thread_init(thread_t *thread, uint32_t tid, void (*fn)(void *), void *arg);
void enter_user_mode(uint32_t user_eip, uint32_t user_esp);
#endif
