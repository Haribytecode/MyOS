#include "task.h"
#include "uart.h"
#include <stdint.h>
#include "sync.h"   // <--- MUST BE AT THE TOP
#define STACK_SIZE 4096

static uint8_t stack_a[STACK_SIZE];
static uint8_t stack_b[STACK_SIZE];

static task_t tasks[2];
static int current = 0;

void task_a() {
    volatile int counter = 0;
    while(1) { 
        counter++; // Just do some invisible work
    }
}
void task_b(void) {
    __asm__ volatile("sti"); 
    while (1) {
        // safe_uart_puts("B"); <--- REMOVE OR COMMENT THIS OUT
        for (volatile int i = 0; i < 5000000; i++);
    }
}
static void init_task(task_t* task, uint8_t* stack, void (*entry)(void)) {
    uint32_t* sp = (uint32_t*)(stack + STACK_SIZE);

    // We are simulating what 'switch_task' expects to see on the stack.
    // When switch_task runs 'ret', it pops EIP.
    *(--sp) = (uint32_t)entry;  // EIP (Return Address)

    // When switch_task runs 'pop', it expects these 4 registers:
    *(--sp) = 0; // ebp
    *(--sp) = 0; // ebx
    *(--sp) = 0; // esi
    *(--sp) = 0; // edi

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