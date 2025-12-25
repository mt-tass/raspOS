#include "stdint.h"
#ifndef I2C_H
#define I2C_H

void i2c_init(void);
int i2c_write(uint8_t addr , uint8_t *data , uint32_t size);
int i2c_read(uint8_t addr , uint8_t *data , uint32_t size);

#endif