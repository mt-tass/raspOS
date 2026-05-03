#include "timer.h"

#define TIMER_BASE 0x3F003000
#define TIMER_CS (TIMER_BASE)
#define TIMER_CLOW (TIMER_BASE + 4)
#define TIMER_CHIGH (TIMER_BASE + 8)
#define TIMER_COMP0 (TIMER_BASE + 12)
#define TIMER_COMP1 (TIMER_BASE + 16)
#define TIMER_COMP2 (TIMER_BASE +20)
#define TIMER_COMP3 (TIMER_BASE +24)
#define REG(addr) (*(volatile uint32_t *)(addr))

uint64_t timer_get_ticks(void){
    while(1){
        uint32_t timer_high1 = REG(TIMER_CHIGH);
        uint32_t timer_low = REG(TIMER_CLOW);
        uint32_t timer_high2 = REG(TIMER_CHIGH);
        if (timer_high1 == timer_high2){
            uint64_t ticks = ((uint64_t)timer_high1  << 32) |(timer_low);
            return ticks;
        }
    }
    
}
void delay_ms(uint32_t time){
    uint64_t start_time = timer_get_ticks();
    while((timer_get_ticks()-start_time) < ((uint64_t)time)*1000){
        continue;
    }
    return;
}
uint32_t timer_get_seconds(void){
    return (uint32_t)timer_get_ticks()/1000000;
}