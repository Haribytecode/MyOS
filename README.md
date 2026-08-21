# BlackPeak OS

<div align="center">

### A 32-bit x86 monolithic operating system kernel built from scratch

**Higher-Half Kernel · Virtual Memory · Per-Process Address Spaces · Ring 3 · System Calls · Preemptive Multitasking**

</div>

---

## Overview

**BlackPeak OS** is a 32-bit x86 monolithic operating system developed from scratch in freestanding C and x86 assembly.

The project focuses on understanding and implementing the mechanisms that form the foundation of an operating system: processor protection, interrupt handling, virtual memory, physical memory management, address-space isolation, task scheduling, context switching, kernel memory allocation, user-mode execution, and system calls.

Rather than relying on an existing kernel or operating-system framework, BlackPeak OS implements these mechanisms directly against the **i386 architecture** and validates them incrementally under emulation and low-level debugging tools.

The current `integrated-stepwise` branch represents a substantial evolution from the original minimal bootable kernel, with the kernel now providing:

- A higher-half kernel layout
- x86 protected-mode execution
- GDT, IDT, and TSS infrastructure
- Hardware interrupt handling
- Paging and recursive page-directory mapping
- Physical frame allocation
- Dynamic virtual memory mapping
- Per-process address spaces
- CR3 address-space switching
- Shared kernel mappings
- User/supervisor page permissions
- TLB invalidation
- A demand-paging foundation
- Kernel heap allocation
- Preemptive multitasking
- Task and thread management
- Context switching
- Ring 3 user-mode execution
- TSS-based kernel stack switching
- `INT 0x80` system-call entry
- VGA and UART output
- Keyboard input
- PIC and PIT drivers
- Kernel panic handling

The project is intentionally developed incrementally: each subsystem is introduced, tested, debugged, and integrated into the larger kernel architecture.

---

## Engineering Focus

BlackPeak OS is primarily a systems-programming project.

The implementation explores:

- x86 protected-mode architecture
- CPU privilege levels
- Interrupt and exception handling
- Descriptor tables
- Hardware task-state mechanisms
- Physical memory management
- Virtual memory
- Page tables and page directories
- Address-space isolation
- Kernel/user memory permissions
- Context switching
- Timer-driven scheduling
- Kernel stack management
- System-call boundaries
- Low-level device I/O
- Kernel heap management
- Debugging kernels without an existing operating system underneath

---

# Architecture

```text
                         User Applications
                              Ring 3
                                │
                                │ INT 0x80
                                │
                                ▼
┌───────────────────────────────────────────────────────┐
│                  BLACKPEAK KERNEL                     │
│                         Ring 0                        │
│                                                       │
│  Scheduler        Virtual Memory       System Calls   │
│  Tasks/Threads    Physical Memory      Console        │
│  Context Switch   Kernel Heap          Panic Handler  │
│  Interrupts       Address Spaces       Drivers        │
└───────────────────────────────────────────────────────┘
                                │
                                ▼
              GDT · TSS · IDT · PIC · PIT · Paging
                                │
                                ▼
                         x86 Hardware
```

### Kernel organization

The kernel is structured around several cooperating subsystems:

```text
Boot / Initialization
        │
        ├── GDT
        ├── TSS
        ├── IDT
        ├── PIC
        ├── PIT
        │
        ▼
Memory Subsystem
        │
        ├── Physical Frame Allocator
        ├── Paging
        ├── Virtual Page Mapping
        ├── Recursive Mapping
        ├── Address Spaces
        ├── CR3 Switching
        └── Demand Paging Foundation
        │
        ▼
Execution Subsystem
        │
        ├── Tasks
        ├── Threads
        ├── Context Management
        ├── Context Switching
        └── Preemptive Scheduler
        │
        ▼
User/Kernel Boundary
        │
        ├── Ring 3
        ├── User Address Spaces
        ├── TSS Kernel Stack Switching
        └── INT 0x80 System Calls
```

---

# Feature Matrix

## Boot and Kernel

