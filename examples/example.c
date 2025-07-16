#include "example.h"

#include <time.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "../include/EPD_IT8951.h"
#include "../include/GUI_Paint.h"
#include "../include/GUI_BMPfile.h"
#include "../include/Debug.h"

UBYTE *Refresh_Frame_Buf = NULL;

UBYTE *Panel_Frame_Buf = NULL;
UBYTE *Panel_Area_Frame_Buf = NULL;

bool Four_Byte_Align = false;

extern int epd_mode;
extern UWORD VCOM;
extern UBYTE isColor;
/******************************************************************************
function: Change direction of display, Called after Paint_NewImage()
parameter:
    mode: display mode
******************************************************************************/
static void Epd_Mode(int mode)
{
	if(mode == 3) {
		Paint_SetRotate(ROTATE_0);
		Paint_SetMirroring(MIRROR_NONE);
		isColor = 1;
	}else if(mode == 2) {
		Paint_SetRotate(ROTATE_0);
		Paint_SetMirroring(MIRROR_HORIZONTAL);
	}else if(mode == 1) {
		Paint_SetRotate(ROTATE_0);
		Paint_SetMirroring(MIRROR_HORIZONTAL);
	}else {
		Paint_SetRotate(ROTATE_0);
		Paint_SetMirroring(MIRROR_NONE);
	}
}

/******************************************************************************
function: Display_BMP_Example
parameter:
    Panel_Width: Width of the panel
    Panel_Height: Height of the panel
    Init_Target_Memory_Addr: Memory address of IT8951 target memory address
    BitsPerPixel: Bits Per Pixel, 2^BitsPerPixel = grayscale
    bmp_path: Path to the BMP file
******************************************************************************/
UBYTE Display_BMP_Example(UWORD Panel_Width, UWORD Panel_Height, UDOUBLE Init_Target_Memory_Addr, UBYTE BitsPerPixel, const char* bmp_path){
    UWORD WIDTH;
    if(Four_Byte_Align == true){
        WIDTH  = Panel_Width - (Panel_Width % 32);
    }else{
        WIDTH = Panel_Width;
    }
    UWORD HEIGHT = Panel_Height;

    UDOUBLE Imagesize;

    Imagesize = ((WIDTH * BitsPerPixel % 8 == 0)? (WIDTH * BitsPerPixel / 8 ): (WIDTH * BitsPerPixel / 8 + 1)) * HEIGHT;
    if((Refresh_Frame_Buf = (UBYTE *)malloc(Imagesize)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }

    Paint_NewImage(Refresh_Frame_Buf, WIDTH, HEIGHT, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
	Epd_Mode(epd_mode);
    Paint_SetBitsPerPixel(BitsPerPixel);
    Paint_Clear(WHITE);

    GUI_ReadBmp(bmp_path, 0, 0);

    //you can draw your character and pattern on the image, for color definition of all BitsPerPixel, you can refer to GUI_Paint.h, 
    //Paint_DrawRectangle(50, 50, WIDTH/2, HEIGHT/2, 0x30, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
    //Paint_DrawCircle(WIDTH*3/4, HEIGHT/4, 100, 0xF0, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    //Paint_DrawNum(WIDTH/4, HEIGHT/5, 709, &Font20, 0x30, 0xB0);

    switch(BitsPerPixel){
        case BitsPerPixel_8:{
            Paint_DrawString_EN(10, 10, "8 bits per pixel 16 grayscale", &Font24, 0xF0, 0x00);
            EPD_IT8951_8bp_Refresh(Refresh_Frame_Buf, 0, 0, WIDTH,  HEIGHT, false, Init_Target_Memory_Addr);
            break;
        }
        case BitsPerPixel_4:{
            Paint_DrawString_EN(10, 10, "4 bits per pixel 16 grayscale", &Font24, 0xF0, 0x00);
            EPD_IT8951_4bp_Refresh(Refresh_Frame_Buf, 0, 0, WIDTH,  HEIGHT, false, Init_Target_Memory_Addr,false);
            break;
        }
        case BitsPerPixel_2:{
            Paint_DrawString_EN(10, 10, "2 bits per pixel 4 grayscale", &Font24, 0xC0, 0x00);
            EPD_IT8951_2bp_Refresh(Refresh_Frame_Buf, 0, 0, WIDTH,  HEIGHT, false, Init_Target_Memory_Addr,false);
            break;
        }
        case BitsPerPixel_1:{
            Paint_DrawString_EN(10, 10, "1 bit per pixel 2 grayscale", &Font24, 0x80, 0x00);
            EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, 0, 0, WIDTH,  HEIGHT, A2_Mode, Init_Target_Memory_Addr,false);
            break;
        }
    }

    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Refresh_Frame_Buf = NULL;
    }
    return 0;
}

