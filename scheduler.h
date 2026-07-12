#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 32

typedef enum {
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING
} task_state_t;

typedef struct task {
    uint32_t pid;
    uint32_t esp;              // kernel stack pointer
    uint32_t cr3;
    task_state_t state;

    int is_user;               // 1 = ring‑3 task
    uint32_t user_eip;
    uint32_t user_esp;

    uint32_t kernel_stack_top;

    // needed by context_switch.s
    uint32_t edi, esi, ebp, ebx, edx, ecx, eax;
    uint32_t eip, cs, eflags;
    uint32_t user_ss;

    struct task *next;
} task_t;

void scheduler_init(void);
uint32_t *schedule(uint32_t *esp);
void scheduler_save_context(uint32_t *esp);
task_t *scheduler_next_task(void);
task_t *scheduler_current(void);
task_t *scheduler_create_task(void (*entry)(void), int is_user,
                              uint32_t user_eip, uint32_t user_esp);
#endif