| Feature | Status |
|---|---|
| GRUB Multiboot boot | ✅ |
| BIOS boot path | ✅ |
| x86 assembly bootstrap | ✅ |
| 32-bit i386 protected mode | ✅ |
| Freestanding C kernel | ✅ |
| Custom linker script | ✅ |
| Higher-half kernel layout | ✅ |
| Kernel initialization pipeline | ✅ |
| Monolithic kernel architecture | ✅ |

---

## CPU Protection and Descriptor Tables

| Feature | Status |
|---|---|
| Global Descriptor Table (GDT) | ✅ |
| Kernel code/data segments | ✅ |
| User code/data segments | ✅ |
| Interrupt Descriptor Table (IDT) | ✅ |
| Task State Segment (TSS) | ✅ |
| Ring 0 execution | ✅ |
| Ring 3 execution | ✅ |
| Ring 0 ↔ Ring 3 transitions | ✅ |
| TSS-based kernel stack switching | ✅ |

---

## Interrupts and Exceptions

| Feature | Status |
|---|---|
| IDT initialization | ✅ |
| ISR infrastructure | ✅ |
| Exception handling | ✅ |
| IRQ infrastructure | ✅ |
| Programmable Interrupt Controller (PIC) | ✅ |
| Programmable Interval Timer (PIT) | ✅ |
| Hardware timer interrupts | ✅ |
| Keyboard interrupts | ✅ |
| Interrupt-driven preemption | ✅ |
| Kernel panic handling | ✅ |

---

# Memory Management

Memory management is one of the core components of BlackPeak OS.

The kernel implements both **physical memory management** and **virtual memory management**, providing the foundation required for isolated processes and future user-space expansion.

### Physical Memory

- Bitmap-based physical frame allocation
- Physical frame tracking
- Page-frame allocation for virtual memory
- Separation between physical frame management and virtual mappings

### Virtual Memory

- 32-bit x86 paging
- Page directory management
- Page table management
- Higher-half kernel mappings
- Recursive page-directory mapping
- Dynamic virtual page mapping
- Page permission management
- User/supervisor page permissions
- Read/write permission control
- TLB invalidation
- CR3 address-space switching
- Per-process page directories
- Shared kernel mappings
- Independent process address spaces

### Demand Paging Foundation

BlackPeak OS also contains the **foundation for demand paging**.

The page-fault path is integrated with the interrupt subsystem so that page faults can be detected and routed through the kernel's memory-management infrastructure.

This provides the foundation for future expansion toward fully realized demand-paged user processes and lazy page allocation.

> **Important:** this is described as a *demand-paging foundation*, not as a claim of a complete production-grade demand-paging implementation.

---

# Address Spaces and Process Isolation

The kernel supports separate virtual address spaces for processes.

The design includes:

```text
Process A
    │
    └── Page Directory A
            │
            ├── User mappings
            └── Shared kernel mappings


Process B
    │
    └── Page Directory B
            │
            ├── User mappings
            └── Shared kernel mappings
```

The CPU's `CR3` register is switched when changing address spaces.

This establishes hardware-enforced separation between user memory and kernel memory and provides the architectural foundation for independent user processes.

---

# Kernel Heap

BlackPeak OS includes a kernel heap allocator for dynamic kernel memory management.

The heap implementation includes:

- Dynamic kernel allocations
- Free-block management
- Block splitting
- Block coalescing
- Kernel-side heap management

This provides dynamic allocation capabilities without relying on a host operating system's allocator.

---

# Scheduling and Multitasking

BlackPeak OS implements **timer-driven preemptive multitasking**.

### Scheduler capabilities

- Round-robin scheduling
- Kernel task scheduling
- Thread/task management
- Context management
- Context save and restore
- Assembly-level context switching
- Independent task execution contexts
- PIT-driven scheduling
- Timer interrupt preemption
- Ring 3 task scheduling

The basic execution path is:

```text
PIT
 │
 ▼
Timer IRQ
 │
 ▼
Interrupt Handler
 │
 ▼
Scheduler
 │
 ▼
Select Next Task
 │
 ▼
Context Switch
 │
 ▼
IRET / Resume Execution
```

The scheduler and task infrastructure are separated from the low-level assembly context-switch mechanism, allowing the execution model to evolve independently from the CPU-specific switching code.

---

