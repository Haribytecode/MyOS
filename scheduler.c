#include "scheduler.h"
#include "task.h"
#include "console.h"
static task_t task_table[MAX_TASKS];
#include "uart.h"
static task_t *current_task = 0;
static uint32_t next_pid = 1;
#define KERNEL_STACK_SIZE 4096

static uint8_t kernel_stacks[MAX_TASKS][KERNEL_STACK_SIZE];

void scheduler_init(void)
{
    for (uint32_t i = 0; i < MAX_TASKS; i++)
    {
        task_table[i].pid = 0;
        task_table[i].esp = 0;
        task_table[i].ebp = 0;
        task_table[i].eip = 0;
        task_table[i].cr3 = 0;
        task_table[i].state = TASK_UNUSED;
        task_table[i].next = 0;

        task_table[i].edi = 0;
        task_table[i].esi = 0;
        task_table[i].ebx = 0;
        task_table[i].edx = 0;
        task_table[i].ecx = 0;
        task_table[i].eax = 0;

        task_table[i].cs = 0x08;
        task_table[i].eflags = 0x202;

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
            uint32_t *stack = (uint32_t *)task_table[i].kernel_stack_top;

            /* CPU interrupt frame */
            *--stack = 0x202;                  /* EFLAGS */
            *--stack = 0x08;                   /* CS */
            *--stack = (uint32_t)task_entry;   /* EIP */

            /* pushad frame */
            *--stack = 0; /* EAX */
            *--stack = 0; /* ECX */
            *--stack = 0; /* EDX */
            *--stack = 0; /* EBX */
            *--stack = 0; /* Original ESP */
            *--stack = 0; /* EBP */
            *--stack = 0; /* ESI */
            *--stack = 0; /* EDI */

            task_table[i].esp = (uint32_t)stack;
            task_table[i].ebp = (uint32_t)stack;
            task_table[i].eip = (uint32_t)task_entry;
            task_table[i].cr3 = 0;
            task_table[i].next = 0;

            return &task_table[i];
        }
    }

    return 0;
}


void scheduler_save_context(uint32_t *esp)
{
    current_task->esp = (uint32_t)esp;
}

task_t *scheduler_current(void)
{
    return current_task;
}

task_t *scheduler_next_task(void)
{
    uint32_t start = current_task->pid;

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

uint32_t *schedule(uint32_t *esp)
{
    scheduler_save_context(esp);

    task_t *next = scheduler_next_task();

    if (next != current_task)
    {
        current_task->state = TASK_READY;
        next->state = TASK_RUNNING;
        current_task = next;
    }

    return (uint32_t *)current_task->esp;
}