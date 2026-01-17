// IRQ subsystem
// IRQ0: PIT timer (scheduler tick)   switching is done using this PIT which forces task A to stop and run task B by switching //
// IRQ1: Keyboard input
// PIC remapped to vectors 32–47 [Master ,slave port] 

void irq_handler(int irq){
    if(irq==1){
        timer_handler();
    }
    else if(irq==0){
        keyboard_handler();
    }
}

// CPU has one chip to handle programmable interrupt controller so CPU has totally 8 pins to handle//
//PIC contains two ports //
//Master port,slave port//
//IRQ0->TIMER,IRQ1->KEYBOARD,IRQ2->[SLAVE PORT(contains 8 to 15[Handles 8 interrupts])]// //IRQ 12 is Mouse
//EOI is important outb->output byte ,outb(command port,data port) //
///////0 to 31 are cpu defined exceptions,32 to 255 are user defined interrupts handled by OS via IDT///////