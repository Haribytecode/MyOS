.global switch_task

# switch_task(task_t *old, task_t *new)
switch_task:
    # 1. Get arguments from the stack (GCC default convention)
    mov 4(%esp), %eax    # old task pointer is at ESP + 4
    mov 8(%esp), %edx    # new task pointer is at ESP + 8

    # 2. Save callee-saved registers (C calling convention)
    push %ebp
    push %ebx
    push %esi
    push %edi

    # 3. Perform the stack swap
    mov %esp, (%eax)     # Save old stack pointer to old->esp
    mov (%edx), %esp     # Load new stack pointer from new->esp

    # 4. Restore registers for the NEW task
    pop %edi
    pop %esi
    pop %ebx
    pop %ebp

    # 5. Return to the new task's code
    ret