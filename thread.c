#include "thread.h"
#include "context.h"

extern void kernel_thread_bootstrap(void);

void thread_init(thread_t *thread, uint32_t tid, void (*fn)(void *), void *arg)
{
    thread->tid = tid;
    
    // Move stack location down into the known, identity-mapped kernel memory zone.
    // This guarantees the page is writable and present, preventing a Page Fault.
    thread->kernel_stack_top = 0x001A0000 + (tid * 4096);

    // Map the context structure footprint onto the stack space
    context_t *ctx = (context_t *)(thread->kernel_stack_top - sizeof(context_t));

    // Forge the initial register values to survive the first context switch
    ctx->ebp = 0;
    ctx->edi = (uint32_t)arg;  // Kept intact for kernel_thread_bootstrap
    ctx->esi = (uint32_t)fn;   // Kept intact for kernel_thread_bootstrap
    ctx->ebx = 0;
    ctx->eip = (uint32_t)kernel_thread_bootstrap; // Where 'ret' jumps to

    // Save the adjusted stack pointer into the thread control block
    thread->kernel_esp = (uint32_t)ctx;

    thread->state = THREAD_READY;
}
