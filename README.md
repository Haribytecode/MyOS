# BlackPeak OS

<div align="center">

### A 32-bit x86 Monolithic Operating System Built From Scratch

**Higher-Half Kernel • Virtual Memory • Ring 3 • System Calls • Preemptive Multitasking**

</div>

---

## Overview

**BlackPeak OS** is a 32-bit x86 monolithic operating system developed from scratch using freestanding C and x86 assembly.

The project focuses on understanding operating-system internals by implementing core kernel subsystems directly on the **i386 architecture**, rather than relying on an existing operating-system kernel or runtime.

The kernel has evolved from a minimal GRUB-bootable system into a structured kernel foundation containing:

- Protected-mode initialization
- Higher-half kernel architecture
- GDT / IDT / TSS management
- Hardware interrupt handling
- Physical memory management
- Paging and virtual memory infrastructure
- Address-space management
- Preemptive multitasking
- Context switching
- Kernel and Ring 3 execution
- User/kernel privilege separation
- System-call infrastructure
- Device and debugging drivers
- Demand-paging foundation

Development is incremental and emphasizes low-level correctness, subsystem isolation, debugging, and validation under emulation.

---

## Architecture

```text
                    User Applications
                         Ring 3
                           │
                           │ INT 0x80
                           ▼
┌─────────────────────────────────────────────────────┐
│                  BLACKPEAK KERNEL                   │
│                         Ring 0                      │
│                                                     │
│  Scheduler     Memory Manager     System Calls      │
│  Task/Thread   Virtual Memory     Console           │
│  Interrupts    Drivers            Panic Handling    │
└─────────────────────────────────────────────────────┘
                           │
                           ▼
          GDT • TSS • IDT • PIC • PIT • Paging
                           │
                           ▼
                    x86 Hardware
```

# Core Features
Boot & Kernel Architecture
GRUB Multiboot boot process
32-bit Intel x86 / i386 architecture
Protected-mode kernel
Freestanding C environment
x86 assembly bootstrap code
Custom linker script
Higher-half kernel layout
Kernel initialization pipeline
Monolithic kernel architecture
Descriptor Tables & Protection
GDT
Global Descriptor Table initialization
Kernel code/data segments
User code/data segments
Privilege-level separation
IDT
Interrupt Descriptor Table
Exception handling infrastructure
Interrupt entry stubs
Page-fault interrupt entry
Hardware interrupt integration
TSS
Task State Segment
Kernel stack configuration for privilege transitions
Ring 3 → Ring 0 stack switching foundation
Interrupt & Hardware Support

BlackPeak OS contains a low-level x86 interrupt subsystem.

Implemented components include:

CPU exception infrastructure
Interrupt Descriptor Table
IRQ handling
PIC initialization
PIT timer
Keyboard interrupt handling
Interrupt service routines
Interrupt assembly stubs
UART serial diagnostics
VGA text console

The interrupt architecture provides the foundation required for timer-driven scheduling, keyboard input, system calls, and memory exceptions.

Memory Management

Memory management is one of the major subsystems of BlackPeak OS.

Physical Memory Management

Implemented:

Physical frame allocation
Bitmap-based physical frame management
Frame allocation API
Physical memory tracking
Paging

Implemented:

x86 paging
Page-directory management
Page-table management
Virtual-to-physical page mapping
Page permission management
User / supervisor page permissions
Page-present permissions
TLB invalidation
CR3 address-space switching
Higher-Half Paging

The kernel uses a higher-half memory layout, allowing the kernel to operate in its designated higher virtual address region while maintaining appropriate physical mappings during initialization.

Recursive Paging

Recursive page-directory mapping is used as part of the virtual-memory infrastructure to simplify access to page tables and page-directory structures.

Address Spaces

The kernel contains infrastructure for:

Per-process page directories
Address-space switching
Shared kernel mappings
User-space memory mappings
User stack mappings
Kernel/user memory separation
Demand Paging Foundation

BlackPeak OS also contains a demand-paging foundation.

The page-fault path contains the initial mechanism for handling eligible user-space page faults by:

Reading the faulting address from CR2
Inspecting the page-fault error code
Determining whether the fault represents an eligible non-present user page
Allocating a physical frame
Mapping the frame into the faulting virtual address
Initializing the newly mapped page

The current implementation is intentionally described as a foundation rather than claiming a complete production demand-paging subsystem.

This provides a base for future improvements such as complete virtual-memory fault policies, backing storage, page reclamation, and filesystem integration.

Process & Task Management

BlackPeak OS contains task and thread management infrastructure.

Implemented areas include:

