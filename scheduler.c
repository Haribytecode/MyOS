#include "scheduler.h"

static task_t task_table[MAX_TASKS];

static task_t *current_task = 0;

void scheduler_init(void)
{
    for (uint32_t i = 0; i < MAX_TASKS; i++)
    {
        task_table[i].pid = i;
        task_table[i].esp = 0;
        task_table[i].ebp = 0;
        task_table[i].eip = 0;
        task_table[i].cr3 = 0;
        task_table[i].state = TASK_UNUSED;
        task_table[i].next = 0;
    }

    current_task = &task_table[0];
    current_task->state = TASK_RUNNING;
}

void schedule(void)
{
    return;
}

task_t *scheduler_current(void)
{
    return current_task;
}