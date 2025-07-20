/*****************************************************************************
* | File      	:	Debug.h
* | Author      :   Waveshare team
* | Function    :	debug with printf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V2.0
* | Date        :   2018-10-30
* | Info        :   
*   1.USE_DEBUG -> DEBUG, If you need to see the debug information, 
*    clear the execution: make DEBUG=-DDEBUG
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include <stdarg.h>

// Log levels
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} log_level_t;

// Global log level (can be set at runtime)
extern log_level_t current_log_level;

// Initialize logging system
void log_init(log_level_t level);

// Logging functions
void log_error(const char *format, ...);
void log_warn(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);
void log_trace(const char *format, ...);

// Convenience macros for different components
#define LOG_ERROR(fmt, ...) log_error("[%s] " fmt, __func__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  log_warn("[%s] " fmt, __func__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  log_info("[%s] " fmt, __func__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_debug("[%s] " fmt, __func__, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) log_trace("[%s] " fmt, __func__, ##__VA_ARGS__)

// Component-specific logging macros
#define DEV_LOG_ERROR(fmt, ...) log_error("[DEV] " fmt, ##__VA_ARGS__)
#define DEV_LOG_WARN(fmt, ...)  log_warn("[DEV] " fmt, ##__VA_ARGS__)
#define DEV_LOG_INFO(fmt, ...)  log_info("[DEV] " fmt, ##__VA_ARGS__)
#define DEV_LOG_DEBUG(fmt, ...) log_debug("[DEV] " fmt, ##__VA_ARGS__)
#define DEV_LOG_TRACE(fmt, ...) log_trace("[DEV] " fmt, ##__VA_ARGS__)

#define EPD_LOG_ERROR(fmt, ...) log_error("[EPD] " fmt, ##__VA_ARGS__)
#define EPD_LOG_WARN(fmt, ...)  log_warn("[EPD] " fmt, ##__VA_ARGS__)
#define EPD_LOG_INFO(fmt, ...)  log_info("[EPD] " fmt, ##__VA_ARGS__)
#define EPD_LOG_DEBUG(fmt, ...) log_debug("[EPD] " fmt, ##__VA_ARGS__)
#define EPD_LOG_TRACE(fmt, ...) log_trace("[EPD] " fmt, ##__VA_ARGS__)

// Legacy Debug macro for backward compatibility
#ifdef USE_DEBUG
#define Debug(fmt, ...) log_debug(fmt, ##__VA_ARGS__)
#else
#define Debug(fmt, ...) do {} while(0)
#endif

#endif // __DEBUG_H

