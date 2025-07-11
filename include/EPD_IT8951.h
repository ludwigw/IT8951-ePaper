/**
 * @file EPD_IT8951.h
 * @brief Driver interface for the IT8951 e-Paper controller.
 *
 * Provides functions and data structures for initializing, refreshing, and controlling
 * e-Paper displays using the IT8951 controller. Supports multiple bit depths, partial updates,
 * and platform-agnostic hardware abstraction.
 *
 * @author Waveshare
 * @date 2019-09-17
 */
#ifndef __EPD_IT8951_H_
#define __EPD_IT8951_H_

#include <stdbool.h>
#include "DEV_Config.h"

/**
 * @brief Image load information for IT8951 controller.
 */
typedef struct IT8951_Load_Img_Info {
    UWORD    Endian_Type;         /**< Endian type: little or big endian. */
    UWORD    Pixel_Format;        /**< Pixel format (bit depth). */
    UWORD    Rotate;              /**< Rotation mode. */
    UBYTE*   Source_Buffer_Addr;  /**< Start address of source frame buffer. */
    UDOUBLE  Target_Memory_Addr;  /**< Base address of target image buffer. */
} IT8951_Load_Img_Info;

/**
 * @brief Area information for partial image updates.
 */
typedef struct IT8951_Area_Img_Info {
    UWORD Area_X; /**< X coordinate of area. */
    UWORD Area_Y; /**< Y coordinate of area. */
    UWORD Area_W; /**< Width of area. */
    UWORD Area_H; /**< Height of area. */
} IT8951_Area_Img_Info;

/**
 * @brief Device information structure for IT8951.
 */
typedef struct IT8951_Dev_Info {
    UWORD Panel_W; /**< Panel width. */
    UWORD Panel_H; /**< Panel height. */
    UWORD Memory_Addr_L; /**< Low part of memory address. */
    UWORD Memory_Addr_H; /**< High part of memory address. */
    UWORD FW_Version[8]; /**< Firmware version. */
    UWORD LUT_Version[8]; /**< LUT version. */
} IT8951_Dev_Info;

typedef struct {
    UBYTE bits_per_pixel;
    UWORD rotate;
    UBYTE mirror;
    UBYTE is_color;
} EPD_Config;

EPD_Config EPD_IT8951_ComputeConfig(UWORD mode);

/*-----------------------------------------------------------------------
IT8951 Command defines
------------------------------------------------------------------------*/

//Built in I80 Command Code
#define IT8951_TCON_SYS_RUN      0x0001
#define IT8951_TCON_STANDBY      0x0002
#define IT8951_TCON_SLEEP        0x0003
#define IT8951_TCON_REG_RD       0x0010
#define IT8951_TCON_REG_WR       0x0011

#define IT8951_TCON_MEM_BST_RD_T 0x0012
#define IT8951_TCON_MEM_BST_RD_S 0x0013
#define IT8951_TCON_MEM_BST_WR   0x0014
#define IT8951_TCON_MEM_BST_END  0x0015

#define IT8951_TCON_LD_IMG       0x0020
#define IT8951_TCON_LD_IMG_AREA  0x0021
#define IT8951_TCON_LD_IMG_END   0x0022

//I80 User defined command code
#define USDEF_I80_CMD_DPY_AREA     0x0034
#define USDEF_I80_CMD_GET_DEV_INFO 0x0302
#define USDEF_I80_CMD_DPY_BUF_AREA 0x0037
#define USDEF_I80_CMD_VCOM		   0x0039

/*-----------------------------------------------------------------------
 IT8951 Mode defines
------------------------------------------------------------------------*/

//Rotate mode
#define IT8951_ROTATE_0     0
#define IT8951_ROTATE_90    1
#define IT8951_ROTATE_180   2
#define IT8951_ROTATE_270   3

//Pixel mode (Bit per Pixel)
#define IT8951_2BPP   0
#define IT8951_3BPP   1
#define IT8951_4BPP   2
#define IT8951_8BPP   3

//Endian Type
#define IT8951_LDIMG_L_ENDIAN   0
#define IT8951_LDIMG_B_ENDIAN   1

/*-----------------------------------------------------------------------
IT8951 Registers defines
------------------------------------------------------------------------*/
//Register Base Address
#define DISPLAY_REG_BASE 0x1000               //Register RW access

//Base Address of Basic LUT Registers
#define LUT0EWHR  (DISPLAY_REG_BASE + 0x00)   //LUT0 Engine Width Height Reg
#define LUT0XYR   (DISPLAY_REG_BASE + 0x40)   //LUT0 XY Reg
#define LUT0BADDR (DISPLAY_REG_BASE + 0x80)   //LUT0 Base Address Reg
#define LUT0MFN   (DISPLAY_REG_BASE + 0xC0)   //LUT0 Mode and Frame number Reg
#define LUT01AF   (DISPLAY_REG_BASE + 0x114)  //LUT0 and LUT1 Active Flag Reg

