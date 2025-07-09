## 1 Program Description

### 1.1 Changelog

* 1. Uses 4bpp to refresh 16-level grayscale images, reducing SPI data transfer by half compared to the previous version, avoiding RAM and transfer time waste caused by using 8bpp for 16-level grayscale images.

* 2. SPI transfer speed doubled (effective on Raspberry Pi 3; not effective on Pi 4 due to higher CPU frequency).

* 3. Image refresh interval reduced to 1/4 of the previous version. In practice, the previous version took about 10 seconds to refresh a 16-level grayscale BMP image in GC16 mode on a 10.3-inch e-Paper (D); this example only takes about 3 seconds.

* 4. Fixed memory leak where BMP file buffers were allocated but not freed in the previous version.

* 5. Added support for 1bpp, 2bpp, 4bpp, and 8bpp modes.

* 6. Added A2 mode refresh example to demonstrate A2 mode refresh speed.

* 7. Added support for drawing points, lines, circles, rectangles, and characters, with grayscale selection for characters and patterns. Supports 1bpp, 2bpp, 4bpp, 8bpp. If 1bpp is selected, A2 mode refresh is also supported.

* 8. Added support for refreshing BMP images in 1bpp, 2bpp, 4bpp, 8bpp. If 1bpp is selected, A2 mode refresh is also supported.

* 9. Added GIF display example: multiple images can be written to IT8951 memory and displayed by refreshing different memory addresses, eliminating the need for RPi-to-IT8951 data transfer and achieving up to 7fps refresh rate.

* 10. Added frame rate test example to measure refresh rates for different area sizes, bpp modes, and refresh modes (A2, GC16).

* 11. Enhanced drive capability to avoid partial blur in some cases.

* 12. Added 4-byte alignment support for 6-inch e-Paper HAT and 6-inch HD e-Paper HAT to avoid display issues in 1bpp mode.

* 13. Program now requires VCOM input at runtime and fully clears the screen at exit to protect and extend the life of the display.

* 14. Optimized program structure, reduced coupling, and standardized some function and variable names.


### 1.2 Example Descriptions

#### 1.2.1 Sequentially Refresh 16 Grayscale Bars from Light to Dark
* Function: Display_ColorPalette_Example
* Demonstrates how to use 4bpp and GC16 mode to sequentially refresh 16 grayscale bars from light to dark.

#### 1.2.2 Draw Points, Lines, Circles, Rectangles, and Characters
* Function: Display_CharacterPattern_Example
* Demonstrates how to draw points, lines, circles, rectangles, and characters. Supports 1bpp, 2bpp, 4bpp, 8bpp. If 1bpp is used, A2 mode refresh is also supported.

#### 1.2.3 Display BMP Image
* Function: Display_BMP_Example
* Demonstrates how to refresh a BMP image. Supports 1bpp, 2bpp, 4bpp, 8bpp. If 1bpp is used, A2 mode refresh is also supported.

#### 1.2.4 A2 Mode Refresh Example
* Function: Dynamic_Refresh_Example
* Demonstrates how to use A2 mode for refreshing. The refresh area changes and grows with each refresh. After multiple refreshes, ghosting may appear, which is cleared using INIT mode. This example visually demonstrates A2 mode speed.

#### 1.2.5 Display GIF Image
* Function: Dynamic_GIF_Example
* Demonstrates how to refresh a GIF image. The GIF is split into 7 BMP images, which are written to consecutive IT8951 memory addresses. During display, images are read from memory and refreshed to the e-Paper display, achieving high refresh speed (about 7fps).

#### 1.2.6 Frame Rate Test
* Function: Check_FrameRate_Example
* Measures refresh rates for different area sizes, bpp modes, and refresh modes (A2, GC16). Automatically calculates and displays the frame rate (fps) for 10 frames.


### 1.3 Additional Notes
#### 1.3.1 Mode Descriptions

