#include <assert.h>
#include <stdio.h>
#include "../include/GUI_Paint.h"

// Dummy buffer for testing
unsigned char test_image[100 * 100];

void test_Paint_NewImage() {
    Paint_NewImage(test_image, 100, 100, ROTATE_0, WHITE);
    assert(Paint.Width == 100);
    assert(Paint.Height == 100);
    assert(Paint.Image == test_image);
    assert(Paint.Color == WHITE);
}

void test_Paint_SetPixel_and_GetPixel() {
    Paint_NewImage(test_image, 10, 10, ROTATE_0, WHITE);
    Paint_SetPixel(5, 5, BLACK);
    // There is no direct get pixel, but we can check the buffer
    unsigned int addr = 5 * (Paint.BitsPerPixel) / 8 + 5 * Paint.WidthByte;
    assert(Paint.Image[addr] == BLACK);
}

void test_Paint_SetRotate() {
    Paint_NewImage(test_image, 10, 20, ROTATE_0, WHITE);
    Paint_SetRotate(ROTATE_90);
    assert(Paint.Rotate == ROTATE_90);
    Paint_SetRotate(ROTATE_180);
    assert(Paint.Rotate == ROTATE_180);
    Paint_SetRotate(ROTATE_270);
    assert(Paint.Rotate == ROTATE_270);
}

int main() {
    test_Paint_NewImage();
    test_Paint_SetPixel_and_GetPixel();
    test_Paint_SetRotate();
    printf("All GUI_Paint tests passed!\n");
    return 0;
} 