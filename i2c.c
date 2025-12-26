#include "i2c.h"
#include "gpio.h"
#include "timer.h"

#define I2C1_BASE 0x3F804000
#define I2C_CTRL (I2C1_BASE + 0)
#define I2C_STAT (I2C1_BASE + 4)
#define I2C_DL (I2C1_BASE + 8)
#define I2C_ADDR (I2C1_BASE + 12)
#define I2C_FIFO (I2C1_BASE + 16)
#define I2C_DIV (I2C1_BASE + 20)
#define REG(ADDR) (*(volatile uint32_t *)(ADDR))
#define BSC_C_I2CEN (1 << 15)
#define BSC_C_ST (1 << 7)
#define BSC_C_CLEAR (1 << 4)  
#define BSC_C_READ (1 << 0)
#define BSC_S_CLKT (1 << 9)
#define BSC_S_ERR (1 << 8)
#define BSC_S_RXD (1 << 5)
#define BSC_S_TXD (1 << 4)
#define BSC_S_DONE (1 << 1)

static inline void bsc_clear_status(void){
    REG(I2C_STAT) = BSC_S_CLKT | BSC_S_ERR | BSC_S_DONE;
}
static inline void bsc_clear_fifo(void){
    REG(I2C_CTRL) = BSC_C_CLEAR;
}
void i2c_init(void){
    gpio_set_function(2, AF0);
    gpio_set_function(3, AF0);
    REG(I2C_CTRL) = 0;
    REG(I2C_DIV) = 1500;
    REG(I2C_CTRL) = BSC_C_I2CEN;
}
int i2c_write(uint8_t addr, uint8_t *data, uint32_t size){
    if (!data || size == 0){
        return -1;
    }
    REG(I2C_CTRL) = 0;
    bsc_clear_fifo();
    bsc_clear_status();
    REG(I2C_ADDR) = addr &(0x7F);
    REG(I2C_DL)= size;
    REG(I2C_CTRL) = BSC_C_I2CEN;
    for (uint32_t i = 0; i < size; i++) {
        uint64_t t0 = timer_get_ticks();
        while (!(REG(I2C_STAT) & BSC_S_TXD)) {
            if (timer_get_ticks() - t0 > 100000)
                return -5;
        }
        REG(I2C_FIFO) = data[i];
    }
    REG(I2C_CTRL) = BSC_C_I2CEN | BSC_C_ST;
    uint64_t time = timer_get_ticks();
    while (1) {
        uint32_t stat = REG(I2C_STAT);
        if (stat& BSC_S_ERR){
            return -2;
        }
        if (stat& BSC_S_CLKT){
            return -3;
        }
        if (stat& BSC_S_DONE){
            break;
        }
        if (timer_get_ticks() - time > 100000){
            return -4;
        }
    }
    return 0;
}
int i2c_read(uint8_t addr, uint8_t *data, uint32_t size){
    if (!data || size == 0){
        return -1;
    }
    bsc_clear_fifo();
    bsc_clear_status();
    REG(I2C_ADDR) = addr & 0x7F;
    REG(I2C_DL) = size;
    REG(I2C_CTRL) = (BSC_C_I2CEN )|(BSC_C_ST)|(BSC_C_READ);
    uint32_t i = 0;
    uint64_t time = timer_get_ticks();
    while (i < size) {
        if (REG(I2C_STAT) & BSC_S_RXD){
            data[i] = REG(I2C_FIFO);
            i++;
        }
        if (REG(I2C_STAT) & BSC_S_ERR){
            return -2;}

        if (REG(I2C_STAT) & BSC_S_CLKT){
            return -3;
        }
        if(timer_get_ticks() -time  > 100000){
            return -4;
        }
    }
    time = timer_get_ticks();
    while (!(REG(I2C_STAT) & BSC_S_DONE)){
        if(timer_get_ticks()-time  >100000){
            return -4;
        }
    };
    return 0;
}