# User Mode and Privilege Separation

BlackPeak OS uses the x86 protection rings to establish a hardware-enforced user/kernel boundary.

The kernel provides:

- Ring 3 execution
- User code mappings
- User stack mappings
- User address spaces
- User/supervisor page permissions
- TSS kernel stack switching
- Controlled transition into kernel mode
- Return to user mode using `IRET`

Conceptually:

```text
                 RING 3
        ┌─────────────────────┐
        │    User Process     │
        │                     │
        │ Code + User Stack   │
        └──────────┬──────────┘
                   │
                   │ INT 0x80
                   ▼
                 RING 0
        ┌─────────────────────┐
        │       Kernel        │
        │                     │
        │ Scheduler           │
        │ Memory Manager      │
        │ System Calls        │
        │ Drivers             │
        └─────────────────────┘
```

This is a major architectural step beyond a kernel that simply runs everything in Ring 0.

---

# System Calls

BlackPeak OS provides a dedicated system-call entry mechanism using:

```text
INT 0x80
```

The system-call path includes:

- User-mode system-call entry
- Kernel-side syscall dispatcher
- Controlled transition into Ring 0
- Kernel-side syscall handling
- Safe return to user mode
- `IRET`-based return path

The syscall subsystem provides the foundation for expanding the user/kernel API as more user-space functionality is introduced.

---

# Interrupt and Hardware Subsystems

BlackPeak OS directly interacts with the underlying x86 hardware environment.

### Implemented hardware infrastructure

**PIC**

- Programmable Interrupt Controller initialization
- IRQ routing infrastructure
- Hardware interrupt delivery

**PIT**

- Programmable Interval Timer configuration
- Timer interrupts
- Scheduler tick source
- Preemptive scheduling trigger

**Keyboard**

- Keyboard interrupt handling
- Low-level keyboard input
- Hardware I/O interaction

**VGA**

- VGA text-mode console
- Kernel diagnostic output
- Early boot and runtime debugging

**UART**

- Serial output
- Kernel debugging
- Emulation-based diagnostics

---

# Boot Pipeline

The kernel initialization follows a staged low-level boot process:

```text
GRUB
 │
 ▼
Multiboot Entry
 │
 ▼
x86 Assembly Bootstrap
 │
 ▼
Protected Mode
 │
 ▼
Kernel Entry
 │
 ▼
GDT
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
Paging / Virtual Memory
 │
 ▼
Kernel Heap
 │
 ▼
Task / Thread Infrastructure
 │
 ▼
Scheduler
 │
 ▼
Ring 3 User Execution
 │
 ▼
INT 0x80 System Calls
```

The exact initialization dependencies are encoded in the kernel source and build system rather than being delegated to an existing operating-system runtime.

---

# Repository Structure

The repository is intentionally organized around individual kernel subsystems.

```text
BlackPeakOS/
│
├── boot.S
├── kernel.c
├── linker.ld
├── link.ld
│
├── gdt.c
├── gdt.h
├── gdt_flush.s
│
├── idt.c
├── idt.h
├── idt_asm.s
│
├── tss.c
├── tss.h
├── tss_flush.s
│
├── isr.c
├── irq.c
├── interrupt.h
│
├── pic.c
├── pic.h
├── timer.c
├── timer.h
│
├── paging.c
├── paging.h
│
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
├── syscall.c
│
├── user.c
├── user.h
│
├── keyboard.c
├── keyboard.h
│
├── console.c
├── console.h
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
├── docs/
│
├── Makefile
├── LICENSE
└── README.md
```

---

# Build and Run

## Requirements

A Linux development environment with the following tools:

- GCC
- GNU Binutils
- GRUB tools
- `xorriso`
- QEMU
- Make

The kernel targets **32-bit x86 / i386**.

## Build

```bash
make clean
make
cp kernel.elf iso_root/boot/kernel.elf
grub2-mkrescue -o kernel.iso iso_root
qemu-system-i386 -cdrom kernel.iso
```

## Run with QEMU

```bash
qemu-system-i386 -cdrom kernel.iso -serial stdio
```

The serial output is particularly useful for kernel diagnostics and debugging.

---

