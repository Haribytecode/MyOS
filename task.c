#include "task.h"
#include "uart.h"
#include <stdint.h>
#include "sync.h"  
#define STACK_SIZE 4096

static uint8_t stack_a[STACK_SIZE];
static uint8_t stack_b[STACK_SIZE];

static task_t tasks[2];
static int current = 0;

void task_a() {
    volatile int counter = 0;
    while(1) { 
        counter++;
    }
}
void task_b(void) {
    __asm__ volatile("sti"); 
    while (1) {
     
        for (volatile int i = 0; i < 5000000; i++);
    }
}
static void init_task(task_t* task, uint8_t* stack, void (*entry)(void)) {
    uint32_t* sp = (uint32_t*)(stack + STACK_SIZE);

    *(--sp) = (uint32_t)entry; 
  
    *(--sp) = 0; 
    *(--sp) = 0; 
    *(--sp) = 0; 
    *(--sp) = 0; 

    task->esp = (uint32_t)sp;
}


void schedule_init(void) {
    init_task(&tasks[0], stack_a, task_a);
    init_task(&tasks[1], stack_b, task_b);
    current = 0;
}

task_t* current_task(void) {
    return &tasks[current];
}

task_t* next_task(void) {
    current = (current + 1) % 2;
    return &tasks[current];
}