//Update Parameter Setting Register
#define UP0SR     (DISPLAY_REG_BASE + 0x134)  //Update Parameter0 Setting Reg
#define UP1SR     (DISPLAY_REG_BASE + 0x138)  //Update Parameter1 Setting Reg
#define LUT0ABFRV (DISPLAY_REG_BASE + 0x13C)  //LUT0 Alpha blend and Fill rectangle Value
#define UPBBADDR  (DISPLAY_REG_BASE + 0x17C)  //Update Buffer Base Address
#define LUT0IMXY  (DISPLAY_REG_BASE + 0x180)  //LUT0 Image buffer X/Y offset Reg
#define LUTAFSR   (DISPLAY_REG_BASE + 0x224)  //LUT Status Reg (status of All LUT Engines)
#define BGVR      (DISPLAY_REG_BASE + 0x250)  //Bitmap (1bpp) image color table

//System Registers
#define SYS_REG_BASE 0x0000

//Address of System Registers
#define I80CPCR (SYS_REG_BASE + 0x04)

//Memory Converter Registers
#define MCSR_BASE_ADDR 0x0200
#define MCSR  (MCSR_BASE_ADDR + 0x0000)
#define LISAR (MCSR_BASE_ADDR + 0x0008)

/**
 * @brief Enhance the driving capability of the e-Paper display (platform-specific tuning).
 */
void Enhance_Driving_Capability(void);

/**
 * @brief Bring the IT8951 controller out of standby and into system run mode.
 */
void EPD_IT8951_SystemRun(void);

/**
 * @brief Put the IT8951 controller into standby mode.
 */
void EPD_IT8951_Standby(void);

/**
 * @brief Put the IT8951 controller into sleep mode.
 */
void EPD_IT8951_Sleep(void);

/**
 * @brief Initialize the IT8951 controller and return device info.
 * @param VCOM VCOM voltage setting.
 * @return Device information structure.
 */
IT8951_Dev_Info EPD_IT8951_Init(UWORD VCOM);

/**
 * @brief Clear the display and refresh with the given mode.
 * @param Dev_Info Device information.
 * @param Target_Memory_Addr Target memory address for the image buffer.
 * @param Mode Display mode (e.g., INIT, GC16, A2).
 */
void EPD_IT8951_Clear_Refresh(IT8951_Dev_Info Dev_Info, UDOUBLE Target_Memory_Addr, UWORD Mode);

/**
 * @brief Refresh a region of the display with a 1bpp (monochrome) image buffer.
 * @param Frame_Buf Pointer to the image buffer.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Mode Display mode.
 * @param Target_Memory_Addr Target memory address.
 * @param Packed_Write Whether to use packed write mode.
 */
void EPD_IT8951_1bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, UBYTE Mode, UDOUBLE Target_Memory_Addr, bool Packed_Write);

/**
 * @brief Write multiple 1bpp frames to the display memory.
 * @param Frame_Buf Pointer to the image buffer.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Target_Memory_Addr Target memory address.
 * @param Packed_Write Whether to use packed write mode.
 */
void EPD_IT8951_1bp_Multi_Frame_Write(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, UDOUBLE Target_Memory_Addr, bool Packed_Write);

/**
 * @brief Refresh the display from multiple 1bpp frames in memory.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Target_Memory_Addr Target memory address.
 */
void EPD_IT8951_1bp_Multi_Frame_Refresh(UWORD X, UWORD Y, UWORD W, UWORD H, UDOUBLE Target_Memory_Addr);

/**
 * @brief Refresh a region of the display with a 2bpp (4-level grayscale) image buffer.
 * @param Frame_Buf Pointer to the image buffer.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Hold Whether to hold the display after refresh.
 * @param Target_Memory_Addr Target memory address.
 * @param Packed_Write Whether to use packed write mode.
 */
void EPD_IT8951_2bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr, bool Packed_Write);

/**
 * @brief Refresh a region of the display with a 4bpp (16-level grayscale) image buffer.
 * @param Frame_Buf Pointer to the image buffer.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Hold Whether to hold the display after refresh.
 * @param Target_Memory_Addr Target memory address.
 * @param Packed_Write Whether to use packed write mode.
 */
void EPD_IT8951_4bp_Refresh(UBYTE* Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr, bool Packed_Write);

/**
 * @brief Refresh a region of the display with an 8bpp (256-level grayscale) image buffer.
 * @param Frame_Buf Pointer to the image buffer.
 * @param X X coordinate.
 * @param Y Y coordinate.
 * @param W Width.
 * @param H Height.
 * @param Hold Whether to hold the display after refresh.
 * @param Target_Memory_Addr Target memory address.
 */
void EPD_IT8951_8bp_Refresh(UBYTE *Frame_Buf, UWORD X, UWORD Y, UWORD W, UWORD H, bool Hold, UDOUBLE Target_Memory_Addr);

/**
 * @brief High-level API: Display a BMP image file on the e-Paper display.
 *
 * Loads a BMP file, draws it to the display buffer, and refreshes the e-Paper display.
 * Handles all buffer management and display logic internally.
 *
 * @param path Path to the BMP file.
 * @param VCOM VCOM voltage setting (pass 0 to use default).
 * @param Mode Display mode (e.g., INIT, GC16, A2).
 * @return 0 on success, negative value on error.
 */
int EPD_IT8951_DisplayBMP(const char *path, UWORD VCOM, UWORD Mode);

#endif
