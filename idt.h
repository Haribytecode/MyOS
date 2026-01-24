#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* * This function sets up the IDT table structure, 
 * fills it with default handler functions, and then loads 
 * pointer into CPU using assembly function.
 */
void idt_init(void);

#endif

