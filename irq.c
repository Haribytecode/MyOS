extern void timer_handler(void);
extern void keyboard_handler(void);


void irq_handler(int irq)
{
    if (irq == 0)
    {
        timer_handler();
    }
    else if (irq == 1)
    {
        keyboard_handler();
    }
}