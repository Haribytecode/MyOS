CC = gcc
LD = ld

CFLAGS = -m32 -std=gnu11 -O2 -ffreestanding -fno-builtin -Wall -Wextra -nostdlib
LDFLAGS = -m elf_i386 -T linker.ld

# Objects must include idt_asm.o and process.o
OBJS = boot.o idt_asm.o kernel.o gdt.o idt.o isr.o panic.o pic.o keyboard.o uart.o task.o process.o sync.o

all: kernel.elf

# This rule forces 32-bit mode for both .S and .s files
%.o: %.S
	$(CC) -m32 -ffreestanding -c $< -o $@

%.o: %.s
	$(CC) -m32 -ffreestanding -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJS)

# Use this to build the final ISO
iso: kernel.elf
	cp kernel.elf iso_root/boot/kernel.elf
	grub-mkrescue -o myos.iso iso_root

clean:
	rm -f *.o kernel.elf myos.iso