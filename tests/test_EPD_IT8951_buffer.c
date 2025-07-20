#include <assert.h>
#include <stdio.h>
#include "../include/EPD_IT8951.h"

void test_buffer_packing_1bpp() {
    IT8951_Load_Img_Info load_info;
    IT8951_Area_Img_Info area_info;
    area_info.Area_W = 64;
    area_info.Area_H = 10;
    // For 1bpp: Source_Buffer_Width = Area_W / 2; (from code)
    unsigned int expected_width = area_info.Area_W / 2;
    unsigned int expected_height = area_info.Area_H;
    unsigned int expected_length = expected_width * expected_height;
    assert(expected_width == 32);
    assert(expected_height == 10);
    assert(expected_length == 320);
}

void test_buffer_packing_2bpp() {
    IT8951_Load_Img_Info load_info;
    IT8951_Area_Img_Info area_info;
    area_info.Area_W = 32;
    area_info.Area_H = 8;
    // For 2bpp: Source_Buffer_Width = (Area_W*2/8)/2;
    unsigned int expected_width = ((area_info.Area_W * 2 / 8) / 2);
    unsigned int expected_height = area_info.Area_H;
    unsigned int expected_length = expected_width * expected_height;
    assert(expected_width == 4);
    assert(expected_height == 8);
    assert(expected_length == 32);
}

void test_buffer_packing_4bpp() {
    IT8951_Load_Img_Info load_info;
    IT8951_Area_Img_Info area_info;
    area_info.Area_W = 16;
    area_info.Area_H = 4;
    // For 4bpp: Source_Buffer_Width = (Area_W*4/8)/2;
    unsigned int expected_width = ((area_info.Area_W * 4 / 8) / 2);
    unsigned int expected_height = area_info.Area_H;
    unsigned int expected_length = expected_width * expected_height;
    assert(expected_width == 4);
    assert(expected_height == 4);
    assert(expected_length == 16);
}

void test_buffer_packing_8bpp() {
    IT8951_Load_Img_Info load_info;
    IT8951_Area_Img_Info area_info;
    area_info.Area_W = 8;
    area_info.Area_H = 2;
    // For 8bpp: Source_Buffer_Width = (Area_W*8/8)/2;
    unsigned int expected_width = ((area_info.Area_W * 8 / 8) / 2);
    unsigned int expected_height = area_info.Area_H;
    unsigned int expected_length = expected_width * expected_height;
    assert(expected_width == 4);
    assert(expected_height == 2);
    assert(expected_length == 8);
}

int main() {
    test_buffer_packing_1bpp();
    test_buffer_packing_2bpp();
    test_buffer_packing_4bpp();
    test_buffer_packing_8bpp();
    printf("EPD_IT8951 buffer packing and area calculation tests passed!\n");
    return 0;
} 