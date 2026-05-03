#include "stdint.h"
#include "stdbool.h"
#ifndef SPI_H
#define SPI_H
typedef enum {
    MODE0 = 0 ,
    MODE1 = 1,
    MODE2 = 2,
    MODE3 = 3
}spi_mode;

void spi_init(spi_mode mode , uint32_t divider);
int spi_transfer(const uint8_t *data_t , uint8_t *data_r , uint32_t size);
void spi_cs(uint8_t cs , bool val);

#endif