Kernel task creation
Task structures
Thread support
Independent kernel stacks
Task state management
User task foundation
Address-space association
Context management
Preemptive Scheduler

The kernel implements timer-driven preemptive multitasking.

Scheduler Components
PIT-driven scheduling
Timer interrupts
Round-robin scheduling
Task queues
Context switching
Kernel task scheduling
User task scheduling
Independent task stacks

Execution flow:

PIT
 │
 ▼
Timer IRQ
 │
 ▼
Scheduler
 │
 ▼
Context Switch
 │
 ▼
Restored Task Context
 │
 ▼
IRET

This provides the foundation for concurrent kernel tasks and user-space execution.

Context Switching

The kernel contains a dedicated low-level context-switching subsystem implemented using x86 assembly and C.

The context subsystem is responsible for preserving and restoring execution state required for task switching.

The design separates:

Task management
Scheduler policy
Context representation
Low-level context switching

This keeps the scheduler architecture modular and allows future expansion.

Ring 3 User Mode

BlackPeak OS implements hardware-enforced user/kernel privilege separation using the x86 protection model.

Implemented infrastructure includes:

Ring 0 kernel execution
Ring 3 user execution
User code mapping
User stack mapping
User page permissions
TSS-based kernel stack switching
IRET-based privilege transition
Kernel/user address-space separation

Conceptually:

             Ring 3
        User Application
              │
              │ INT 0x80
              ▼
        ┌─────────────┐
        │   Ring 0    │
        │   Kernel    │
        └─────────────┘
System Calls

The kernel contains an x86 INT 0x80 system-call interface foundation.

Implemented components include:

INT 0x80 entry
System-call interrupt path
Kernel-side syscall dispatcher
User → kernel transition
Safe return path toward user execution

The syscall subsystem provides the foundation for future user-space services and applications.

Device & Debugging Support
VGA Console

The kernel provides VGA text-mode console output for direct machine/emulator diagnostics.

UART

Serial output is used extensively for low-level kernel debugging and diagnostics.

This is particularly useful when debugging:

Interrupts
Paging
Scheduler transitions
Task execution
Exceptions
Kernel initialization
Keyboard

The keyboard driver receives hardware keyboard interrupts and provides basic keyboard input handling.

PIC

The Programmable Interrupt Controller is initialized and integrated with the kernel interrupt subsystem.

PIT

The Programmable Interval Timer provides the timer source used by the preemptive scheduler.

Kernel Error Handling

BlackPeak OS contains kernel panic infrastructure for handling fatal kernel conditions.

Diagnostics can be routed through:

VGA console
UART serial output

This provides visibility during low-level kernel failures where conventional debugging facilities are unavailable.

Boot Pipeline

The kernel initialization flow can be represented approximately as:

GRUB Multiboot
      │
      ▼
Assembly Bootstrap
      │
      ▼
Protected Mode
      │
      ▼
Higher-Half Kernel Setup
      │
      ▼
GDT
      │
      ▼
Paging / Virtual Memory
      │
      ▼
TSS
      │
      ▼
IDT
      │
      ▼
PIC
      │
      ▼
PIT
      │
      ▼
Physical Memory Manager
      │
      ▼
Kernel Heap / Memory Services
      │
      ▼
Scheduler
      │
      ▼
Task / Thread Infrastructure
      │
      ▼
Ring 3 User Execution
      │
      ▼
INT 0x80 System Calls
Repository Structure
BlackPeakOS/
│
├── boot.S
├── linker.ld
├── Makefile
│
├── kernel.c
│
├── gdt.c
├── gdt.h
├── gdt_flush.s
│
├── idt.c
├── idt.h
├── idt_asm.s
├── isr.c
├── irq.c
├── interrupt.h
├── pic.c
├── pic.h
│
├── paging.c
├── paging.h
├── heap.c
├── heap.h
│
├── scheduler.c
├── scheduler.h
├── task.c
├── task.h
├── thread.c
├── thread.h
├── context.c
├── context.h
├── context_switch.s
│
├── timer.c
├── timer.h
│
├── tss.c
├── tss.h
├── tss_flush.s
│
├── syscall.c
│
├── user.c
├── user.h
│
├── console.c
├── console.h
├── keyboard.c
├── keyboard.h
├── uart.c
├── uart.h
│
├── panic.c
├── panic.h
│
├── io.h
│
├── iso_root/
│   └── boot/
│       └── grub/
│           └── grub.cfg
│
└── docs/

Generated build artifacts such as object files, kernel binaries, ISO images, logs, and backups are intentionally excluded from version control through .gitignore.

Build Environment

Typical development environment:

