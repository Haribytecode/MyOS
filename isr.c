#include "panic.h"
#include "keyboard.h"

extern void timer_handler(void);

void isr_handler(void) {

    //calling timer_handler function 
   
    timer_handler();
}
