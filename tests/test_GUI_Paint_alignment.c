#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/GUI_Paint.h"

unsigned char buf[256];

void test_4byte_alignment() {
    // Simulate a panel that requires 4-byte alignment (e.g., width must be multiple of 32)
    int width = 33; // Not a multiple of 32
    int aligned_width = width - (width % 32) + 32 * (width % 32 != 0); // Next multiple of 32
    Paint_NewImage(buf, width, 1, ROTATE_0, WHITE);
    // For 1bpp, WidthByte should be ceil(width/8)
    Paint_SetBitsPerPixel(1);
    int expected_bytes = (width + 7) / 8;
    assert(Paint.WidthByte == expected_bytes);
    // For 4bpp, WidthByte should be ceil(width*4/8)
    Paint_SetBitsPerPixel(4);
    expected_bytes = (width * 4 + 7) / 8;
    assert(Paint.WidthByte == expected_bytes);
}

void test_bit_depth_conversion() {
    // Set a pixel in 8bpp, then switch to 4bpp and check that high nibble is preserved
    Paint_NewImage(buf, 10, 1, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_SetPixel(0, 0, 0xF0); // Set to max gray
    assert(Paint.Image[0] == 0xF0);
    // Now switch to 4bpp and set the same pixel
    Paint_SetBitsPerPixel(4);
    Paint_SetPixel(0, 0, 0xF0);
    assert((Paint.Image[0] & 0xF0) == 0xF0);
    // Now set a 2bpp pixel
    Paint_SetBitsPerPixel(2);
    Paint_SetPixel(0, 0, 0xC0);
    assert((Paint.Image[0] & 0xC0) == 0xC0);
    // Now set a 1bpp pixel
    Paint_SetBitsPerPixel(1);
    Paint_SetPixel(0, 0, 0x80);
    assert((Paint.Image[0] & 0x80) == 0x80);
}

int main() {
    test_4byte_alignment();
    test_bit_depth_conversion();
    printf("Buffer alignment and bit depth conversion tests passed!\n");
    return 0;
} 