# Testing and Validation

BlackPeak OS is developed and validated incrementally rather than treating the kernel as a single black box.

Testing and debugging have included:

- QEMU
- Oracle VirtualBox
- Bochs
- VGA diagnostics
- UART serial logging
- `objdump`
- `nm`
- GDB-compatible debugging workflows
- Boot-time diagnostics
- Interrupt-path debugging
- Memory-management validation
- Scheduler/context-switch validation

The repository also contains testing artifacts documenting emulator-based validation.

---

# Development Philosophy

The project follows a **bottom-up systems-development approach**.

Instead of immediately building user applications or filesystem abstractions, the kernel establishes the mechanisms underneath them first:

```text
CPU Protection
      ↓
Interrupts
      ↓
Physical Memory
      ↓
Virtual Memory
      ↓
Address Spaces
      ↓
Task / Thread Execution
      ↓
Preemptive Scheduling
      ↓
User Mode
      ↓
System Calls
      ↓
Future User-Space Services
```

This approach makes each subsystem independently understandable while allowing the pieces to form a coherent operating-system architecture.

---

# Current Status

BlackPeak OS has progressed substantially beyond its original minimal-kernel stage.

### Implemented

- [x] GRUB Multiboot boot
- [x] 32-bit i386 protected mode
- [x] Freestanding C kernel
- [x] Custom linker layout
- [x] Higher-half kernel
- [x] GDT
- [x] IDT
- [x] TSS
- [x] Ring 3 execution
- [x] Ring 0 ↔ Ring 3 transitions
- [x] Hardware interrupt infrastructure
- [x] PIC
- [x] PIT
- [x] Keyboard interrupts
- [x] VGA console
- [x] UART serial debugging
- [x] Physical frame allocator
- [x] Paging
- [x] Higher-half paging
- [x] Recursive page-directory mapping
- [x] Dynamic virtual page mapping
- [x] User/supervisor page permissions
- [x] TLB invalidation
- [x] CR3 switching
- [x] Per-process page directories
- [x] Shared kernel mappings
- [x] User address spaces
- [x] Demand-paging foundation
- [x] Kernel heap
- [x] Heap block splitting
- [x] Heap block coalescing
- [x] Task management
- [x] Thread infrastructure
- [x] Context management
- [x] Assembly context switching
- [x] Round-robin scheduling
- [x] PIT-driven preemption
- [x] Kernel task scheduling
- [x] Ring 3 task scheduling
- [x] `INT 0x80` system-call entry
- [x] System-call dispatcher
- [x] Kernel panic handling

---

# Roadmap

The current kernel provides the foundation for higher-level operating-system functionality.

### Next stages

- [ ] Expand the system-call API
- [ ] Complete demand-paging behavior
- [ ] ELF executable loading
- [ ] User-space program loading
- [ ] Virtual File System (VFS)
- [ ] Filesystem implementation
- [ ] Persistent storage drivers
- [ ] More device drivers
- [ ] Richer user-space process model
- [ ] Expanded process lifecycle management

The roadmap is intentionally layered on top of the existing kernel primitives rather than bypassing them.

---

# Why This Project Matters

BlackPeak OS is not intended to compete with mature operating systems such as Linux or BSD.

Its purpose is different:

**to implement and understand the mechanisms that make an operating system work.**

The project requires reasoning about:

- CPU privilege transitions
- Page-table structures
- Physical-to-virtual address translation
- CR3 and address-space switching
- Interrupt entry and return paths
- Kernel stack management
- Context preservation
- Scheduler state
- User/kernel memory boundaries
- Hardware timer preemption
- System-call entry
- Low-level device I/O

Working at this level provides practical experience with the boundary between software and hardware that is difficult to obtain through conventional application development alone.

---

# References

- Intel® 64 and IA-32 Architectures Software Developer's Manual
- OSDev Wiki
- GRUB Multiboot Specification

---

# License

MIT License.

---

# Author

**Hariharan J**

Systems Programming · Operating Systems · Kernel Development · x86 Architecture

---

<div align="center">

### BlackPeak OS

**From bootloader to Ring 3 — building the kernel from the hardware boundary upward.**

</div>
