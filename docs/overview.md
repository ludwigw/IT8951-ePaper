# IT8951-ePaper Library Overview

## Purpose

This library provides a universal driver and example code for controlling WaveShare e-Paper displays (IT8951 controller) from a Raspberry Pi (including Pi Zero W2). Its main use case is to display images and graphics on a variety of supported e-ink panels, with support for different bit depths, refresh modes, and display sizes.

## 🚀 Simple Usage

The easiest way to display an image is using the built-in CLI tool:

```sh
make bin/epdraw           # Build the CLI tool
./bin/epdraw image.bmp    # Display your image
```

The 'epdraw' CLI tool is the recommended way to use this library for image display.

## Key Features
- **Simple Usage**: Display images with just two commands
- Supports multiple WaveShare e-Paper displays (various sizes, grayscale, and color)
- Works with Raspberry Pi (including Pi Zero W2) and STM32
- Multiple refresh modes (GC16, A2, INIT)
- Supports 1bpp, 2bpp, 4bpp, and 8bpp image depths
- Example code for displaying BMP images, drawing graphics, and more

## Main Components
- **examples/**: Example programs, including `main.c` for displaying BMP images
- **src/Config/**: Hardware abstraction and configuration (GPIO, SPI, delays)
- **src/e-Paper/**: Core e-Paper driver logic (IT8951 protocol) with high-level API
- **src/GUI/**: Image and drawing utilities (drawing shapes, text, handling BMP files)
- **src/Fonts/**: Font data for text rendering

## Typical Use Case
1. Prepare your Raspberry Pi and connect the e-Paper display
2. Build the library: `make`
3. Convert your image to a compatible BMP format
4. Display the image: `./main image.bmp`

See the other documentation files in this directory for detailed setup, usage, and troubleshooting instructions. 