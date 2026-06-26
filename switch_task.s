.global switch_task

switch_task:
    mov 4(%esp), %eax
    mov %eax, %esp
    ret

