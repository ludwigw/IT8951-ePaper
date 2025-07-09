/**
 * @file DEV_Config_BCM.c
 * @brief Hardware abstraction implementation for Raspberry Pi using BCM2835 library.
 *
 * Provides GPIO, SPI, and timing functions for e-Paper display control on Raspberry Pi platforms
 * using the bcm2835 library.
 */
#include "../../Raspberry/lib/Config/DEV_Config.h"
#include <bcm2835.h>
#include <fcntl.h>

/**
 * @brief Write a digital value to a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Value HIGH or LOW.
 */
void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    bcm2835_gpio_write(Pin, Value);
}

/**
 * @brief Read a digital value from a GPIO pin.
 * @param Pin GPIO pin number.
 * @return HIGH or LOW.
 */
UBYTE DEV_Digital_Read(UWORD Pin) {
    return bcm2835_gpio_lev(Pin);
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
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
    } else {
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
    }
}

/**
 * @brief Initialize all required GPIO pins for the e-Paper display.
 */
static void DEV_GPIO_Init(void) {
    DEV_GPIO_Mode(EPD_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    DEV_GPIO_Mode(EPD_CS_PIN, BCM2835_GPIO_FSEL_OUTP);
    DEV_GPIO_Mode(EPD_BUSY_PIN, BCM2835_GPIO_FSEL_INPT);
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
}

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void) {
    Debug("/***********************************/ \r\n");
    if(!bcm2835_init()) {
        Debug("bcm2835 init failed  !!! \r\n");
        return 1;
    } else {
        Debug("bcm2835 init success !!! \r\n");
    }
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
    DEV_GPIO_Init();
    Debug("/***********************************/ \r\n");
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