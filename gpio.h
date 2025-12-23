#include <stdint.h>
#ifndef GPIO_H
#define GPIO_H

typedef enum {
    INPUT = 0b000,
    OUTPUT = 0b001,
    AF0 = 0b100,
    AF1 = 0b101,
    AF2 = 0b110,
    AF3 = 0b111,
    AF4 = 0b011,
    AF5 = 0b010
} gpio_function;

typedef enum{
    PULL_UP = 0b10,
    PULL_DOWN = 0b01,
    NONE = 0b00
} pull_type;

void gpio_set_function(uint32_t num , gpio_function af);
void gpio_set_high(uint32_t num);
void gpio_set_low(uint32_t num);
int gpio_get_level(uint32_t num);
void gpio_set_pull(uint32_t num , pull_type pt);
void gpio_toggle_pin(uint32_t num);

#endif