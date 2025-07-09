/**
 * @file fonts.h
 * @brief Bitmap font data structures and font tables for e-Paper display text rendering.
 *
 * Provides font structures and font tables for rendering ASCII and Chinese (GB2312) text
 * on e-Paper displays. Supports multiple font sizes and both English and Chinese characters.
 *
 * @author STMicroelectronics, Waveshare
 * @date 2014-02-18, 2018-11-12
 */
/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   Header for fonts.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

/*΢ź24 (32x41) */
#define MAX_HEIGHT_FONT         41
#define MAX_WIDTH_FONT          32
#define OFFSET_BITMAP           

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * @brief Structure describing an ASCII (English) bitmap font.
 */
typedef struct _tFont {
    const uint8_t *table;   /**< Pointer to font bitmap data. */
    uint16_t Width;         /**< Character width in pixels. */
    uint16_t Height;        /**< Character height in pixels. */
} sFONT;

/**
 * @brief Structure describing a single Chinese (GB2312) character bitmap.
 */
typedef struct {
    const char index[2];    /**< GB2312 character index (2 bytes). */
    const char matrix[MAX_HEIGHT_FONT*MAX_WIDTH_FONT/8+2]; /**< Bitmap data. */
} CH_CN;

/**
 * @brief Structure describing a Chinese (GB2312) font table.
 */
typedef struct {
    const CH_CN *table;     /**< Pointer to array of Chinese character bitmaps. */
    uint16_t size;          /**< Number of characters in the table. */
    uint16_t ASCII_Width;   /**< Width of ASCII characters in this font. */
    uint16_t Width;         /**< Character width in pixels. */
    uint16_t Height;        /**< Character height in pixels. */
} cFONT;

/**
 * @brief 24-pixel-high ASCII font.
 */
extern sFONT Font24;
/**
 * @brief 20-pixel-high ASCII font.
 */
extern sFONT Font20;
/**
 * @brief 16-pixel-high ASCII font.
 */
extern sFONT Font16;
/**
 * @brief 12-pixel-high ASCII font.
 */
extern sFONT Font12;
/**
 * @brief 8-pixel-high ASCII font.
 */
extern sFONT Font8;

/**
 * @brief 12-pixel-high Chinese (GB2312) font.
 */
extern cFONT Font12CN;
/**
 * @brief 24-pixel-high Chinese (GB2312) font.
 */
extern cFONT Font24CN;
#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
