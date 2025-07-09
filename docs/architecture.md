# Architecture

This document describes the structure of the IT8951-ePaper codebase and how its main components interact to display images on a WaveShare e-Paper device.

---

## High-Level Flow

1. **Image Preparation**: Convert your image to a compatible BMP format (16-color grayscale, rotated as needed).
2. **Program Startup**: The example program (`main.c`) initializes the hardware and display.
3. **Image Loading**: The BMP file is loaded into memory and processed.
4. **Buffer Drawing**: The image is drawn into a framebuffer using the GUI utilities.
5. **Display Update**: The framebuffer is sent to the e-Paper display using the IT8951 protocol.

---

## Main Modules

### 1. `examples/`
- **main.c**: Entry point for running demos and displaying images. Handles argument parsing, device initialization, and demo selection.
- **example.c/h**: Implements demo routines, including displaying BMPs, drawing shapes, and more.

### 2. `lib/Config/`
- **DEV_Config.c/h**: Hardware abstraction for GPIO, SPI, delays, and device initialization. Supports multiple Pi models and libraries (BCM, GPIOD, LGPIO).
- **Debug.h**: Debugging macros and utilities.

### 3. `lib/e-Paper/`
- **EPD_IT8951.c/h**: Core driver for the IT8951 controller. Handles low-level communication, display refresh, and mode selection (GC16, A2, INIT).

### 4. `lib/GUI/`
- **GUI_Paint.c/h**: Drawing primitives (points, lines, rectangles, circles, text) and image buffer management.
- **GUI_BMPfile.c/h**: BMP file loading and conversion to framebuffer format.

### 5. `lib/Fonts/`
- Font data for rendering text in various sizes and languages.

---

## Data Flow Diagram

```mermaid
graph TD
    A[User Image (PNG/JPG)] --> B[convert_to_eink.sh]
    B --> C[BMP File (16-color, rotated)]
    C --> D[main.c]
    D --> E[GUI_BMPfile.c: Load BMP]
    E --> F[GUI_Paint.c: Draw to Framebuffer]
    F --> G[EPD_IT8951.c: Send to Display]
    G --> H[WaveShare e-Paper Display]
```

---

## Key Concepts
- **VCOM**: Voltage parameter required for display operation (printed on FPC cable)
- **Display Modes**: Different e-Paper models require different modes (rotation, mirroring, color)
- **Bit Depths**: 1bpp, 2bpp, 4bpp, 8bpp supported; 4bpp is recommended for most use cases
- **Alignment**: Some displays require 4-byte alignment for correct rendering

---

For more details, see [quickstart.md](./quickstart.md) and the code comments in each module. 

---

## Platform Selection: Which Backend Should I Use?

The library supports multiple hardware backends for GPIO and SPI access. Select the backend that matches your hardware and OS:

- **BCM (bcm2835 library):**
  - Use for most Raspberry Pi models (Zero, 2, 3, 4, etc.) running Raspberry Pi OS or similar.
  - This is the default and most widely tested backend.
- **LGPIO:**
  - Use for Raspberry Pi 5, or if you are running a newer OS where the bcm2835 library is not supported.
  - Also suitable for advanced users who want to use the lgpio library for GPIO/SPI access.
- **GPIOD:**
  - Use for Jetson Nano, some newer Linux SBCs, or if you want to use the Linux GPIO character device interface.
  - Advanced/experimental; not as widely tested as BCM.

**How to select the backend:**
- Use `make PLATFORM=LGPIO` or `make PLATFORM=GPIOD` when building, or just `make` for the default (BCM).
- See the Makefile for details.

---

## Porting to New Platforms (ESP32, STM32, etc.)

The IT8951-ePaper library is designed to be portable. All hardware-specific code (GPIO, SPI, delays) is isolated in the hardware abstraction layer (`DEV_Config.c/h`).

**To support a new platform:**

1. Create a new implementation of the abstraction layer (e.g., `DEV_Config_esp32.c/h`).
2. Implement the required functions using your platform’s SDK (see the function list in `DEV_Config.h`).
3. Update the build system to select the correct implementation (e.g., via a Makefile or CMake option).
4. Test with your hardware and contribute your changes!

**Best Practices:**
- Keep all hardware access in the abstraction layer; do not mix platform code into display logic or GUI modules.
- Use generic function names and signatures.
- Document any platform-specific requirements or limitations.

See [PLAN.md](PLAN.md) for more details and future-proofing tips. 