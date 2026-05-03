#include "spi.h"
#include "gpio.h"
#include "timer.h"

#define SPI_BASE 0x3F204000
#define SPI_CS (SPI_BASE + 0)
#define SPI_FIFO (SPI_BASE + 4)
#define SPI_CLK (SPI_BASE + 8)
#define REG(ADDR) (*(volatile uint32_t *)(ADDR))
#define SPI_CS_TXD (1 << 18)
#define SPI_CS_RXD (1 << 17)
#define SPI_CS_DONE (1 << 16)
#define SPI_CS_TA (1 << 7)
#define SPI_CS_CLEAR_RX (2 << 4)
#define SPI_CS_CLEAR_TX (1 << 4)
#define SPI_CS_CPOL (1 << 3)
#define SPI_CS_CPHA (1 << 2)
#define SPI_CS_CS (3 << 0)

void spi_init(spi_mode mode , uint32_t divider){
    gpio_set_function(7,AF0);
    gpio_set_function(8,AF0);
    gpio_set_function(9,AF0);
    gpio_set_function(10,AF0);
    gpio_set_function(11,AF0);
    REG(SPI_CLK) = divider;
    uint32_t cs = 0;
    if(mode == MODE1 || mode == MODE3){
        cs = cs | SPI_CS_CPHA;
    }
    else if(mode == MODE2 || mode == MODE3){
        cs = cs | SPI_CS_CPOL;
    }
    REG(SPI_CS) = cs | SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
}
int spi_transfer(const uint8_t *datat, uint8_t *datar, uint32_t size){
    REG(SPI_CS) |= SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;
    REG(SPI_CS) |= SPI_CS_TA;
    uint32_t tx_count = 0, rx_count = 0;
    uint64_t start = timer_get_ticks();
    while (tx_count < size || rx_count < size) {
        while (tx_count < size && (REG(SPI_CS) & SPI_CS_TXD)) {
            REG(SPI_FIFO) = datat ? datat[tx_count]:0x00;
            tx_count++;
            start = timer_get_ticks();
        }
        while (rx_count < size && (REG(SPI_CS) & SPI_CS_RXD)) {
            uint8_t data = REG(SPI_FIFO) & 0xFF;
            if (datar) {
                datar[rx_count] = data;
            }
            rx_count++;
            start = timer_get_ticks();
        }
        if (timer_get_ticks()-start > 100000) {
            REG(SPI_CS) &= ~SPI_CS_TA;
            return -1;
        }
    start = timer_get_ticks();
    while (!(REG(SPI_CS) & SPI_CS_DONE)) {
        if (timer_get_ticks()-start > 100000) {
            REG(SPI_CS) &= ~SPI_CS_TA;
            return -3;
        }
    }
    REG(SPI_CS) &= ~SPI_CS_TA;
    return 0;
    }
}
void spi_chip_select(uint8_t cs, bool val){
    uint32_t value = REG(SPI_CS);
    value &= ~(SPI_CS_CS | SPI_CS_TA);
    value |= (cs & 0x3);
    if (val) value |= SPI_CS_TA;
    REG(SPI_CS) = value;
}