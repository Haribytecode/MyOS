#include "scheduler.h"

static task_t task_table[MAX_TASKS];

static task_t *current_task = 0;
extern void kprint(const char* str);
static uint32_t next_pid = 1;
#define KERNEL_STACK_SIZE 4096

static uint8_t kernel_stacks[MAX_TASKS][KERNEL_STACK_SIZE];
void scheduler_init(void)
{
    for (uint32_t i = 0; i < MAX_TASKS; i++)
    {   
        task_table[i].slot = i;
        task_table[i].pid = 0;
        task_table[i].esp = 0;
        task_table[i].ebp = 0;
        task_table[i].eip = 0;
        task_table[i].cr3 = 0;
        task_table[i].state = TASK_UNUSED;
        task_table[i].next = 0;
        task_table[i].kernel_stack_top =
    (uint32_t)&kernel_stacks[i][KERNEL_STACK_SIZE];
    }

    current_task = &task_table[0];

    current_task->pid = 0;
    current_task->state = TASK_RUNNING;
}

task_t *scheduler_create_task(void)
{
    for (uint32_t i = 1; i < MAX_TASKS; i++)
    {
        if (task_table[i].state == TASK_UNUSED)
        {
            task_table[i].pid = next_pid++;
            task_table[i].state = TASK_READY;
            task_table[i].esp = task_table[i].kernel_stack_top;
            task_table[i].ebp = task_table[i].kernel_stack_top;
            task_table[i].eip = 0;
            task_table[i].cr3 = 0;
            task_table[i].next = 0;

            return &task_table[i];
        }
    }

    return 0;
}


void scheduler_save_context(void)
{
    asm volatile("mov %%esp, %0" : "=r"(current_task->esp));
    asm volatile("mov %%ebp, %0" : "=r"(current_task->ebp));

    asm volatile(
        "pushf\n\t"
        "pop %0"
        : "=r"(current_task->eflags));

    current_task->eip = 0;
}


task_t *scheduler_current(void)
{
    return current_task;
}

task_t *scheduler_next_task(void)
{
    uint32_t start = current_task->slot;
    for (uint32_t i = 1; i < MAX_TASKS; i++)
    {
        uint32_t index = (start + i) % MAX_TASKS;

        if (task_table[index].state == TASK_READY)
        {
            return &task_table[index];
        }
    }

    return current_task;
}

void schedule(void)
{
    scheduler_save_context();

    task_t *next = scheduler_next_task();

    if (next != current_task)
    {
        current_task->state = TASK_READY;
        next->state = TASK_RUNNING;
        current_task = next;
    }
}
