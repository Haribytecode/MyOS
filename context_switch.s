.code32

.global switch_context
.global kernel_thread_bootstrap
.global enter_user_mode_v2 

.extern panic
.extern tss

# --- ABI BOUNDARY CONTRACT OFFSETS ---
.set THREAD_KERNEL_STACK_TOP, 4
.set THREAD_KERNEL_ESP,       8
.set TSS_ESP0,                4

/*
 * void switch_context(thread_t *old, thread_t *new)
 */
switch_context:
    # 1. Save callee-saved registers onto current stack
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ebp

    # 2. old->kernel_esp = current ESP
    movl 20(%esp), %eax
    movl %esp, THREAD_KERNEL_ESP(%eax)

    # 3. ESP = new->kernel_esp
    movl 24(%esp), %eax
    movl THREAD_KERNEL_ESP(%eax), %esp

    # 4. CRITICAL MILESTONE: tss.esp0 = new->kernel_stack_top
    movl THREAD_KERNEL_STACK_TOP(%eax), %edx
    movl %edx, tss + TSS_ESP0

    # 5. Restore callee-saved registers from the NEW stack
    popl %ebp
    popl %edi
    popl %esi
    popl %ebx

    # 6. Jump cleanly to the new thread's saved entry address
    ret

/*
 * Execution lands here on a thread's very first context switch
 */
kernel_thread_bootstrap:
    pushl %edi
    call *%esi
    addl $4, %esp

    pushl $panic_msg
    call panic

.halt_loop:
    cli
    hlt
    jmp .halt_loop

/*
 * void enter_user_mode(uint32_t user_eip, uint32_t user_esp);
 *
 * Stack on entry:
 *  esp+4  = user_eip
 *  esp+8  = user_esp
 */
enter_user_mode_v2:
    # 1. Clean the segment registers using User Data Selector (0x23)
    movw $0x23, %cx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs

    # 2. Build the hardware privilege-transition frame expected by IRET
    # %edx contains user_esp, %eax contains user_eip
    pushl $0x23             # User SS (Data Segment Selector 4 | RPL 3)
    pushl %edx              # User ESP
    pushl $0x00000002
    pushl $0x1B             # User CS (Code Segment Selector 3 | RPL 3)
    pushl %eax              # User EIP

    # 3. Take the verified hardware leap of faith into Ring 3!
    iret
    
.section .rodata
panic_msg:
    .string "Fatal: Kernel thread returned into bootstrap closure."

.section .note.GNU-stack,"",@progbits
