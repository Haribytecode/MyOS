#ifndef TIMER_H
#define TIMER_H

#include "interrupt.h"

interrupt_frame_t *timer_handler(interrupt_frame_t *frame);

#endif