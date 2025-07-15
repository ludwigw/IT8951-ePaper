/**
 * @file DEV_Config_BCM.c
 * @brief Hardware abstraction implementation for Raspberry Pi using BCM2835 library.
 *
 * Provides GPIO, SPI, and timing functions for e-Paper display control on Raspberry Pi platforms
 * using the bcm2835 library.
 */
#include "../../include/DEV_Config.h"
#include <bcm2835.h>
#include <fcntl.h>
#include <errno.h> // Added for errno
#include <stdlib.h> // Added for getenv

/**
 * @brief Write a digital value to a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Value HIGH or LOW.
 */
void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    DEV_LOG_TRACE("Setting pin %d to value %d", Pin, Value);
    bcm2835_gpio_write(Pin, Value);
    DEV_LOG_TRACE("Pin %d set to %d successfully", Pin, Value);
}

/**
 * @brief Read a digital value from a GPIO pin.
 * @param Pin GPIO pin number.
 * @return HIGH or LOW.
 */
UBYTE DEV_Digital_Read(UWORD Pin) {
    DEV_LOG_TRACE("Reading pin %d", Pin);
    UBYTE value = bcm2835_gpio_lev(Pin);
    DEV_LOG_TRACE("Pin %d read value: %d", Pin, value);
    return value;
}

/**
 * @brief Write a byte over the SPI bus.
 * @param Value Byte to send.
 */
void DEV_SPI_WriteByte(UBYTE Value) {
    bcm2835_spi_transfer(Value);
}

/**
 * @brief Read a byte from the SPI bus.
 * @return Byte read from SPI.
 */
UBYTE DEV_SPI_ReadByte() {
    return bcm2835_spi_transfer(0x00);
}

/**
 * @brief Delay for a specified number of milliseconds.
 * @param xms Number of milliseconds to delay.
 */
void DEV_Delay_ms(UDOUBLE xms) {
    bcm2835_delay(xms);
}

/**
 * @brief Delay for a specified number of microseconds.
 * @param xus Number of microseconds to delay.
 */
void DEV_Delay_us(UDOUBLE xus) {
    bcm2835_delayMicroseconds(xus);
}

/**
 * @brief Set the mode (input/output) for a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Mode 0 or BCM2835_GPIO_FSEL_INPT for input, otherwise output.
 */
static void DEV_GPIO_Mode(UWORD Pin, UWORD Mode) {
    DEV_LOG_DEBUG("Setting pin %d to mode %d", Pin, Mode);
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
        DEV_LOG_DEBUG("Configuring pin %d as input", Pin);
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
    } else {
        DEV_LOG_DEBUG("Configuring pin %d as output", Pin);
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
    }
    DEV_LOG_DEBUG("Pin %d configuration completed", Pin);
}

/**
 * @brief Initialize all required GPIO pins for the e-Paper display.
 */
static void DEV_GPIO_Init(void) {
    DEV_LOG_INFO("Starting GPIO pin configuration");
    DEV_LOG_DEBUG("Configuring RST_PIN (%d) as output", EPD_RST_PIN);
    DEV_GPIO_Mode(EPD_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    DEV_LOG_DEBUG("RST_PIN configured successfully");
    DEV_LOG_DEBUG("Configuring CS_PIN (%d) as output", EPD_CS_PIN);
    DEV_GPIO_Mode(EPD_CS_PIN, BCM2835_GPIO_FSEL_OUTP);
    DEV_LOG_DEBUG("CS_PIN configured successfully");
    DEV_LOG_DEBUG("Configuring BUSY_PIN (%d) as input", EPD_BUSY_PIN);
    DEV_GPIO_Mode(EPD_BUSY_PIN, BCM2835_GPIO_FSEL_INPT);
    DEV_LOG_DEBUG("BUSY_PIN configured successfully");
    // Enable pull-up on BUSY_PIN
    DEV_LOG_DEBUG("Enabling pull-up resistor on BUSY_PIN (%d)", EPD_BUSY_PIN);
    bcm2835_gpio_set_pud(EPD_BUSY_PIN, BCM2835_GPIO_PUD_UP);
    DEV_LOG_DEBUG("Pull-up resistor enabled on BUSY_PIN");
    DEV_LOG_DEBUG("Setting CS_PIN HIGH");
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
    DEV_LOG_DEBUG("CS_PIN set HIGH successfully");
    DEV_LOG_INFO("GPIO pin configuration completed");
}

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void) {
    DEV_LOG_INFO("Starting BCM2835 initialization");
    DEV_LOG_DEBUG("Checking GPIO memory access...");
    FILE *gpio_mem = fopen("/dev/gpiomem", "r");
    if (gpio_mem) {
        DEV_LOG_DEBUG("/dev/gpiomem is accessible");
        fclose(gpio_mem);
    } else {
        DEV_LOG_WARN("/dev/gpiomem not accessible (errno: %d)", errno);
    }
    DEV_LOG_DEBUG("Checking SPI device access...");
    FILE *spi_dev = fopen("/dev/spidev0.0", "r");
    if (spi_dev) {
        DEV_LOG_DEBUG("/dev/spidev0.0 is accessible");
        fclose(spi_dev);
    } else {
        DEV_LOG_WARN("/dev/spidev0.0 not accessible (errno: %d)", errno);
    }
    DEV_LOG_INFO("Starting BCM2835 initialization");
    DEV_LOG_DEBUG("Calling bcm2835_init()");
    if(!bcm2835_init()) {
        DEV_LOG_ERROR("bcm2835_init() failed!");
        return 1;
    } else {
        DEV_LOG_INFO("bcm2835_init() succeeded");
    }
    DEV_LOG_DEBUG("bcm2835 library version: %u", bcm2835_version());
    DEV_LOG_INFO("Starting SPI configuration");
    DEV_LOG_DEBUG("Calling bcm2835_spi_begin()");
    bcm2835_spi_begin();
    DEV_LOG_DEBUG("bcm2835_spi_begin() completed");
    DEV_LOG_DEBUG("Setting SPI bit order to LSBFIRST");
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);
    DEV_LOG_DEBUG("SPI bit order set successfully");
    DEV_LOG_DEBUG("Setting SPI data mode to MODE0");
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    DEV_LOG_DEBUG("SPI data mode set successfully");
    DEV_LOG_DEBUG("Setting SPI clock divider to 16");
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
    DEV_LOG_DEBUG("SPI clock divider set successfully");
    DEV_LOG_INFO("SPI configuration completed");
    DEV_LOG_DEBUG("Initializing GPIO pins");
    DEV_GPIO_Init();
    DEV_LOG_DEBUG("GPIO initialization completed");
    DEV_LOG_INFO("BCM2835 initialization completed successfully");
    return 0;
}

/**
 * @brief Deinitialize the device and release resources.
 */
void DEV_Module_Exit(void) {
    DEV_Digital_Write(EPD_CS_PIN, LOW);
    DEV_Digital_Write(EPD_RST_PIN, LOW);
    bcm2835_spi_end();
    bcm2835_close();
} 