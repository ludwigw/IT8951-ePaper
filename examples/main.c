#include "../include/DEV_Config.h"
#include "example.h"
#include "../include/GUI_BMPfile.h"
#include "../include/EPD_IT8951.h"
#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include <stdlib.h>     //exit()
#include <signal.h>     //signal()

#define Enhance false

#define USE_Normal_Demo true

UWORD VCOM = 2510;

IT8951_Dev_Info Dev_Info = {0, 0};
UWORD Panel_Width;
UWORD Panel_Height;
UDOUBLE Init_Target_Memory_Addr;
int epd_mode = 0;	//0: no rotate, no mirror
					//1: no rotate, horizontal mirror, for 10.3inch
					//2: no totate, horizontal mirror, for 5.17inch
					//3: no rotate, no mirror, isColor, for 6inch color
					
void  Handler(int signo){
    Debug("\r\nHandler:exit\r\n");
    if(Refresh_Frame_Buf != NULL){
        free(Refresh_Frame_Buf);
        Debug("free Refresh_Frame_Buf\r\n");
        Refresh_Frame_Buf = NULL;
    }
    if(Panel_Frame_Buf != NULL){
        free(Panel_Frame_Buf);
        Debug("free Panel_Frame_Buf\r\n");
        Panel_Frame_Buf = NULL;
    }
    if(Panel_Area_Frame_Buf != NULL){
        free(Panel_Area_Frame_Buf);
        Debug("free Panel_Area_Frame_Buf\r\n");
        Panel_Area_Frame_Buf = NULL;
    }
    if(bmp_src_buf != NULL){
        free(bmp_src_buf);
        Debug("free bmp_src_buf\r\n");
        bmp_src_buf = NULL;
    }
    if(bmp_dst_buf != NULL){
        free(bmp_dst_buf);
        Debug("free bmp_dst_buf\r\n");
        bmp_dst_buf = NULL;
    }
	if(Dev_Info.Panel_W != 0){
		Debug("Going to sleep\r\n");
		EPD_IT8951_Sleep();
	}
    DEV_Module_Exit();
    exit(0);
}


int main(int argc, char *argv[])
{
    //Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if (argc < 2) {
        printf("Usage: %s <bmp_path> [vcom] [mode]\n", argv[0]);
        printf("  <bmp_path>: Path to BMP image file\n");
        printf("  [vcom]: VCOM voltage (default: 0, use panel default)\n");
        printf("  [mode]: Display mode (default: 2, GC16)\n");
        return 1;
    }
    const char *bmp_path = argv[1];
    UWORD vcom = (argc > 2) ? (UWORD)atoi(argv[2]) : 0;
    UWORD mode = (argc > 3) ? (UWORD)atoi(argv[3]) : 2; // GC16_Mode

    int result = EPD_IT8951_DisplayBMP(bmp_path, vcom, mode);
    if (result == 0) {
        printf("Image displayed successfully!\n");
    } else {
        printf("Failed to display image (error %d)\n", result);
    }
    return result;
}
