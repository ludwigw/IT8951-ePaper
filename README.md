# IT8951-ePaper

> **Purpose:**
> 
> This library provides a universal driver and example code for controlling WaveShare e-Paper displays (IT8951 controller) from a Raspberry Pi (including Pi Zero W2). Its main use case is to display images and graphics on a variety of supported e-ink panels, with support for different bit depths, refresh modes, and display sizes.
> 
> **Simple API:** Display images with just one function call: `EPD_IT8951_DisplayBMP("image.bmp", 0, 2)`

## 🚀 Quick Start

Display an image on your e-Paper display in 3 steps:

```sh
make bin/epdraw           # Build the CLI tool
./bin/epdraw myphoto.jpg # Display your image (any format: PNG, JPG, BMP, etc.)
```

The 'epdraw' CLI tool is the recommended way to use this library for image display. You can pass any image file (PNG, JPG, BMP, etc.) and it will be automatically converted to the correct format for your e-Paper display using ImageMagick.

See the [`/docs`](./docs) directory for detailed guides and troubleshooting.

> **Documentation Update:**
> 
> For a detailed guide on understanding, building, and using this library to display images on WaveShare e-ink devices from a Raspberry Pi, see the new documentation in the [`/docs`](./docs) directory.
> 
> This includes step-by-step instructions, architecture overview, and troubleshooting tips for common hacks and issues.

## ⚠️ Platform Backends: BCM Library, LGPIO, GPIOD — Why This Matters

This library supports multiple hardware backends for controlling the e-Paper display, and **choosing the correct one is critical for your hardware to work reliably.**

### Background: The Confusion
- **BCM Backend:** Uses the `bcm2835` library, which is a popular C library for Raspberry Pi GPIO and SPI. It relies on the Pi's hardware SPI and GPIO subsystems, and is easy to use for most Pi projects.
- **LGPIO Backend:** For Raspberry Pi 5 or newer OSes, uses the `lgpio` library for GPIO/SPI access.
- **GPIOD Backend:** For Jetson Nano, some newer Linux SBCs, or if you want to use the Linux GPIO character device interface. Advanced/experimental.

### Why Does This Matter?
- Some e-Paper panels (especially the 10.3" D variant) are **very sensitive to SPI timing and CS line handling**.
- The BCM backend works for most Pi models and Waveshare panels.
- The LGPIO and GPIOD backends are for advanced users or special hardware.

### How to Choose
- **Default:** The Makefile uses `PLATFORM=BCM` by default for compatibility.
- **If you have display update or BUSY pin issues:** Try building with `PLATFORM=LGPIO` or `PLATFORM=GPIOD`:
  ```sh
  make bin/epdraw PLATFORM=LGPIO
  make bin/epdraw PLATFORM=GPIOD
  ```
- This will use the appropriate backend for your hardware.

### What Actually Happens
- The Makefile and codebase will only include one backend at a time (BCM, LGPIO, or GPIOD).
- The CLI and library will link against the correct platform-specific code for SPI and GPIO.

### Why Multiple Backends?
- The BCM backend is simpler and works for most Pi models and Waveshare panels.
- The LGPIO and GPIOD backends are for advanced users, hardware debugging, or when the BCM backend fails due to timing/CS issues.
- This multi-backend approach is necessary because the IT8951 controller is very sensitive to low-level hardware details, and the official demo code is the only known-good reference for some panels.

---

## waveshare electronics
![waveshare_logo.png](waveshare_logo.png)
http://www.waveshare.net  
https://www.waveshare.com  

## 中文 ## 
e-Paper IT8951 Driver HAT通用驱动代码，适用于IT8951驱动的并口墨水屏，支持树莓派和STM32

## English ## 
e-Paper IT8951 Driver HAT universal Driver code, suitable for IT8951 Driver parallel ink screen, support Raspberry Pi and STM32

---

## 📚 Further Documentation

See the [`/docs`](./docs) directory for:
- In-depth guides on building and running the library
- Architecture and code structure
- How to display images on WaveShare e-ink devices
- Troubleshooting and common hacks for Raspberry Pi Zero W2

## 📚 API Documentation

- [Quick API Reference](docs/api.md) - Most commonly used functions
- [Browse the full API reference (HTML, Doxygen)](docs/api/html/index.html)
- To regenerate the docs: run `make apidocs`