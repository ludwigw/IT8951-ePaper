/**
 * @file GUI_BMPfile.h
 * @brief BMP image file loading and parsing for e-Paper displays.
 *
 * Provides functions and data structures for reading BMP files and rendering them
 * onto the e-Paper display buffer. Supports various bit depths and color palettes.
 *
 * @author Waveshare
 * @date 2018-11-12
 */
#ifndef __GUI_BMPFILE_H
#define __GUI_BMPFILE_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "DEV_Config.h"

extern UBYTE *bmp_dst_buf;
extern UBYTE *bmp_src_buf;

/**
 * @brief Bitmap file header (14 bytes).
 *
 * Used to describe the basic properties of a BMP file.
 */
typedef struct
{
    UWORD bType;              /**< File identifier, for BMP is 0x4D42. */
    UDOUBLE bSize;            /**< Size of the file in bytes. */
    UWORD brgbReversed1;      /**< Reserved, must be set to 0. */
    UWORD brgbReversed2;      /**< Reserved, must be set to 0. */
    UDOUBLE bOffset;          /**< Offset from file start to image data. */
} __attribute__((packed)) BMPFILEHEADER;

/**
 * @brief Bitmap information header (40 bytes).
 *
 * Contains detailed information about the BMP image.
 */
typedef struct
{
    UDOUBLE biInfoSize;       /**< Size of this header (40 bytes). */
    UDOUBLE biWidth;          /**< Image width in pixels. */
    UDOUBLE biHeight;         /**< Image height in pixels. */
    UWORD biPlanes;           /**< Number of color planes (must be 1). */
    UWORD biBitCount;         /**< Bits per pixel. */
    UDOUBLE biCompression;    /**< Compression type. */
    UDOUBLE bimpImageSize;    /**< Image size in bytes (multiple of 4). */
    UDOUBLE biXPelsPerMeter;  /**< Horizontal resolution (pixels/meter). */
    UDOUBLE biYPelsPerMeter;  /**< Vertical resolution (pixels/meter). */
    UDOUBLE biClrUsed;        /**< Number of colors in palette. */
    UDOUBLE biClrImportant;   /**< Number of important colors. */
} __attribute__((packed)) BMPINFOHEADER;

/**
 * @brief RGB color entry for BMP palette.
 */
typedef struct
{
    UBYTE rgbBlue;        /**< Blue intensity. */
    UBYTE rgbGreen;       /**< Green intensity. */
    UBYTE rgbRed;         /**< Red intensity. */
    UBYTE rgbReversed;    /**< Reserved. */
} __attribute__((packed)) BMPRGBQUAD;

/**
 * @brief Read and render a BMP file to the e-Paper display buffer.
 *
 * @param path Path to the BMP file.
 * @param x X coordinate on the display to start drawing.
 * @param y Y coordinate on the display to start drawing.
 * @return 0 on success, negative value on error.
 */
int GUI_ReadBmp(const char *path, UWORD x, UWORD y);

#endif
