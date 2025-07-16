# Quickstart: Displaying an Image on a WaveShare e-Paper Display

This guide will help you quickly get an image displayed on your WaveShare e-Paper device using a Raspberry Pi (including Pi Zero W2).

---

## 🚀 Quick Start: Display an Image in 3 Steps

### 1. Hardware Setup
- Connect your WaveShare e-Paper display to the Raspberry Pi according to the manufacturer's instructions.
- Ensure the SPI interface is enabled on your Pi.
- For Raspberry Pi 5, edit `/boot/config.txt`:
  - Comment out: `#dtparam=spi=on`
  - Add: `dtoverlay=spi0-0cs`
  - Reboot: `sudo reboot`

### 2. Build and Run
```sh
make bin/epdraw           # Build the CLI tool
./bin/epdraw myphoto.jpg # Display your image (any format: PNG, JPG, BMP, etc.)
```

### 3. That's it! 

The 'epdraw' CLI tool is the recommended way to use this library for image display. You can pass any image file (PNG, JPG, BMP, etc.) and it will be automatically converted to the correct format for your e-Paper display using ImageMagick.

**Optional parameters:**
```sh
./bin/epdraw image.bmp 2510 2    # Specify VCOM (-2.51V) and mode (GC16)
```

---

## Image Preparation

You do not need to manually convert your image. The CLI will automatically convert any image (PNG, JPG, etc.) to the correct BMP format for your e-Paper display using ImageMagick. Just pass your image file directly to the CLI.

**Display modes:**
- `0`: No rotate, no mirroring (default)
- `1`: No rotate, X mirroring (10.3" e-Paper HAT)
- `2`: No rotate, X mirroring (5.2" e-Paper) - **Recommended**
- `3`: No rotate, no mirroring, color (6" color)

---

## Troubleshooting

- If the image does not display, check your wiring and SPI configuration.
- Ensure your BMP is 16-color grayscale and properly rotated.
- Try different modes if the image appears mirrored or rotated.
- See [troubleshooting.md](./troubleshooting.md) for more help.

---

For more details, see [overview.md](./overview.md), [API Reference](./api.md), and the rest of the documentation in this directory. 