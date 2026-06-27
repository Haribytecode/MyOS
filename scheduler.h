#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 32

typedef enum
{
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING
} task_state_t;

typedef struct task
{
    uint32_t pid;

    /* Complete interrupt frame */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /* CPU state */
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

    uint32_t cr3;

    uint32_t kernel_stack_top;

    task_state_t state;

    struct task *next;
} task_t;

void scheduler_init(void);
uint32_t *schedule(uint32_t *esp);
void scheduler_save_context(uint32_t *esp);
task_t *scheduler_next_task(void);
task_t *scheduler_current(void);
task_t *scheduler_create_task(void);
#endif