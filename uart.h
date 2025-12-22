#include <stdint.h>
#ifndef UART_H
#define UART_H

typedef enum {
    DECIMAL,
    BINARY,
    OCTAHEDRAL,
    HEXA
} num_format;

void uart_init(void);
void uart_print_char(char c);
void uart_print_string(const char *str);
void uart_print_num(uint32_t num , num_format fmt);

#endif 