The IT8951 controller uses different firmware for different screen resolutions, each supporting different refresh modes. For details, see [Mode Description](http://www.waveshare.net/wiki/File:E-paper-mode-declaration.pdf). Modes used in these examples: INIT, GC16, A2.

| Mode | Features | 6inch/6inch HD | 7.8inch/9.7inch/10.3inch |
| ---- | ------------------------------------------------------------ | -------------- | ------------------------ |
| INIT | Erases/clears the display. Recommended after multiple A2 refreshes. | Mode0 | Mode0 |
| GC16 | 16-level grayscale update, best display quality | Mode2 | Mode2 |
| A2   | Black/white only, fastest refresh | Mode4 | Mode6 |

```c
// Basic mode definition
UBYTE INIT_Mode = 0;
UBYTE GC16_Mode = 2;
// A2_Mode's value is not fixed, depends on firmware LUT
UBYTE A2_Mode = 6;
```

```c
if( strcmp(LUT_Version, "M641") == 0 ){
    // 6inch e-Paper HAT(800,600), 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
    A2_Mode = 4;
    Four_Byte_Align = true;
}else if( strcmp(LUT_Version, "M841") == 0 ){
    // 9.7inch e-Paper HAT(1200,825)
    A2_Mode = 6;
}else if( strcmp(LUT_Version, "M841_TFA2812") == 0 ){
    // 7.8inch e-Paper HAT(1872,1404)
    A2_Mode = 6;
}else if( strcmp(LUT_Version, "M841_TFA5210") == 0 ){
    // 10.3inch e-Paper HAT(1872,1404)
    A2_Mode = 6;
}else{
    // default set to 6 as A2 Mode
    A2_Mode = 6;
}
```

#### 1.3.2 bpp Explanation

bpp (Bits Per Pixel) indicates how many bits each pixel uses. All screens support 1bpp, 2bpp, 4bpp, and 8bpp refresh modes.

* 1bpp
  * Each pixel uses 1 bit
  * 2 grayscale levels (2^1=2), suitable for A2 mode (black/white only)
  * 1 byte stores 8 pixels
  * Pixels stored in big-endian order in RAM (see diagram)
  * IT8951 uses little-endian by default, so conversion is needed

* 2bpp
  * Each pixel uses 2 bits
  * 4 grayscale levels (2^2=4)
  * 1 byte stores 4 pixels
  * Pixels stored in big-endian order in RAM (see diagram)
  * IT8951 uses little-endian by default, so conversion is needed

* 4bpp
  * Each pixel uses 4 bits
  * 16 grayscale levels (2^4=16)
  * 1 byte stores 2 pixels
  * Pixels stored in big-endian order in RAM (see diagram)
  * IT8951 uses little-endian by default, so conversion is needed
  * **Recommended: 4bpp for 16-level grayscale, half the data of 8bpp, same quality, faster transfer.**

* 8bpp
  * Each pixel uses 8 bits
  * 256 grayscale levels (2^8=256), but IT8951 only uses the high 4 bits (so 16 levels)
  * 1 byte stores 1 pixel
  * Pixels stored in big-endian order in RAM (see diagram)
  * IT8951 uses little-endian by default, so conversion is needed

* To get the required grayscale from the original image, extract the high bits from each 8-bit pixel. Example code:

```c
UDOUBLE Addr = X * (Paint.BitsPerPixel) / 8 + Y * Paint.WidthByte;
switch( Paint.BitsPerPixel ){
    case 8:{
        Paint.Image[Addr] = Color & 0xF0;
        break;
    }
    case 4:{
        Paint.Image[Addr] &= ~( (0xF0) >> (7 - (X*4+3)%8 ) );
        Paint.Image[Addr] |= (Color & 0xF0) >> (7 - (X*4+3)%8 );
        break;
    }
    case 2:{
        Paint.Image[Addr] &= ~( (0xC0) >> (7 - (X*2+1)%8 ) );
        Paint.Image[Addr] |= (Color & 0xC0) >> (7 - (X*2+1)%8 );
        break;
    }
    case 1:{
        Paint.Image[Addr] &= ~( (0x80) >> (7 - X%8) );
        Paint.Image[Addr] |= (Color & 0x80) >> (7 - X%8);
        break;
    }
}
```

#### 1.3.3 4-Byte Alignment Explanation

For 6-inch e-Paper HAT, 6-inch HD e-Paper HAT, and 6-inch HD touch e-Paper HAT, 1bpp mode requires 4-byte (32-bit) alignment for the X start and width, or the image will display incorrectly. Example:

```c
if( strcmp(LUT_Version, "M641") == 0 ){
    // 6inch e-Paper HAT(800,600), 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
    A2_Mode = 4;
    Four_Byte_Align = true;
}else if( strcmp(LUT_Version, "M841") == 0 ){
    // ...
}
```

```c
if(Four_Byte_Align == true){
    In_4bp_Refresh_Area_Width = Panel_Width - (Panel_Width % 32);
}else{
    In_4bp_Refresh_Area_Width = Panel_Width;
}
```
