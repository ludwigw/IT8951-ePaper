/**
 * @file DEV_Config_GPIOD.c
 * @brief Hardware abstraction implementation for platforms using GPIOD and custom SPI.
 *
 * Provides GPIO, SPI, and timing functions for e-Paper display control on platforms
 * using the GPIOD and custom SPI libraries.
 */
#include "../../include/DEV_Config.h"
#include "../../include/RPI_gpiod.h"
#include "../../include/dev_hardware_SPI.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Write a digital value to a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Value HIGH or LOW.
 */
void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    GPIOD_Write(Pin, Value);
}

/**
 * @brief Read a digital value from a GPIO pin.
 * @param Pin GPIO pin number.
 * @return HIGH or LOW.
 */
UBYTE DEV_Digital_Read(UWORD Pin) {
    return GPIOD_Read(Pin);
}

/**
 * @brief Write a byte over the SPI bus.
 * @param Value Byte to send.
 */
void DEV_SPI_WriteByte(UBYTE Value) {
    DEV_HARDWARE_SPI_TransferByte(Value);
}

/**
 * @brief Read a byte from the SPI bus.
 * @return Byte read from SPI.
 */
UBYTE DEV_SPI_ReadByte() {
    return DEV_HARDWARE_SPI_TransferByte(0x00);
}

/**
 * @brief Delay for a specified number of milliseconds.
 * @param xms Number of milliseconds to delay.
 */
void DEV_Delay_ms(UDOUBLE xms) {
    UDOUBLE i;
    for(i=0; i < xms; i++) {
        usleep(1000);
    }
}

/**
 * @brief Delay for a specified number of microseconds.
 * @param xus Number of microseconds to delay.
 */
void DEV_Delay_us(UDOUBLE xus) {
    usleep(xus);
}

/**
 * @brief Set the mode (input/output) for a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Mode 0 or GPIOD_IN for input, otherwise output.
 */
static void DEV_GPIO_Mode(UWORD Pin, UWORD Mode) {
    if(Mode == 0 || Mode == GPIOD_IN) {
        GPIOD_Direction(Pin, GPIOD_IN);
    } else {
        GPIOD_Direction(Pin, GPIOD_OUT);
    }
}

/**
 * @brief Initialize all required GPIO pins for the e-Paper display.
 */
static void DEV_GPIO_Init(void) {
    DEV_GPIO_Mode(EPD_BUSY_PIN, 0);
    DEV_GPIO_Mode(EPD_RST_PIN, 1);
    DEV_GPIO_Mode(EPD_CS_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void) {
    Debug("/***********************************/ \r\n");
    printf("Write and read /dev/spidev0.0 \r\n");
    GPIOD_Export();
    DEV_GPIO_Init();
    DEV_HARDWARE_SPI_begin("/dev/spidev0.0");
    DEV_HARDWARE_SPI_setSpeed(12500000);
    Debug("/***********************************/ \r\n");
    return 0;
}

/**
 * @brief Deinitialize the device and release resources.
 */
void DEV_Module_Exit(void) {
    DEV_HARDWARE_SPI_end();
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    GPIOD_Unexport(EPD_RST_PIN);
    GPIOD_Unexport(EPD_BUSY_PIN);
    GPIOD_Unexport_GPIO();
} 