/**
 * @file GUI_Paint.h
 * @brief Drawing and image buffer management for e-Paper displays.
 *
 * Provides functions for drawing primitives (points, lines, rectangles, circles),
 * rendering text (ASCII and Chinese), and managing image buffers for e-Paper displays.
 * Supports multiple bit depths, rotation, mirroring, and grayscale.
 *
 * @author Waveshare
 * @date 2019-04-18
 */
#ifndef __GUI_PAINT_H
#define __GUI_PAINT_H

#include "DEV_Config.h"
#include "fonts.h"

/**
 * @brief Image buffer attributes and drawing context.
 */
typedef struct {
    UBYTE *Image;           /**< Pointer to image buffer. */
    UWORD Width;            /**< Width of the drawing area. */
    UWORD Height;           /**< Height of the drawing area. */
    UWORD WidthMemory;      /**< Width of the memory buffer. */
    UWORD HeightMemory;     /**< Height of the memory buffer. */
    UWORD Color;            /**< Default color. */
    UWORD Rotate;           /**< Rotation setting. */
    UWORD Mirror;           /**< Mirroring setting. */
    UWORD WidthByte;        /**< Bytes per row. */
    UWORD HeightByte;       /**< Bytes per column. */
    UWORD BitsPerPixel;     /**< Bits per pixel. */
    UWORD GrayScale;        /**< Number of grayscale levels. */
} PAINT;
extern PAINT Paint;

/**
 * @brief Display rotation options.
 */
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

/**
 * @brief Display mirroring options.
 */
typedef enum {
    MIRROR_NONE  = 0x00,         /**< No mirroring. */
    MIRROR_HORIZONTAL = 0x01,    /**< Horizontal mirror. */
    MIRROR_VERTICAL = 0x02,      /**< Vertical mirror. */
    MIRROR_ORIGIN = 0x03,        /**< Mirror both axes. */
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * @brief Common color definitions.
 */
#define WHITE          0xFF
#define BLACK          0x00
#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

/**
 * @brief Dot (point) size options.
 */
typedef enum {
    DOT_PIXEL_1X1  = 1,      /**< 1x1 dot. */
    DOT_PIXEL_2X2,           /**< 2x2 dot. */
    DOT_PIXEL_3X3,           /**< 3x3 dot. */
    DOT_PIXEL_4X4,           /**< 4x4 dot. */
    DOT_PIXEL_5X5,           /**< 5x5 dot. */
    DOT_PIXEL_6X6,           /**< 6x6 dot. */
    DOT_PIXEL_7X7,           /**< 7x7 dot. */
    DOT_PIXEL_8X8,           /**< 8x8 dot. */
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1

/**
 * @brief Dot fill style options.
 */
typedef enum {
    DOT_FILL_AROUND  = 1,    /**< Fill around center. */
    DOT_FILL_RIGHTUP,        /**< Fill right/up. */
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND

/**
 * @brief Line style options.
 */
typedef enum {
    LINE_STYLE_SOLID = 0,    /**< Solid line. */
    LINE_STYLE_DOTTED,       /**< Dotted line. */
} LINE_STYLE;

/**
 * @brief Rectangle/circle fill options.
 */
typedef enum {
    DRAW_FILL_EMPTY = 0,     /**< No fill. */
    DRAW_FILL_FULL,          /**< Filled. */
} DRAW_FILL;

/**
 * @brief Structure for representing time (for time display functions).
 */
typedef struct {
    UWORD Year;      /**< Year (e.g., 2023). */
    UBYTE Month;     /**< Month (1-12). */
    UBYTE Day;       /**< Day (1-31). */
    UBYTE Hour;      /**< Hour (0-23). */
    UBYTE Min;       /**< Minute (0-59). */
    UBYTE Sec;       /**< Second (0-59). */
} PAINT_TIME;
extern PAINT_TIME sPaint_time;

/**
 * @brief Initialize a new image buffer for drawing.
 * @param image Pointer to the image buffer.
 * @param Width Width of the image.
 * @param Height Height of the image.
 * @param Rotate Rotation setting.
 * @param Color Default color (background).
 */
void Paint_NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color);

/**
 * @brief Select the image buffer to draw on.
 * @param image Pointer to the image buffer.
 */
void Paint_SelectImage(UBYTE *image);

/**
 * @brief Set the rotation for drawing operations.
 * @param Rotate Rotation angle (0, 90, 180, 270).
 */
void Paint_SetRotate(UWORD Rotate);

/**
 * @brief Set the mirroring mode for drawing operations.
 * @param mirror Mirroring mode.
 */
void Paint_SetMirroring(UBYTE mirror);

/**
 * @brief Set the bits per pixel for the image buffer.
 * @param bpp Bits per pixel (1, 2, 4, or 8).
 */
void Paint_SetBitsPerPixel(UBYTE bpp);

/**
 * @brief Set a pixel in the image buffer.
 * @param Xpoint X coordinate.
 * @param Ypoint Y coordinate.
 * @param Color Color value.
 */
void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color);

/**
 * @brief Clear the entire image buffer to a color.
 * @param Color Color value to fill.
 */
void Paint_Clear(UWORD Color);

/**
 * @brief Clear a rectangular window in the image buffer.
 * @param Xstart Starting X coordinate.
 * @param Ystart Starting Y coordinate.
 * @param Xend Ending X coordinate.
 * @param Yend Ending Y coordinate.
 * @param Color Color value to fill.
 */
void Paint_ClearWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color);

