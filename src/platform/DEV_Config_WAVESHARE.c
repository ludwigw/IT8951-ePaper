// DEV_Config_WAVESHARE.c
// Platform backend for Raspberry Pi using Waveshare demo's SPI (ioctl) and GPIO (gpiod) logic
#include "../../include/DEV_Config.h"
#include "../../include/dev_hardware_SPI.h"
#include "../../include/RPI_gpiod.h"
#include "../../include/Debug.h"
#include <unistd.h>
#include <stdint.h>

// --- GPIO Abstraction ---
void DEV_Digital_Write(UWORD Pin, UBYTE Value) {
    if (Pin == EPD_CS_PIN) {
        EPD_LOG_TRACE("[PLATFORM] CS pin write: pin=%d, value=%d (%s)", Pin, Value, Value ? "HIGH" : "LOW");
    }
    GPIOD_Write(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin) {
    UBYTE value = GPIOD_Read(Pin);
    EPD_LOG_TRACE("[PLATFORM] Digital read: pin=%d, value=%d", Pin, value);
    return value;
}

// --- SPI Abstraction ---
void DEV_SPI_WriteByte(UBYTE Value) {
    EPD_LOG_TRACE("[PLATFORM] SPI write byte: 0x%02X", Value);
    DEV_HARDWARE_SPI_TransferByte(Value);
}

UBYTE DEV_SPI_ReadByte() {
    UBYTE value = DEV_HARDWARE_SPI_TransferByte(0x00);
    EPD_LOG_TRACE("[PLATFORM] SPI read byte: 0x%02X", value);
    return value;
}

void DEV_Delay_ms(UDOUBLE xms) {
    EPD_LOG_DEBUG("[PLATFORM] Delay: %llu ms", (unsigned long long)xms);
    usleep(xms * 1000);
}

void DEV_Delay_us(UDOUBLE xus) {
    EPD_LOG_DEBUG("[PLATFORM] Delay: %llu us", (unsigned long long)xus);
    usleep(xus);
}

static void DEV_GPIO_Init(void) {
    GPIOD_Export();
    GPIOD_Direction(EPD_RST_PIN, GPIOD_OUT);
    GPIOD_Direction(EPD_CS_PIN, GPIOD_OUT);
    GPIOD_Direction(EPD_BUSY_PIN, GPIOD_IN);
    GPIOD_Write(EPD_CS_PIN, 1); // Set CS high
}

UBYTE DEV_Module_Init(void) {
    EPD_LOG_INFO("[PLATFORM] Using platform: waveshare");
    DEV_GPIO_Init();
    DEV_HARDWARE_SPI_begin("/dev/spidev0.0");
    DEV_HARDWARE_SPI_setSpeed(20000000); // 20 MHz as in demo
    return 0;
}

void DEV_Module_Exit(void) {
    DEV_HARDWARE_SPI_end();
    GPIOD_Unexport_GPIO();
} 