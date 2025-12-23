#include "gpio.h"

#define GPIO_FSEL0 0x3F200000
#define GPIO_FSEL1 GPIO_FSEL0+4
#define GPIO_FSEL2 GPIO_FSEL0+(4*2)
#define GPIO_FSEL3 GPIO_FSEL0+(4*3)
#define GPIO_FSEL4 GPIO_FSEL0+(4*4)
#define GPIO_FSEL5 GPIO_FSEL0+(4*5)
#define GPIO_SET0 0x3F20001C
#define GPIO_SET1 0x3F200020
#define GPIO_CLR0 0x3F200028
#define GPIO_CLR1 0x3F20002C
#define GPIO_LEV0 0x3F200034
#define GPIO_LEV1 0x3F200038
#define GPPUD_CTRL 0x3F200094
#define GPPUD_CLK0 0x3F200098
#define GPPUD_CLK1 0x3F20009C
#define REG(addr) (*((volatile uint32_t *)(addr)))
static inline void delay(uint32_t count) {
    while (count > 0) {
        asm volatile("nop");
        count--;
    }
}
void gpio_set_function(uint32_t num , gpio_function af){
    if(num > 53){return;}
    uint32_t reg_add = GPIO_FSEL0 + (4*(num/10));
    uint32_t pin_pos = (num%10)*3;
    volatile uint32_t reg_val = REG(reg_add);
    reg_val = reg_val & (~(0b111 << pin_pos));
    reg_val = reg_val | (af << pin_pos);
    REG(reg_add)= reg_val;
}
void gpio_set_high(uint32_t num){
    if(num > 53){return;}
    if(num < 32){
        REG(GPIO_SET0) = 1u << num ;
    }
    else{
        REG(GPIO_SET1) = 1u << (num-32);
    }

}
void gpio_set_low(uint32_t num){
    if(num > 53){return;}
    if(num < 32){
        REG(GPIO_CLR0) = 1u << num;
    }
    else{
        REG(GPIO_CLR1) = 1u << (num-32);
    }
}
int gpio_get_level(uint32_t num){
    if(num > 53){return -1;}
    volatile uint32_t reg_val;
    if(num < 32){
        reg_val = REG(GPIO_LEV0);
        if((reg_val & (1u << num)) != 0){
            return 1;
        }else{
            return 0;
        }
    }
    else{
        reg_val = REG(GPIO_LEV1);
        if((reg_val & (1u << (num-32))) != 0){
            return 1;
        }else{
            return 0;
        }
    }
}
void gpio_set_pull(uint32_t num ,pull_type pt){
    REG(GPPUD_CTRL) = pt;
    delay(150);
    if(num < 32){
        REG(GPPUD_CLK0) = 1u << num;
    }else{
        REG(GPPUD_CLK1) =1u << (num-32);
    }
    delay(150);
    REG(GPPUD_CTRL) = 0;
    if(num<32){
        REG(GPPUD_CLK0) = 0;
    }else{
        REG(GPPUD_CLK1)=0;
    }
}
void gpio_toggle_pin(uint32_t num){
    if(num > 53){return;}
    int val = gpio_get_level(num);
    if(val == 1){
        gpio_set_low(num);
    }else{
        gpio_set_high(num);
    }
}