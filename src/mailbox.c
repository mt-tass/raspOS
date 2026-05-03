// mailbox.c - CORRECTED VERSION

#include "mailbox.h"
#include "timer.h"

#define MAILBOX_BASE 0x3F00B880
#define MAILBOX_READ (MAILBOX_BASE + 0)
#define MAILBOX_STATUS (MAILBOX_BASE + 24)
#define MAILBOX_WRITE (MAILBOX_BASE + 32)
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000
#define REG(ADDR) (*(volatile uint32_t *)(ADDR))
#define MAILBOX_TIMEOUT 1000000 

int mailbox_call(mailbox_channels channel, volatile uint32_t *buffer){
    uint32_t addr = (uint32_t)(uintptr_t)buffer & 0x3FFFFFFF;
    if (addr & 0xF) {
        return -2;
    }
    uint32_t message = (addr & 0xFFFFFFF0) | (channel & 0xF);
    uint64_t start = timer_get_ticks();
    while(REG(MAILBOX_STATUS) & MAILBOX_FULL){
        if((timer_get_ticks() - start) > MAILBOX_TIMEOUT){
            return -1;
        }
    }
    REG(MAILBOX_WRITE) = message;
    start = timer_get_ticks();
    while(1){
        if((timer_get_ticks() - start) > MAILBOX_TIMEOUT){
            return -1;  
        }
        while(REG(MAILBOX_STATUS) & MAILBOX_EMPTY){
            if((timer_get_ticks() - start) > MAILBOX_TIMEOUT){
                return -1;
            }
        }
        uint32_t response = REG(MAILBOX_READ);
        if((response & 0xF) == channel){
            if((response & 0xFFFFFFF0) == (addr & 0xFFFFFFF0)){
                if(buffer[1] == 0x80000000){
                    return 0;
                } 
                else {
                    return -3;
                }
            }
        }

    }
}