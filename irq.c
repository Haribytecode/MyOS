
// PIC remapped to the vectors 32–47 [Master port and slave port] 

void irq_handler(int irq){
    if(irq==1){
        timer_handler();
    }
    else if(irq==0){
        keyboard_handler();
    }
}
