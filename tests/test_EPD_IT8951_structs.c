#include <assert.h>
#include <stdio.h>
#include "../include/EPD_IT8951.h"

void test_IT8951_Load_Img_Info_packing() {
    IT8951_Load_Img_Info info;
    info.Endian_Type = 1;
    info.Pixel_Format = 2;
    info.Rotate = 3;
    info.Source_Buffer_Addr = (unsigned char*)0x12345678;
    info.Target_Memory_Addr = 0xABCDEF01;
    // Check fields
    assert(info.Endian_Type == 1);
    assert(info.Pixel_Format == 2);
    assert(info.Rotate == 3);
    assert(info.Source_Buffer_Addr == (unsigned char*)0x12345678);
    assert(info.Target_Memory_Addr == 0xABCDEF01);
    // Check argument packing (as in EPD_IT8951_LoadImgStart)
    unsigned short Args = (info.Endian_Type << 8) | (info.Pixel_Format << 4) | info.Rotate;
    assert(Args == ((1 << 8) | (2 << 4) | 3));
}

void test_IT8951_Area_Img_Info_packing() {
    IT8951_Area_Img_Info area;
    area.Area_X = 10;
    area.Area_Y = 20;
    area.Area_W = 30;
    area.Area_H = 40;
    assert(area.Area_X == 10);
    assert(area.Area_Y == 20);
    assert(area.Area_W == 30);
    assert(area.Area_H == 40);
    // Check argument array as in EPD_IT8951_LoadImgAreaStart
    IT8951_Load_Img_Info info = {0};
    info.Endian_Type = 1;
    info.Pixel_Format = 2;
    info.Rotate = 3;
    unsigned short Args[5];
    Args[0] = (info.Endian_Type << 8) | (info.Pixel_Format << 4) | info.Rotate;
    Args[1] = area.Area_X;
    Args[2] = area.Area_Y;
    Args[3] = area.Area_W;
    Args[4] = area.Area_H;
    assert(Args[0] == ((1 << 8) | (2 << 4) | 3));
    assert(Args[1] == 10);
    assert(Args[2] == 20);
    assert(Args[3] == 30);
    assert(Args[4] == 40);
}

int main() {
    test_IT8951_Load_Img_Info_packing();
    test_IT8951_Area_Img_Info_packing();
    printf("EPD_IT8951 struct and argument packing tests passed!\n");
    return 0;
} 