#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/GUI_Paint.h"
#include "../include/fonts.h"

unsigned char buf[200 * 200];

void test_draw_char_ascii() {
    Paint_NewImage(buf, 20, 20, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    // Draw 'A' at (0,0) with all font sizes
    sFONT* fonts[] = { &Font8, &Font12, &Font16, &Font20, &Font24 };
    int heights[] = { 8, 12, 16, 20, 24 };
    for (int i = 0; i < 5; ++i) {
        Paint_Clear(WHITE);
        Paint_DrawChar(0, 0, 'A', fonts[i], BLACK, WHITE);
        // Check at least one pixel in the expected area is black
        int found = 0;
        for (int y = 0; y < heights[i]; ++y) {
            for (int x = 0; x < fonts[i]->Width; ++x) {
                unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
                if (Paint.Image[addr] == BLACK) found = 1;
            }
        }
        assert(found);
    }
}

void test_draw_string_ascii() {
    Paint_NewImage(buf, 40, 20, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    // Draw "Hi" at (0,0) with all font sizes
    sFONT* fonts[] = { &Font8, &Font12, &Font16, &Font20, &Font24 };
    int heights[] = { 8, 12, 16, 20, 24 };
    for (int i = 0; i < 5; ++i) {
        Paint_Clear(WHITE);
        Paint_DrawString_EN(0, 0, "Hi", fonts[i], BLACK, WHITE);
        // Check at least one pixel in the expected area is black
        int found = 0;
        for (int y = 0; y < heights[i]; ++y) {
            for (int x = 0; x < fonts[i]->Width * 2; ++x) {
                unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
                if (Paint.Image[addr] == BLACK) found = 1;
            }
        }
        assert(found);
    }
}

void test_draw_empty_string() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(0, 0, "", &Font12, BLACK, WHITE);
    // Buffer should remain all white
    for (int i = 0; i < 100; ++i) {
        assert(Paint.Image[i] == WHITE);
    }
}

void test_draw_char_out_of_bounds() {
    Paint_NewImage(buf, 10, 10, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_Clear(WHITE);
    // Draw at out-of-bounds position
    Paint_DrawChar(20, 20, 'A', &Font12, BLACK, WHITE);
    // Buffer should remain all white
    for (int i = 0; i < 100; ++i) {
        assert(Paint.Image[i] == WHITE);
    }
}

void test_draw_cn_ascii() {
#ifdef Font12CN
    Paint_NewImage(buf, 40, 40, ROTATE_0, WHITE);
    Paint_SetBitsPerPixel(8);
    Paint_Clear(WHITE);
    // Draw ASCII with cFONT (should fallback to ASCII width)
    Paint_DrawString_CN(0, 0, "A", &Font12CN, BLACK, WHITE);
    int found = 0;
    for (int y = 0; y < Font12CN.Height; ++y) {
        for (int x = 0; x < Font12CN.ASCII_Width; ++x) {
            unsigned int addr = x * (Paint.BitsPerPixel) / 8 + y * Paint.WidthByte;
            if (Paint.Image[addr] == BLACK) found = 1;
        }
    }
    assert(found);
#endif
}

int main() {
    test_draw_char_ascii();
    test_draw_string_ascii();
    test_draw_empty_string();
    test_draw_char_out_of_bounds();
    test_draw_cn_ascii();
    printf("All Fonts/text rendering tests passed!\n");
    return 0;
} 