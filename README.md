# MyOS

**MyOS** is a minimal x86 operating system built from scratch as a learning and research project.
It boots via **GRUB**, runs its **own kernel**, handles **hardware interrupts**, supports
**preemptive multitasking**, and includes **basic physical memory management**.

> Architecture: **x86 (32-bit, i386)**  
> Boot method: **BIOS + GRUB**  
> Status: **v1.0 (Mini Functional OS)**

---

## Features

- Boots from **ISO via GRUB**
- Custom **x86 kernel**
- **GDT / IDT** setup
- **Hardware interrupts** (PIT timer, keyboard)
- **Preemptive multitasking**
- **Physical Memory Management (PMM)**
- **UART (serial) output** and **VGA text output**
- Kernel panic handling

---

## Project Structure

MyOS/
├─ boot.S
├─ linker.ld / link.ld
├─ kernel.c
├─ gdt.*
├─ idt.*
├─ isr.* / irq.*
├─ pic.*
├─ task.* / process.S
├─ keyboard.*
├─ uart.*
├─ panic.*
├─ iso_root/
│ └─ boot/grub/grub.cfg
├─ Makefile
└─ myos.iso



---

## Build & Run

### Requirements
- GCC (i386-elf recommended)
- GRUB tools
- QEMU or VirtualBox
- Linux / WSL (Ubuntu)

### Build
```bash
make
Run (QEMU)
bash
Copy code
qemu-system-i386 -cdrom myos.iso
Limitations (v1.0)
32-bit x86 (BIOS-based)

No virtual memory yet

No filesystem or syscalls yet

USB boot on UEFI-only hardware not supported

Roadmap
Virtual Memory (Paging)

System calls

Simple filesystem

User programs

64-bit UEFI support (future)

License
MIT License.

Author
Built by Hariharan J as a systems programming and OS development project.


