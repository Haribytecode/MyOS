# MyOS Architecture

This document describes the high-level architecture of **MyOS v1.0**, a minimal
32-bit x86 operating system built from scratch.

---

## Target Architecture

- CPU: x86 (i386, 32-bit)
- Boot mode: BIOS
- Bootloader: GRUB
- Execution environment: QEMU / VirtualBox

---

## Boot Flow

1. System starts in **BIOS mode**
2. BIOS loads **GRUB** from the bootable ISO
3. GRUB loads `kernel.elf` into memory
4. Control is transferred to the kernel entry point
5. Kernel initializes core subsystems
6. Kernel enters main loop / scheduler

---

## Kernel Layout

The kernel is a **monolithic kernel**. All core subsystems run in kernel space.

Main components:
- Boot code (`boot.S`)
- Kernel entry (`kernel.c`)
- Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Interrupt Service Routines (ISR / IRQ)
- Scheduler and task switching
- Basic physical memory management
- Device drivers (keyboard, timer, serial)

---

## Interrupt Handling

- PIC is remapped to avoid CPU exception conflicts
- Hardware interrupts handled:
  - IRQ0: PIT timer (used for scheduling)
  - IRQ1: Keyboard input
- IDT entries point to assembly stubs
- Stubs call C-level interrupt handlers

---

## Scheduling Model

- Preemptive multitasking
- Timer interrupt triggers task switch
- Simple round-robin scheduler
- Context switching implemented in assembly

---

## Memory Management (v1.0)

- Physical memory only
- Simple bump allocator
- Page-aligned allocations (4 KB)
- Virtual memory planned for future versions

---

## Input / Output

- Serial output via UART (COM1) for debugging
- VGA text mode output for on-screen display
- Keyboard input via IRQ1

---

## Design Philosophy

- Minimal and educational
- No external libraries
- Clear separation of subsystems
- Incremental development

---

## Future Extensions

- Virtual memory (paging)
- System calls
- Filesystem
- User mode programs
- 64-bit UEFI support
