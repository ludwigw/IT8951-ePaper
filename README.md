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

## ⚠️ Platform Backends: BCM Library vs. Custom IOCTL (Waveshare) — Why This Matters

This library supports multiple hardware backends for controlling the e-Paper display, and **choosing the correct one is critical for your hardware to work reliably.**

### Background: The Confusion
- **BCM Backend:** Uses the `bcm2835` library, which is a popular C library for Raspberry Pi GPIO and SPI. It relies on the Pi's hardware SPI and GPIO subsystems, and is easy to use for most Pi projects.
- **Waveshare (Custom IOCTL) Backend:** The official Waveshare demo code does **not** use the bcm2835 library. Instead, it directly controls SPI and GPIO using Linux `ioctl` calls and the `gpiod` library. This gives it more precise control over chip select (CS), timing, and inter-byte delays, which are sometimes required for the IT8951 controller to work correctly.

### Why Does This Matter?
- Some e-Paper panels (especially the 10.3" D variant) are **very sensitive to SPI timing and CS line handling**.
- The BCM backend may not work on all hardware, because it cannot control inter-byte delays or manually toggle CS between bytes/commands.
- The Waveshare backend matches the official demo's low-level logic, and is more likely to work on all hardware, but is more complex and requires additional dependencies (`libgpiod-dev`).

### How to Choose
- **Default:** The Makefile uses `PLATFORM=BCM` by default for compatibility.
- **If you have display update or BUSY pin issues:** Try building with `PLATFORM=WAVESHARE`:
  ```sh
  make bin/epdraw PLATFORM=WAVESHARE
  ```
- This will use the custom IOCTL/gpiod backend, matching the official Waveshare demo logic.

### What Actually Happens
- The Makefile and codebase will only include one backend at a time (BCM, LGPIO, GPIOD, or WAVESHARE).
- The CLI and library will link against the correct platform-specific code for SPI and GPIO.
- If you use the WAVESHARE backend, you must have `libgpiod-dev` installed (the Dockerfile now includes this).

### Why Both?
- The BCM backend is simpler and works for most Pi models and Waveshare panels.
- The WAVESHARE backend is for advanced users, hardware debugging, or when the BCM backend fails due to timing/CS issues.
- This dual-backend approach is necessary because the IT8951 controller is very sensitive to low-level hardware details, and the official demo code is the only known-good reference for some panels.

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