Linux
GCC
GNU Binutils
GRUB tools
xorriso
QEMU
VirtualBox
GDB
objdump
nm

The kernel is built as a freestanding 32-bit x86 target.

Example compilation flags include:

-m32
-std=gnu11
-ffreestanding
-fno-builtin
-fno-stack-protector
-nostdlib
Build & Run

Build the kernel using the project's Makefile:

make

Depending on the configured Makefile targets, an ISO image can then be generated and launched with:

qemu-system-i386 -cdrom kernel.iso -serial stdio

UART output is useful for observing low-level kernel diagnostics while VGA output provides direct console visibility.

Testing & Debugging

BlackPeak OS has been developed and tested incrementally using:

QEMU
Oracle VirtualBox
VGA console diagnostics
UART serial logging
GDB-compatible debugging workflow
objdump
nm
Kernel build/linker diagnostics

Low-level validation is performed while developing individual kernel subsystems rather than treating the kernel as a single monolithic implementation.

Engineering Approach

The project follows an incremental systems-development approach.

Major subsystems are developed independently and then integrated into the kernel.

Examples include:

Boot
  ↓
CPU / GDT
  ↓
Interrupts
  ↓
Physical Memory
  ↓
Paging
  ↓
Higher-Half Virtual Memory
  ↓
Tasks / Threads
  ↓
Scheduler
  ↓
TSS / Ring 3
  ↓
System Calls
  ↓
Demand Paging Foundation

The goal is not simply to produce a bootable image, but to understand the mechanisms that make an operating-system kernel work at the hardware/software boundary.

Current Status
Completed Kernel Foundations
 GRUB boot process
 32-bit protected-mode kernel
 Freestanding C environment
 Higher-half kernel
 Custom linker configuration
 GDT
 IDT
 TSS
 CPU exception infrastructure
 Hardware IRQ infrastructure
 PIC
 PIT
 Keyboard interrupt handling
 VGA console
 UART debugging
 Physical frame allocator
 Paging
 Higher-half paging
 Recursive page-directory mapping
 Virtual page mapping
 User/supervisor page permissions
 CR3 address-space switching
 Per-process page-directory infrastructure
 Shared kernel mappings
 Kernel heap foundation
 Task management
 Thread infrastructure
 Context switching
 PIT-driven preemptive scheduling
 Round-robin scheduler
 Kernel task execution
 Ring 3 execution foundation
 User stack mapping
 Kernel/user privilege separation
 INT 0x80 syscall infrastructure
 Demand paging foundation
Roadmap

The current kernel provides the foundation for a more complete operating-system environment.

Next Major Subsystems
 Virtual File System (VFS)
 Filesystem abstraction layer
 TARFS / initial filesystem support
 ELF executable loader
 User-space program execution
 Expanded system-call API
 More complete demand-paging implementation
 Page-backed storage
 Additional device drivers
 Improved process management
 Richer user-space environment
Long-Term Direction
64-bit x86-64 architecture
UEFI boot support
More advanced filesystem support
Expanded user-space services
Additional hardware drivers
More complete POSIX-like facilities
Why This Project?

Building an operating system from scratch provides practical experience with areas that are difficult to understand from application-level programming alone:

CPU privilege levels
Interrupt handling
Virtual memory
Address translation
Context switching
Scheduling
Kernel/user boundaries
Hardware interfaces
ABI design
Low-level debugging
Linkers and executable memory layouts

BlackPeak OS is therefore both a kernel-development project and a systems-programming learning platform.

Technical Focus

Architecture: x86 / i386
Kernel: Monolithic
Language: Freestanding C + x86 Assembly
Boot: BIOS / GRUB Multiboot
Memory: Paging + Higher-Half Virtual Memory
Scheduling: Preemptive Round-Robin
Privilege Levels: Ring 0 / Ring 3
Syscall Interface: INT 0x80
Debugging: UART + VGA + GDB-compatible workflow
License: MIT

Author

Hariharan J

Systems Programming • Operating Systems • Kernel Development • Low-Level Software

License

MIT License.

Final Note

BlackPeak OS is an evolving operating-system kernel developed from first principles.

The project began as a minimal x86 kernel and has progressively expanded into a structured kernel containing memory management, virtual memory, task management, preemptive scheduling, hardware interrupt handling, user/kernel isolation, system-call infrastructure, and a demand-paging foundation.

The next stage focuses on filesystem infrastructure, executable loading, and a richer user-space environment.

The project prioritizes understanding the underlying mechanisms of operating systems, incremental implementation, low-level debugging, and architectural clarity over simply producing a bootable demo.