/**
 * @brief Draw a point (dot) at the specified location.
 * @param Xpoint X coordinate.
 * @param Ypoint Y coordinate.
 * @param Color Color value.
 * @param Dot_Pixel Dot size.
 * @param Dot_FillWay Dot fill style.
 */
void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);

/**
 * @brief Draw a line between two points.
 * @param Xstart Starting X coordinate.
 * @param Ystart Starting Y coordinate.
 * @param Xend Ending X coordinate.
 * @param Yend Ending Y coordinate.
 * @param Color Color value.
 * @param Line_width Line width (dot size).
 * @param Line_Style Line style (solid/dotted).
 */
void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style);

/**
 * @brief Draw a rectangle.
 * @param Xstart Starting X coordinate.
 * @param Ystart Starting Y coordinate.
 * @param Xend Ending X coordinate.
 * @param Yend Ending Y coordinate.
 * @param Color Color value.
 * @param Line_width Line width (dot size).
 * @param Draw_Fill Fill style (empty/full).
 */
void Paint_DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

/**
 * @brief Draw a circle.
 * @param X_Center Center X coordinate.
 * @param Y_Center Center Y coordinate.
 * @param Radius Circle radius.
 * @param Color Color value.
 * @param Line_width Line width (dot size).
 * @param Draw_Fill Fill style (empty/full).
 */
void Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

/**
 * @brief Draw a single ASCII character.
 * @param Xstart X coordinate.
 * @param Ystart Y coordinate.
 * @param Acsii_Char Character to draw.
 * @param Font Pointer to font structure.
 * @param Color_Foreground Foreground color.
 * @param Color_Background Background color.
 */
void Paint_DrawChar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

/**
 * @brief Draw an ASCII string.
 * @param Xstart X coordinate.
 * @param Ystart Y coordinate.
 * @param pString Pointer to string.
 * @param Font Pointer to font structure.
 * @param Color_Foreground Foreground color.
 * @param Color_Background Background color.
 */
void Paint_DrawString_EN(UWORD Xstart, UWORD Ystart, const char * pString, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

/**
 * @brief Draw a string with Chinese (GB2312) and ASCII characters.
 * @param Xstart X coordinate.
 * @param Ystart Y coordinate.
 * @param pString Pointer to string.
 * @param font Pointer to Chinese font structure.
 * @param Color_Foreground Foreground color.
 * @param Color_Background Background color.
 */
void Paint_DrawString_CN(UWORD Xstart, UWORD Ystart, const char * pString, cFONT* font, UWORD Color_Foreground, UWORD Color_Background);

/**
 * @brief Draw a number as a string.
 * @param Xpoint X coordinate.
 * @param Ypoint Y coordinate.
 * @param Nummber Number to draw.
 * @param Font Pointer to font structure.
 * @param Color_Foreground Foreground color.
 * @param Color_Background Background color.
 */
void Paint_DrawNum(UWORD Xpoint, UWORD Ypoint, int32_t Nummber, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

/**
 * @brief Draw a time value as a string.
 * @param Xstart X coordinate.
 * @param Ystart Y coordinate.
 * @param pTime Pointer to time structure.
 * @param Font Pointer to font structure.
 * @param Color_Foreground Foreground color.
 * @param Color_Background Background color.
 */
void Paint_DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME *pTime, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

/**
 * @brief Set a 3x3 color block at the specified location (for color e-Paper).
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param color Color value.
 */
void Paint_SetColor(UWORD x, UWORD y, UWORD color);

/**
 * @brief Get the 3x3 color block values for a given color (for color e-Paper).
 * @param color Color value.
 * @param arr_color Output array for color values.
 */
void Paint_GetColor(UWORD color, UBYTE* arr_color);

#endif





