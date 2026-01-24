#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct {
    uint32_t esp;
} task_t;

void schedule_init(void);
task_t* current_task(void);
task_t* next_task(void);
void switch_task(task_t* old, task_t* next);


void task_a(void); 

#endif
