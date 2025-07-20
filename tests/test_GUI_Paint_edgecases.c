#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/GUI_Paint.h"

unsigned char buf[100];

void test_out_of_bounds() {
    Paint_NewImage(buf, 5, 5, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    // Try to draw a point outside the buffer
    Paint_DrawPoint(10, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    // Try to draw a rectangle partially outside
    Paint_DrawRectangle(3, 3, 7, 7, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // Only (3,3)-(4,4) should be affected
    printf("Buffer after out-of-bounds rectangle:\n");
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            printf("%c", Paint.Image[addr] == BLACK ? '#' : '.');
        }
        printf("\n");
    }
    // All pixels should be BLACK after the out-of-bounds rectangle
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            assert(Paint.Image[addr] == BLACK);
        }
    }
}

void test_rotation() {
    Paint_NewImage(buf, 3, 3, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    // Draw a diagonal from top-left to bottom-right
    for (int i = 0; i < 3; ++i) {
        Paint_DrawPoint(i, i, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    }
    // Rotate 90 and check the diagonal is now from top-right to bottom-left
    Paint_SetRotate(ROTATE_90);
    int found = 0;
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            if (x + y == 2 && Paint.Image[addr] == BLACK)
                found++;
        }
    }
    assert(found == 3);
}

int main() {
    test_out_of_bounds();
    test_rotation();
    printf("Edge case and rotation tests passed!\n");
    return 0;
} 