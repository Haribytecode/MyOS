#include <stdint.h>
#include "idt.h"

struct idt_entry { uint16_t offset_low; uint16_t selector; uint8_t zero; uint8_t type_attr; uint16_t offset_high; } __attribute__((packed));
struct idt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

static void idt_set_gate(uint8_t n, uint32_t handler){
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

extern void idt_load(uint32_t);
extern void isr_stub(void);
extern void timer_stub(void);
extern void keyboard_stub(void);

void idt_init(void){
    idtp.limit = sizeof(idt)-1;
    idtp.base = (uint32_t)&idt;

    for(int i=0;i<256;i++) idt_set_gate(i,(uint32_t)isr_stub);

    idt_set_gate(32,(uint32_t)timer_stub);
    idt_set_gate(33,(uint32_t)keyboard_stub);

    idt_load((uint32_t)&idtp);
}
