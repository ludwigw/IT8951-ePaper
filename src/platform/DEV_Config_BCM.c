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
    printf("DEV_GPIO_Init: Configuring RST_PIN (%d) as output\n", EPD_RST_PIN);
    DEV_GPIO_Mode(EPD_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("DEV_GPIO_Init: Configuring CS_PIN (%d) as output\n", EPD_CS_PIN);
    DEV_GPIO_Mode(EPD_CS_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("DEV_GPIO_Init: Configuring BUSY_PIN (%d) as input\n", EPD_BUSY_PIN);
    DEV_GPIO_Mode(EPD_BUSY_PIN, BCM2835_GPIO_FSEL_INPT);
    printf("DEV_GPIO_Init: Setting CS_PIN HIGH\n");
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
    printf("DEV_GPIO_Init: GPIO pin configuration completed\n");
}

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void) {
    printf("DEV_Module_Init: Starting BCM2835 initialization\n");
    Debug("/***********************************/ \r\n");
    printf("DEV_Module_Init: Calling bcm2835_init()\n");
    if(!bcm2835_init()) {
        printf("DEV_Module_Init: ERROR - bcm2835_init() failed!\n");
        Debug("bcm2835 init failed  !!! \r\n");
        return 1;
    } else {
        printf("DEV_Module_Init: bcm2835_init() succeeded\n");
        Debug("bcm2835 init success !!! \r\n");
    }
    printf("DEV_Module_Init: Starting SPI configuration\n");
    printf("DEV_Module_Init: Calling bcm2835_spi_begin()\n");
    bcm2835_spi_begin();
    printf("DEV_Module_Init: Setting SPI bit order to MSBFIRST\n");
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    printf("DEV_Module_Init: Setting SPI data mode to MODE0\n");
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    printf("DEV_Module_Init: Setting SPI clock divider to 32\n");
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
    printf("DEV_Module_Init: SPI configuration completed\n");
    printf("DEV_Module_Init: Initializing GPIO pins\n");
    DEV_GPIO_Init();
    printf("DEV_Module_Init: GPIO initialization completed\n");
    printf("DEV_Module_Init: BCM2835 initialization completed successfully\n");
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