#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 32

typedef enum
{
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED
} task_state_t;

typedef struct task
{
    uint32_t pid;

    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    uint32_t cr3;

    task_state_t state;

    struct task *next;
} task_t;

void scheduler_init(void);
void schedule(void);

task_t *scheduler_current(void);

#endif