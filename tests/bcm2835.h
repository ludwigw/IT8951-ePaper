#ifndef BCM2835_H_MOCK
#define BCM2835_H_MOCK

// SPI and GPIO function declarations
int bcm2835_init(void);
void bcm2835_spi_begin(void);
void bcm2835_spi_setBitOrder(int);
void bcm2835_spi_setDataMode(int);
void bcm2835_spi_setClockDivider(int);
void bcm2835_spi_end(void);
void bcm2835_close(void);

void bcm2835_gpio_write(int pin, int value);
int bcm2835_gpio_lev(int pin);
unsigned char bcm2835_spi_transfer(unsigned char value);
void bcm2835_delay(unsigned int ms);
void bcm2835_delayMicroseconds(unsigned int us);
void bcm2835_gpio_fsel(int pin, int mode);

// Macros/constants used in DEV_Config.c
#define BCM2835_GPIO_FSEL_INPT 0
#define BCM2835_GPIO_FSEL_OUTP 1
#define BCM2835_SPI_BIT_ORDER_MSBFIRST 0
#define BCM2835_SPI_MODE0 0
#define BCM2835_SPI_CLOCK_DIVIDER_32 32

#endif // BCM2835_H_MOCK 