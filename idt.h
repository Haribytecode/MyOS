#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* * This function sets up the IDT structure, 
 * fills it with default handlers, and loads 
 * the pointer into the CPU using the assembly function.
 */
void idt_init(void);

#endif

