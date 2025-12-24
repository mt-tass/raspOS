#include <stdint.h>
#ifndef TIMER_H
#define TIMER_H

uint64_t timer_get_ticks(void);
void delay_ms(uint32_t time);
uint32_t timer_get_seconds(void);
#endif