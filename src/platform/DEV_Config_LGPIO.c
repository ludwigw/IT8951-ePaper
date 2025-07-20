/**
 * @file DEV_Config_LGPIO.c
 * @brief Hardware abstraction implementation for Raspberry Pi 5 using LGPIO library.
 *
 * Provides GPIO, SPI, and timing functions for e-Paper display control on Raspberry Pi 5 platforms
 * using the LGPIO library.
 */
#include "../../include/DEV_Config.h"
#include <lgpio.h>
#include <fcntl.h>
#include <stdio.h>

int GPIO_Handle;
int SPI_Handle;

/**
 * @brief Write a digital value to a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Value HIGH or LOW.
 */
void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    lgGpioWrite(GPIO_Handle, Pin, Value);
}

/**
 * @brief Read a digital value from a GPIO pin.
 * @param Pin GPIO pin number.
 * @return HIGH or LOW.
 */
UBYTE DEV_Digital_Read(UWORD Pin) {
    return lgGpioRead(GPIO_Handle, Pin);
}

/**
 * @brief Write a byte over the SPI bus.
 * @param Value Byte to send.
 */
void DEV_SPI_WriteByte(UBYTE Value) {
    lgSpiWrite(SPI_Handle, (char*)&Value, 1);
}

/**
 * @brief Read a byte from the SPI bus.
 * @return Byte read from SPI.
 */
UBYTE DEV_SPI_ReadByte() {
    UBYTE Read_Value = 0x00;
    lgSpiRead(SPI_Handle, (char*)&Read_Value, 1);
    return Read_Value;
}

/**
 * @brief Delay for a specified number of milliseconds.
 * @param xms Number of milliseconds to delay.
 */
void DEV_Delay_ms(UDOUBLE xms) {
    lguSleep(xms/1000.0);
}

/**
 * @brief Delay for a specified number of microseconds.
 * @param xus Number of microseconds to delay.
 */
void DEV_Delay_us(UDOUBLE xus) {
    lguSleep(xus/1000000.0);
}

/**
 * @brief Set the mode (input/output) for a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Mode 0 or LG_SET_INPUT for input, otherwise output.
 */
static void DEV_GPIO_Mode(UWORD Pin, UWORD Mode) {
    if(Mode == 0 || Mode == LG_SET_INPUT){
        lgGpioClaimInput(GPIO_Handle, LFLAGS, Pin);
    } else {
        lgGpioClaimOutput(GPIO_Handle, LFLAGS, Pin, LG_LOW);
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
    Debug("[PLATFORM] Using platform: lgpio\n");
    Debug("/***********************************/ \r\n");
    char buffer[NUM_MAXBUF];
    FILE *fp;
    fp = popen("cat /proc/cpuinfo | grep 'Raspberry Pi 5'", "r");
    if (fp == NULL) {
        Debug("It is not possible to determine the model of the Raspberry PI\n");
        return -1;
    }
    if(fgets(buffer, sizeof(buffer), fp) != NULL) {
        GPIO_Handle = lgGpiochipOpen(4);
        if (GPIO_Handle < 0) {
            Debug("gpiochip4 Export Failed\n");
            return -1;
        }
    } else {
        GPIO_Handle = lgGpiochipOpen(0);
        if (GPIO_Handle < 0) {
            Debug("gpiochip0 Export Failed\n");
            return -1;
        }
    }
    SPI_Handle = lgSpiOpen(0, 0, 12500000, 0);
    DEV_GPIO_Init();
    Debug("/***********************************/ \r\n");
    return 0;
}

/**
 * @brief Deinitialize the device and release resources.
 */
void DEV_Module_Exit(void) {
    // DEV_Digital_Write(EPD_CS_PIN, 0);
    // DEV_Digital_Write(EPD_RST_PIN, 0);
    // lgSpiClose(SPI_Handle);
    // lgGpiochipClose(GPIO_Handle);
} 