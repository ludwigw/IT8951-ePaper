/**
 * @file DEV_Config.h
 * @brief Device (hardware) abstraction layer for e-Paper display drivers.
 *
 * This module provides platform-agnostic functions for GPIO, SPI, and timing operations,
 * enabling portability across different hardware platforms (e.g., Raspberry Pi, Jetson, etc.).
 *
 * All hardware-specific implementations are provided in platform-specific source files.
 *
 * @author Waveshare
 * @date 2018-10-30
 */
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "Debug.h"

#ifdef BCM
    #include <bcm2835.h>
#elif LGPIO
    #include <lgpio.h>
    #define LFLAGS 0
    #define NUM_MAXBUF  4
#elif GPIOD
    #include "RPI_gpiod.h"
    #include "dev_hardware_SPI.h"
#endif

#define HIGH   0x1
#define LOW    0x0  

/**
 * @name GPIO Pin Definitions
 * GPIO pin numbers for e-Paper control signals.
 */
#define EPD_RST_PIN  17
#define EPD_CS_PIN   8
#define EPD_BUSY_PIN 24

/**
 * @name Data Type Aliases
 * Shorter aliases for unsigned integer types.
 */
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * @brief Write a digital value to a GPIO pin.
 * @param Pin GPIO pin number.
 * @param Value HIGH or LOW.
 */
void DEV_Digital_Write(UWORD Pin, UBYTE Value);

/**
 * @brief Read a digital value from a GPIO pin.
 * @param Pin GPIO pin number.
 * @return HIGH or LOW.
 */
UBYTE DEV_Digital_Read(UWORD Pin);

/**
 * @brief Write a byte over the SPI bus.
 * @param Value Byte to send.
 */
void DEV_SPI_WriteByte(UBYTE Value);

/**
 * @brief Read a byte from the SPI bus.
 * @return Byte read from SPI.
 */
UBYTE DEV_SPI_ReadByte();

/**
 * @brief Delay for a specified number of milliseconds.
 * @param xms Number of milliseconds to delay.
 */
void DEV_Delay_ms(UDOUBLE xms);

/**
 * @brief Delay for a specified number of microseconds.
 * @param xus Number of microseconds to delay.
 */
void DEV_Delay_us(UDOUBLE xus);

/**
 * @brief Initialize the device (GPIO, SPI, etc.).
 * @return 0 on success, nonzero on failure.
 */
UBYTE DEV_Module_Init(void);

/**
 * @brief Deinitialize the device and release resources.
 */
void DEV_Module_Exit(void);

#endif
