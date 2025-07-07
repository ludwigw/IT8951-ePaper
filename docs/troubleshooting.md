# Troubleshooting Guide

This guide covers common issues and solutions when using the IT8951-ePaper library to display images on a WaveShare e-Paper device from a Raspberry Pi.

---

## 1. No Image or Blank Screen
- **Check wiring**: Ensure all connections between the Pi and the e-Paper display are secure and correct.
- **SPI enabled**: Make sure SPI is enabled in `raspi-config` and/or `/boot/config.txt`.
- **VCOM value**: Use the correct VCOM value (printed on the FPC cable) when running the program.

## 2. Image is Distorted, Mirrored, or Rotated
- **Display mode**: Try different display modes (0, 1, 2, 3) as required by your panel.
- **Image rotation**: Ensure your BMP is rotated to match the display orientation (use the provided script).

## 3. Image Not Updating or Partial Updates
- **Full refresh**: Some modes require a full refresh (INIT or GC16) after several partial (A2) updates.
- **Alignment**: For some displays, the image width must be 4-byte aligned (see code and docs).

## 4. Program Crashes or Memory Errors
- **Image size**: Ensure your BMP matches the display's resolution and bit depth.
- **Memory leaks**: The code attempts to free buffers, but check for memory leaks if running custom code.

## 5. Build or Library Issues
- **Dependencies**: Make sure you have the required libraries (e.g., `bcm2835`, `gpiod`, `ImageMagick` for conversion script).
- **Clean build**: Run `make clean` before rebuilding if you change code or libraries.

## 6. Slow Updates
- **Bit depth**: Use 4bpp for best balance of speed and quality.
- **SPI speed**: Some Pi models have different optimal SPI speeds (see code comments).

## 7. Other Issues
- **Debug output**: Enable debug output for more information (see `Debug.h`).
- **Consult code comments**: Many files have helpful comments for troubleshooting.

---

If you are still having trouble, consult the [quickstart.md](./quickstart.md) and [architecture.md](./architecture.md) files, or check the Waveshare Wiki and forums for your specific display model. 