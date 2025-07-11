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
./bin/epdraw image.bmp    # Display your image
```

The 'epdraw' CLI tool is the recommended way to use this library for image display.

See the [`/docs`](./docs) directory for detailed guides and troubleshooting.

> **Documentation Update:**
> 
> For a detailed guide on understanding, building, and using this library to display images on WaveShare e-ink devices from a Raspberry Pi, see the new documentation in the [`/docs`](./docs) directory.
> 
> This includes step-by-step instructions, architecture overview, and troubleshooting tips for common hacks and issues.

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