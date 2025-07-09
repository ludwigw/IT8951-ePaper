#include <assert.h>
#include <stdio.h>
#define BCM 1
#define LGPIO 0
#define GPIOD 0

int bcm2835_init_called = 0;
int bcm2835_spi_begin_called = 0;
int bcm2835_spi_setBitOrder_called = 0;
int bcm2835_spi_setDataMode_called = 0;
int bcm2835_spi_setClockDivider_called = 0;
int bcm2835_spi_end_called = 0;
int bcm2835_close_called = 0;
int bcm2835_init_should_fail = 0;

int bcm2835_init() { bcm2835_init_called = 1; return bcm2835_init_should_fail ? 0 : 1; }
void bcm2835_spi_begin() { bcm2835_spi_begin_called = 1; }
void bcm2835_spi_setBitOrder(int x) { bcm2835_spi_setBitOrder_called = 1; }
void bcm2835_spi_setDataMode(int x) { bcm2835_spi_setDataMode_called = 1; }
void bcm2835_spi_setClockDivider(int x) { bcm2835_spi_setClockDivider_called = 1; }
void bcm2835_spi_end() { bcm2835_spi_end_called = 1; }
void bcm2835_close() { bcm2835_close_called = 1; }
unsigned char bcm2835_spi_transfer(unsigned char value) { return 0; }
void bcm2835_gpio_write(int pin, int value) { (void)pin; (void)value; }
int bcm2835_gpio_lev(int pin) { (void)pin; return 0; }
void bcm2835_delay(unsigned int ms) { (void)ms; }
void bcm2835_delayMicroseconds(unsigned int us) { (void)us; }
void bcm2835_gpio_fsel(int pin, int mode) { (void)pin; (void)mode; }

#include "../src/platform/DEV_Config_BCM.c"

void reset_all() {
    bcm2835_init_called = 0;
    bcm2835_spi_begin_called = 0;
    bcm2835_spi_setBitOrder_called = 0;
    bcm2835_spi_setDataMode_called = 0;
    bcm2835_spi_setClockDivider_called = 0;
    bcm2835_spi_end_called = 0;
    bcm2835_close_called = 0;
    bcm2835_init_should_fail = 0;
}

void test_bcm_platform_success() {
    reset_all();
    int result = DEV_Module_Init();
    assert(result == 0);
    assert(bcm2835_init_called);
    assert(bcm2835_spi_begin_called);
    assert(bcm2835_spi_setBitOrder_called);
    assert(bcm2835_spi_setDataMode_called);
    assert(bcm2835_spi_setClockDivider_called);
    DEV_Module_Exit();
    assert(bcm2835_spi_end_called);
    assert(bcm2835_close_called);
}
void test_bcm_platform_error() {
    reset_all();
    bcm2835_init_should_fail = 1;
    int result = DEV_Module_Init();
    assert(result != 0);
    assert(bcm2835_init_called);
    bcm2835_init_should_fail = 0;
}

int main() {
    test_bcm_platform_success();
    test_bcm_platform_error();
    printf("DEV_Config BCM platform selection and error handling tests passed!\n");
    return 0;
} 