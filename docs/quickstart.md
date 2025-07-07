# Quickstart: Displaying an Image on a WaveShare e-Paper Display

This guide will help you quickly get an image displayed on your WaveShare e-Paper device using a Raspberry Pi (including Pi Zero W2).

---

## 1. Hardware Setup
- Connect your WaveShare e-Paper display to the Raspberry Pi according to the manufacturer's instructions.
- Ensure the SPI interface is enabled on your Pi.
- For Raspberry Pi 5, edit `/boot/config.txt`:
  - Comment out: `#dtparam=spi=on`
  - Add: `dtoverlay=spi0-0cs`
  - Reboot: `sudo reboot`

## 2. Build the Library and Example Program
From the project root:

```sh
cd Raspberry
make -j4 LIB=BCM   # For most Pi models (default)
# or
make -j4 LIB=GPIOD # For Pi 5 (uses gpiod for GPIO)
```

This will produce an executable called `epd` in the `Raspberry` directory.

## 3. Prepare Your Image
- Convert your image to a 16-color grayscale BMP, rotated as needed for your display.
- Use the provided script:

```sh
cd Raspberry/pic
./convert_to_eink.sh your_image.png
# This creates your_image.bmp
```

## 4. Display the Image
- Find the VCOM value printed on your display's FPC cable (e.g., -2.51)
- Determine the display mode (see below)
- Run the example:

```sh
sudo ./epd -2.51 0 ./pic/your_image.bmp
```

- Mode 0: No rotate, no mirroring (default)
- Mode 1: No rotate, X mirroring (10.3" e-Paper HAT)
- Mode 2: No rotate, X mirroring (5.2" e-Paper)
- Mode 3: No rotate, no mirroring, color (6" color)

## 5. Troubleshooting
- If the image does not display, check your wiring and SPI configuration.
- Ensure your BMP is 16-color grayscale and properly rotated.
- Try different modes if the image appears mirrored or rotated.
- See [troubleshooting.md](./troubleshooting.md) for more help.

---

For more details, see [overview.md](./overview.md) and the rest of the documentation in this directory. 