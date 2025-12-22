#include "uart.h"

#define UART_BASE       0x3F215000
#define AUX_ENABLES     0x3F215004
#define AUX_MU_IO       (UART_BASE + 0x40)
#define AUX_MU_IER      (UART_BASE + 0x44)
#define AUX_MU_IIR      (UART_BASE + 0x48)
#define AUX_MU_LCR      (UART_BASE + 0x4C)
#define AUX_MU_MCR      (UART_BASE + 0x50)
#define AUX_MU_LSR      (UART_BASE + 0x54)
#define AUX_MU_CNTL     (UART_BASE + 0x60)
#define AUX_MU_BAUD     (UART_BASE + 0x68)
#define REG(addr) (*(volatile uint32_t*)(addr))

void uart_init(void) {
    REG(AUX_ENABLES) = 1;
    REG(AUX_MU_IER) = 0;
    REG(AUX_MU_CNTL) = 0;
    REG(AUX_MU_LCR) = 3;
    REG(AUX_MU_MCR) = 0;
    REG(AUX_MU_IIR) = 0xC6;
    REG(AUX_MU_BAUD) = 270;
    REG(AUX_MU_CNTL) = 3;
}

void uart_print_char(char c) {
    while (!(REG(AUX_MU_LSR) & 0x20)) {
        }
    REG(AUX_MU_IO) = (uint32_t)c;
}

void uart_print_string(const char* str) {
    while (*str) {
        if (*str == '\n') {
            uart_print_char('\r');
        }
        uart_print_char(*str);
        str++;
    }
}

void uart_print_num(uint32_t num , num_format fmt){
    switch (fmt)
    {
    case DECIMAL:
        if(num == 0){
            uart_print_char('0');
            return;
        }
        char buffer[10];
        int i = 0;
        while(num > 0){
            buffer[i++] = '0'+(num%10);
            num = num/10;
        }
        while(i>0){
            uart_print_char(buffer[--i]);
        }
        break;
    case BINARY:
        uart_print_string("0b");
        for(int i = 31; i >=0 ; i--){
            if(num & (1U <<i)){
                uart_print_char('1');
            }
            else{
                uart_print_char('0');
            }
        }
        break;
    case OCTAHEDRAL:
        if(num == 0){
            uart_print_char('0');
            return;
        }
        char buffer2[11];
        int i2 = 0;
        while(num > 0){
            buffer2[i2++] = '0'+(num%8);
            num = num/8;
        }
        while(i2 > 0){
            uart_print_char(buffer2[--i2]);
        }
        break;
    case HEXA:
        const char hexa[] = "0123456789ABCDEF";
        uart_print_string("0x");
        for(int i = 7 ; i >= 0 ; i--){
            uint32_t n = (num >> (i*4)) & 0xF;
            uart_print_char(hexa[n]);
        }
        break;
    
    default:
        break;
    }
}


