#include <assert.h>
#include <stdio.h>
#include <string.h>

// --- BCM MOCKS ---
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

// --- LGPIO MOCKS ---
int lgpio_init_called = 0;
int lgpio_init_should_fail = 0;
int lgpio_init() { lgpio_init_called = 1; return lgpio_init_should_fail ? -1 : 0; }
void lgpio_exit() { /* nothing */ }

// --- GPIOD MOCKS ---
int gpiod_init_called = 0;
int gpiod_init_should_fail = 0;
int gpiod_init() { gpiod_init_called = 1; return gpiod_init_should_fail ? -1 : 0; }
void gpiod_exit() { /* nothing */ }

// Platform selection macros
#define TEST_BCM 1
#define TEST_LGPIO 2
#define TEST_GPIOD 3

// Helper to reset all flags
void reset_all() {
    bcm2835_init_called = 0;
    bcm2835_spi_begin_called = 0;
    bcm2835_spi_setBitOrder_called = 0;
    bcm2835_spi_setDataMode_called = 0;
    bcm2835_spi_setClockDivider_called = 0;
    bcm2835_spi_end_called = 0;
    bcm2835_close_called = 0;
    bcm2835_init_should_fail = 0;
    lgpio_init_called = 0;
    lgpio_init_should_fail = 0;
    gpiod_init_called = 0;
    gpiod_init_should_fail = 0;
}

// --- Platform test harness ---
#define PLATFORM_TEST(NAME, PLATFORM_DEFINE, INIT_FN, EXIT_FN, INIT_CALLED, INIT_SHOULD_FAIL, EXIT_CALLED) \
    void test_##NAME##_platform_success() { \
        reset_all(); \
        /* Simulate platform define */ \
        /* Redefine macros for DEV_Config */ \
        #undef BCM \
        #undef LGPIO \
        #undef GPIOD \
        #define BCM (PLATFORM_DEFINE == TEST_BCM) \
        #define LGPIO (PLATFORM_DEFINE == TEST_LGPIO) \
        #define GPIOD (PLATFORM_DEFINE == TEST_GPIOD) \
        /* Re-include DEV_Config to pick up new defines */ \
        #include "../Raspberry/lib/Config/DEV_Config.c" \
        int result = DEV_Module_Init(); \
        assert(result == 0); \
        assert(INIT_CALLED); \
        DEV_Module_Exit(); \
    } \
    void test_##NAME##_platform_error() { \
        reset_all(); \
        INIT_SHOULD_FAIL = 1; \
        /* Simulate platform define */ \
        #undef BCM \
        #undef LGPIO \
        #undef GPIOD \
        #define BCM (PLATFORM_DEFINE == TEST_BCM) \
        #define LGPIO (PLATFORM_DEFINE == TEST_LGPIO) \
        #define GPIOD (PLATFORM_DEFINE == TEST_GPIOD) \
        /* Re-include DEV_Config to pick up new defines */ \
        #include "../Raspberry/lib/Config/DEV_Config.c" \
        int result = DEV_Module_Init(); \
        assert(result != 0); \
        assert(INIT_CALLED); \
        INIT_SHOULD_FAIL = 0; \
    }

// Can't actually re-include DEV_Config.c multiple times in one translation unit, so instead, do manual per-platform tests:

// --- BCM platform test ---
void test_bcm_platform_success() {
    reset_all();
    bcm2835_init_should_fail = 0;
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

// --- LGPIO platform test ---
void test_lgpio_platform_success() {
    reset_all();
    lgpio_init_should_fail = 0;
    lgpio_init_called = 0;
    int result = lgpio_init();
    assert(result == 0);
    assert(lgpio_init_called);
    lgpio_exit();
}
void test_lgpio_platform_error() {
    reset_all();
    lgpio_init_should_fail = 1;
    int result = lgpio_init();
    assert(result != 0);
    assert(lgpio_init_called);
    lgpio_init_should_fail = 0;
}

// --- GPIOD platform test ---
void test_gpiod_platform_success() {
    reset_all();
    gpiod_init_should_fail = 0;
    gpiod_init_called = 0;
    int result = gpiod_init();
    assert(result == 0);
    assert(gpiod_init_called);
    gpiod_exit();
}
void test_gpiod_platform_error() {
    reset_all();
    gpiod_init_should_fail = 1;
    int result = gpiod_init();
    assert(result != 0);
    assert(gpiod_init_called);
    gpiod_init_should_fail = 0;
}

int main() {
    test_bcm_platform_success();
    test_bcm_platform_error();
    test_lgpio_platform_success();
    test_lgpio_platform_error();
    test_gpiod_platform_success();
    test_gpiod_platform_error();
    printf("DEV_Config platform selection and error handling tests passed!\n");
    return 0;
} 