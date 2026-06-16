.code32
.global idt_load
.global keyboard_stub
.global isr_stub
.global timer_stub
.extern keyboard_handler
.extern isr_handler
.extern timer_handler

idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

keyboard_stub:
    pushal
    call keyboard_handler
    popal
    iretl

isr_stub:
    pushal
    call isr_handler
    popal
    iretl

timer_stub:
    pushal
    call timer_handler
    popal
    iretl

.global isr6_stub
    isr6_stub:
    pushal
    call isr_handler
    popal
    iretl

.global page_fault_stub
.extern page_fault_handler

page_fault_stub:
    pushal
    call page_fault_handler
    popal
    add $4, %esp
    iretl

.section .note.GNU-stack,"",@progbits