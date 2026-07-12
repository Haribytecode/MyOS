#include "scheduler.h"
#include "console.h"
#include "uart.h"
#include "tss.h"
extern uint32_t __phys_page_directory;

static task_t task_table[MAX_TASKS];
static task_t *current_task = 0;
static uint32_t next_pid = 1;

#define KERNEL_STACK_SIZE 4096
static uint8_t kernel_stacks[MAX_TASKS][KERNEL_STACK_SIZE];

void scheduler_init(void)
{
    for (uint32_t i = 0; i < MAX_TASKS; i++) {
        task_table[i].pid = 0;
        task_table[i].esp = 0;
        task_table[i].cr3 = 0;
        task_table[i].state = TASK_UNUSED;
        task_table[i].is_user = 0;
        task_table[i].user_eip = 0;
        task_table[i].user_esp = 0;
        task_table[i].kernel_stack_top =
            (uint32_t)&kernel_stacks[i][KERNEL_STACK_SIZE];
            
        task_table[0].esp = task_table[0].kernel_stack_top;
    }

    current_task = &task_table[0];
    current_task->pid = 0;
    current_task->state = TASK_RUNNING;
    current_task->cr3 = (uint32_t)&__phys_page_directory;
    tss.esp0 = current_task->kernel_stack_top;
    task_table[0].esp = task_table[0].kernel_stack_top;
}

task_t *scheduler_create_task(void (*entry)(void), int is_user,
                              uint32_t user_eip, uint32_t user_esp)
{
    for (uint32_t i = 1; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_UNUSED) {
            task_table[i].pid = next_pid++;
            task_table[i].state = TASK_READY;
            task_table[i].is_user = is_user;
            task_table[i].user_eip = user_eip;
            task_table[i].user_esp = user_esp;

            uint32_t *stack = (uint32_t *)task_table[i].kernel_stack_top;

           if (is_user) {
            *--stack = 0x23;               // user SS
            *--stack = user_esp;           // user ESP
            *--stack = 0x202;              // EFLAGS (IF=1)
            *--stack = 0x1B;               // user CS
            *--stack = user_eip;           // user EIP
        }
            else {
            *--stack = 0x202;              // EFLAGS
            *--stack = 0x08;               // CS (kernel)
            *--stack = (uint32_t)entry;    // EIP
        }

            // pushad frame (common)
            *--stack = 0; // EDI
            *--stack = 0; // ESI
            *--stack = 0; // EBP
            *--stack = 0; // original ESP (ignored)
            *--stack = 0; // EBX
            *--stack = 0; // EDX
            *--stack = 0; // ECX
            *--stack = 0; // EAX

            task_table[i].esp = (uint32_t)stack;
            task_table[i].cr3 = 0;   // caller sets
            return &task_table[i];
        }
    }
    return 0;
}

void scheduler_save_context(uint32_t *esp)
{
    current_task->esp = (uint32_t)esp;
}

task_t *scheduler_current(void) { return current_task; }

task_t *scheduler_next_task(void)
{
    int cur_idx = -1;
    for (int i = 0; i < MAX_TASKS; i++)
        if (&task_table[i] == current_task) { cur_idx = i; break; }
    if (cur_idx < 0) cur_idx = 0;

    for (int i = 1; i <= MAX_TASKS; i++) {
        int idx = (cur_idx + i) % MAX_TASKS;
        if (task_table[idx].state == TASK_READY)
            return &task_table[idx];
    }
    return current_task;
}

uint32_t *schedule(uint32_t *esp)
{
    scheduler_save_context(esp);
    if (current_task != &task_table[0])
        current_task->state = TASK_READY;

    task_t *next = scheduler_next_task();
    if (next->cr3 != 0) {
        uint32_t cur_cr3;
        asm volatile("mov %%cr3, %0" : "=r"(cur_cr3));
        if (next->cr3 != cur_cr3)
            asm volatile("mov %0, %%cr3" : : "r"(next->cr3) : "memory");
    }
    next->state = TASK_RUNNING;
    current_task = next;
    tss.esp0 = next->kernel_stack_top;
    return (uint32_t *)next->esp;
}