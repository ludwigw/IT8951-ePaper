#include <assert.h>
#include <stdio.h>
#define BCM 0
#define LGPIO 1
#define GPIOD 0

// --- BCM2835 mocks (needed for linking) ---
int bcm2835_init() { return 1; }
void bcm2835_spi_begin() {}
void bcm2835_spi_setBitOrder(int x) { (void)x; }
void bcm2835_spi_setDataMode(int x) { (void)x; }
void bcm2835_spi_setClockDivider(int x) { (void)x; }
void bcm2835_spi_end() {}
void bcm2835_close() {}
unsigned char bcm2835_spi_transfer(unsigned char value) { (void)value; return 0; }
void bcm2835_gpio_write(int pin, int value) { (void)pin; (void)value; }
int bcm2835_gpio_lev(int pin) { (void)pin; return 0; }
void bcm2835_delay(unsigned int ms) { (void)ms; }
void bcm2835_delayMicroseconds(unsigned int us) { (void)us; }
void bcm2835_gpio_fsel(int pin, int mode) { (void)pin; (void)mode; }

// --- LGPIO mocks ---
int lgpio_init_called = 0;
int lgpio_init_should_fail = 0;
int lgpio_init() { lgpio_init_called = 1; return lgpio_init_should_fail ? -1 : 0; }
void lgpio_exit() { /* nothing */ }

#include "../src/platform/DEV_Config_LGPIO.c"

void reset_all() {
    lgpio_init_called = 0;
    lgpio_init_should_fail = 0;
}

void test_lgpio_platform_success() {
    reset_all();
    int result = DEV_Module_Init();
    assert(result == 0);
    assert(lgpio_init_called);
    DEV_Module_Exit();
}
void test_lgpio_platform_error() {
    reset_all();
    lgpio_init_should_fail = 1;
    int result = DEV_Module_Init();
    assert(result != 0);
    assert(lgpio_init_called);
    lgpio_init_should_fail = 0;
}

int main() {
    test_lgpio_platform_success();
    test_lgpio_platform_error();
    printf("DEV_Config LGPIO platform selection and error handling tests passed!\n");
    return 0;
} 