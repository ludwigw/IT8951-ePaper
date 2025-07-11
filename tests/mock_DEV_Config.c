#include <stdint.h>
#include "../include/DEV_Config.h"

void DEV_Delay_us(uint32_t xus) { (void)xus; }
void DEV_Delay_ms(uint32_t xms) { (void)xms; }

// Stubs for other functions if needed by tests
void DEV_Digital_Write(uint16_t Pin, uint8_t Value) { (void)Pin; (void)Value; }
uint8_t DEV_Digital_Read(uint16_t Pin) { (void)Pin; return 0; }
void DEV_SPI_WriteByte(uint8_t Value) { (void)Value; }
uint8_t DEV_SPI_ReadByte(void) { return 0; }
unsigned char DEV_Module_Init(void) { return 0; }
void DEV_Module_Exit(void) {} 