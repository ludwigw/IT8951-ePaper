# API Reference

This document provides a quick reference for the most commonly used functions in the IT8951-ePaper library.

## 🚀 Quick Usage (Recommended)

The easiest way to display an image is using the built-in CLI tool:

```sh
make bin/epdraw           # Build the CLI tool
./bin/epdraw image.bmp    # Display your image
```

The 'epdraw' CLI tool is the recommended way to use this library for image display.

## High-Level API (For Custom Programs)

If you want to integrate e-Paper display functionality into your own C programs, use the high-level API below.

### `EPD_IT8951_DisplayBMP`

Display a BMP image on the e-Paper display with a single function call.

```c
int EPD_IT8951_DisplayBMP(const char* filename, int vcom, int mode);
```

**Parameters:**
- `filename`: Path to the BMP file to display
- `vcom`: VCOM voltage (0 = use panel default, or specify value like 2510 for -2.51V)
- `mode`: Display mode (2 = GC16 mode, recommended for most images)

**Returns:**
- `0`: Success
- `-1`: File not found
- `-2`: Invalid BMP format
- `-3`: Display initialization failed
- `-4`: Display update failed

**Example:**
```c
#include "EPD_IT8951.h"

int main() {
    int result = EPD_IT8951_DisplayBMP("image.bmp", 0, 2);
    if (result == 0) {
        printf("Image displayed successfully!\n");
    } else {
        printf("Failed to display image (error %d)\n", result);
    }
    return 0;
}
```

---

## Low-Level API (For Advanced Users)

### Display Initialization

```c
int EPD_IT8951_Init(int vcom);
```
Initialize the e-Paper display with the specified VCOM voltage.

### Display Update

```c
int EPD_IT8951_Display(const UBYTE* image, int x, int y, int w, int h, int mode);
```
Display image data at the specified position with the given mode.

### Display Modes

- `0`: No rotate, no mirroring (default)
- `1`: No rotate, X mirroring (10.3" e-Paper HAT)
- `2`: No rotate, X mirroring (5.2" e-Paper) - **Recommended**
- `3`: No rotate, no mirroring, color (6" color)

### GUI Functions

```c
void Paint_NewImage(UBYTE* image, int width, int height, int rotate, int color);
void Paint_SelectImage(UBYTE* image);
void Paint_DrawPoint(int x, int y, int color);
void Paint_DrawLine(int x0, int y0, int x1, int y1, int color);
void Paint_DrawRectangle(int x0, int y0, int x1, int y1, int color);
void Paint_DrawCircle(int x0, int y0, int radius, int color);
void Paint_DrawString(int x, int y, const char* text, int color);
```

### BMP Loading

```c
int GUI_ReadBmp(const char* path, int x, int y);
```
Load and display a BMP file at the specified position.

---

## Platform Selection

The library supports multiple hardware backends. Select the appropriate platform when building:

```sh
make PLATFORM=BCM    # Default, most Raspberry Pi models
make PLATFORM=GPIOD  # Raspberry Pi 5, newer Linux SBCs
make PLATFORM=LGPIO  # Raspberry Pi 5, newer OS versions
```

---

## Error Handling

Always check return values from API functions. Common error codes:
- `0`: Success
- Negative values: Error codes (see function documentation)
- Check `errno` for system-level errors

---

For complete API documentation with all functions and detailed parameters, see the [Doxygen-generated HTML docs](html/index.html). 