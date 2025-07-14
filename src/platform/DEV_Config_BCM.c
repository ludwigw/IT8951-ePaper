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
    printf("DEV_Digital_Write: Setting pin %d to value %d\n", Pin, Value);
    bcm2835_gpio_write(Pin, Value);
    printf("DEV_Digital_Write: Pin %d set to %d successfully\n", Pin, Value);
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
    printf("DEV_GPIO_Mode: Setting pin %d to mode %d\n", Pin, Mode);
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
        printf("DEV_GPIO_Mode: Configuring pin %d as input\n", Pin);
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
    } else {
        printf("DEV_GPIO_Mode: Configuring pin %d as output\n", Pin);
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
    }
    printf("DEV_GPIO_Mode: Pin %d configuration completed\n", Pin);
}

/**
 * @brief Initialize all required GPIO pins for the e-Paper display.
 */
static void DEV_GPIO_Init(void) {
    printf("DEV_GPIO_Init: Starting GPIO pin configuration\n");
    printf("DEV_GPIO_Init: Configuring RST_PIN (%d) as output\n", EPD_RST_PIN);
    DEV_GPIO_Mode(EPD_RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("DEV_GPIO_Init: RST_PIN configured successfully\n");
    printf("DEV_GPIO_Init: Configuring CS_PIN (%d) as output\n", EPD_CS_PIN);
    DEV_GPIO_Mode(EPD_CS_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("DEV_GPIO_Init: CS_PIN configured successfully\n");
    printf("DEV_GPIO_Init: Configuring BUSY_PIN (%d) as input\n", EPD_BUSY_PIN);
    DEV_GPIO_Mode(EPD_BUSY_PIN, BCM2835_GPIO_FSEL_INPT);
    printf("DEV_GPIO_Init: BUSY_PIN configured successfully\n");
    printf("DEV_GPIO_Init: Setting CS_PIN HIGH\n");
    DEV_Digital_Write(EPD_CS_PIN, HIGH);
    printf("DEV_GPIO_Init: CS_PIN set HIGH successfully\n");
    printf("DEV_GPIO_Init: GPIO pin configuration completed\n");
}

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void) {
    printf("DEV_Module_Init: Starting BCM2835 initialization\n");
    
    // Check if we can access GPIO memory
    printf("DEV_Module_Init: Checking GPIO memory access...\n");
    FILE *gpio_mem = fopen("/dev/gpiomem", "r");
    if (gpio_mem) {
        printf("DEV_Module_Init: /dev/gpiomem is accessible\n");
        fclose(gpio_mem);
    } else {
        printf("DEV_Module_Init: WARNING - /dev/gpiomem not accessible (errno: %d)\n", errno);
    }
    
    // Check if we can access SPI device
    printf("DEV_Module_Init: Checking SPI device access...\n");
    FILE *spi_dev = fopen("/dev/spidev0.0", "r");
    if (spi_dev) {
        printf("DEV_Module_Init: /dev/spidev0.0 is accessible\n");
        fclose(spi_dev);
    } else {
        printf("DEV_Module_Init: WARNING - /dev/spidev0.0 not accessible (errno: %d)\n", errno);
    }
    
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
    
    // Check bcm2835 library version
    printf("DEV_Module_Init: bcm2835 library version: %u\n", bcm2835_version());
    
    printf("DEV_Module_Init: Starting SPI configuration\n");
    printf("DEV_Module_Init: Calling bcm2835_spi_begin()\n");
    bcm2835_spi_begin();
    printf("DEV_Module_Init: bcm2835_spi_begin() completed\n");
    printf("DEV_Module_Init: Setting SPI bit order to MSBFIRST\n");
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    printf("DEV_Module_Init: SPI bit order set successfully\n");
    printf("DEV_Module_Init: Setting SPI data mode to MODE0\n");
    printf("DEV_Module_Init: About to call bcm2835_spi_setDataMode(BCM2835_SPI_MODE0)\n");
    printf("DEV_Module_Init: BCM2835_SPI_MODE0 value = %d\n", BCM2835_SPI_MODE0);
    
    // Try to catch any potential issues with the SPI mode setting
    printf("DEV_Module_Init: Attempting SPI data mode configuration...\n");
    
    // Check if we should skip SPI mode setting (workaround for some Pi configurations)
    printf("DEV_Module_Init: About to check SKIP_SPI_MODE environment variable...\n");
    const char* skip_spi_mode = getenv("SKIP_SPI_MODE");
    printf("DEV_Module_Init: getenv(\"SKIP_SPI_MODE\") returned: %s\n", skip_spi_mode ? skip_spi_mode : "NULL");
    
    if (skip_spi_mode && strcmp(skip_spi_mode, "1") == 0) {
        printf("DEV_Module_Init: SKIP_SPI_MODE=1, skipping SPI data mode setting\n");
    } else {
        printf("DEV_Module_Init: About to call bcm2835_spi_setDataMode(BCM2835_SPI_MODE0)...\n");
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
        printf("DEV_Module_Init: SPI data mode set successfully\n");
    }
    
    printf("DEV_Module_Init: Setting SPI clock divider to 32\n");
    printf("DEV_Module_Init: About to call bcm2835_spi_setClockDivider...\n");
    
    // Check if we should skip SPI clock divider setting (workaround for some Pi configurations)
    const char* skip_spi_clock = getenv("SKIP_SPI_CLOCK");
    if (skip_spi_clock && strcmp(skip_spi_clock, "1") == 0) {
        printf("DEV_Module_Init: SKIP_SPI_CLOCK=1, skipping SPI clock divider setting\n");
    } else {
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
        printf("DEV_Module_Init: SPI clock divider set successfully\n");
    }
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