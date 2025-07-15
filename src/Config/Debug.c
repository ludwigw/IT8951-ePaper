#include "../../include/Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Global log level
log_level_t current_log_level = LOG_LEVEL_INFO; // Default to INFO level

// Log level names for output
static const char* log_level_names[] = {
    "ERROR",
    "WARN", 
    "INFO",
    "DEBUG",
    "TRACE"
};

// Log level colors (for terminals that support it)
static const char* log_level_colors[] = {
    "\033[31m", // Red for ERROR
    "\033[33m", // Yellow for WARN
    "\033[32m", // Green for INFO
    "\033[36m", // Cyan for DEBUG
    "\033[35m"  // Magenta for TRACE
};

static const char* color_reset = "\033[0m";

void log_init(log_level_t level) {
    current_log_level = level;
    
    // Check if we should use colors (if output is to a terminal)
    const char* no_color = getenv("NO_COLOR");
    if (no_color != NULL) {
        // Disable colors by setting them to empty strings
        for (int i = 0; i < 5; i++) {
            log_level_colors[i] = "";
        }
        color_reset = "";
    }
}

static void log_message(log_level_t level, const char* format, va_list args) {
    if (level > current_log_level) {
        return; // Skip if level is higher than current setting
    }
    
    // Print timestamp and level
    printf("%s[%s]%s ", log_level_colors[level], log_level_names[level], color_reset);
    
    // Print the actual message
    vprintf(format, args);
    printf("\n");
    fflush(stdout);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_WARN, format, args);
    va_end(args);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

void log_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

void log_trace(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_TRACE, format, args);
    va_end(args);
} 