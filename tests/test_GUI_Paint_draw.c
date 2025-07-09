#include <assert.h>
#include <stdio.h>
#include "../include/GUI_Paint.h"

unsigned char buf[100 * 100];

void test_draw_point() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_DrawPoint(2, 3, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    unsigned int addr = 2 * (Paint.BitsPerPixel) / 8 + 3 * Paint.WidthByte;
    assert(Paint.Image[addr] == BLACK);
}

void test_draw_line() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_DrawLine(0, 0, 0, 5, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    for (int y = 0; y <= 5; ++y) {
        unsigned int addr = 0 * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
        assert(Paint.Image[addr] == BLACK);
    }
}

void test_draw_rectangle() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_DrawRectangle(1, 1, 3, 3, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    for (int x = 1; x <= 3; ++x) {
        for (int y = 1; y <= 3; ++y) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            assert(Paint.Image[addr] == BLACK);
        }
    }
}

void test_draw_circle() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_DrawCircle(5, 5, 2, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Just check the center is not set, but at least one pixel on the circle is black
    int found = 0;
    for (int x = 3; x <= 7; ++x) {
        for (int y = 3; y <= 7; ++y) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            if (Paint.Image[addr] == BLACK) found = 1;
        }
    }
    assert(found);
}

void test_bit_depths() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(1);
    Paint_SetPixel(1, 1, BLACK);
    unsigned int addr1 = 1 / 8 + 1 * Paint.WidthByte;
    assert((Paint.Image[addr1] & (1 << 6)) == 0);

    Paint_SetBitsPerPixel(2);
    Paint_SetPixel(2, 2, BLACK);
    unsigned int addr2 = 2 / 4 + 2 * Paint.WidthByte;
    assert(Paint.Image[addr2] != WHITE);

    Paint_SetBitsPerPixel(4);
    Paint_SetPixel(3, 3, BLACK);
    unsigned int addr4 = 3 / 2 + 3 * Paint.WidthByte;
    assert(Paint.Image[addr4] != WHITE);

    Paint_SetBitsPerPixel(8);
    Paint_SetPixel(4, 4, BLACK);
    unsigned int addr8 = 4 + 4 * Paint.WidthByte;
    assert(Paint.Image[addr8] == BLACK);
}

int main() {
    test_draw_point();
    test_draw_line();
    test_draw_rectangle();
    test_draw_circle();
    test_bit_depths();
    printf("All GUI_Paint drawing and bit depth tests passed!\n");
    return 0;
} 