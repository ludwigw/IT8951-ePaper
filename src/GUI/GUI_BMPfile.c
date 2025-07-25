/**
 * @file GUI_BMPfile.c
 * @brief Implementation of BMP image file loading and parsing for e-Paper displays.
 *
 * Provides functions for reading BMP files, parsing headers, and rendering images
 * onto the e-Paper display buffer. Handles various bit depths and color palettes.
 *
 * @author Waveshare
 * @date 2018-11-12
 */

#include "GUI_BMPfile.h"
#include "GUI_Paint.h"
#include "../../include/Debug.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>//exit()
#include <string.h>//memset()
#include <math.h>//memset()
#include <stdio.h>


//global variables related to BMP picture display
UBYTE *bmp_dst_buf = NULL;
UBYTE *bmp_src_buf = NULL;
UDOUBLE bmp_width, bmp_height;
UBYTE  bmp_BitCount;
UDOUBLE bytesPerLine;
UDOUBLE imageSize;
UDOUBLE skip;
BMPRGBQUAD  palette[256];
extern UBYTE isColor;

static void Bitmap_format_Matrix(UBYTE *dst,UBYTE *src)
{
	UDOUBLE i,j,k;
    UBYTE *psrc = src;
    UBYTE *pdst = dst;
    UBYTE *p = psrc;
	UBYTE temp;
	UDOUBLE count;
	
	//Since the bmp storage is from the back to the front, it needs to be converted in reverse order.
	switch(bmp_BitCount)
	{
		case 1:
			pdst += (bmp_width * bmp_height);
			
			for(i=0;i<bmp_height;i++)
			{
				pdst -= bmp_width;
				count = 0;
				for (j=0;j<(bmp_width+7)/8;j++)
				{
					temp = p[j];
					
					for (k=0;k<8;k++)
					{
						pdst[0]= ((temp & (0x80>>k)) >> (7-k));
						count++;
						pdst++;
						if (count == bmp_width)
						{
							break;
						}
					}
				}
				pdst -= bmp_width;
				p += bytesPerLine;
			}
		break;
		case 4:
			pdst += (bmp_width * bmp_height);

			for(i=0;i<bmp_height;i++)
			{
				pdst -= bmp_width;
				count = 0;
				for (j=0;j<(bmp_width+1)/2;j++)
				{
					temp = p[j];
					pdst[0]= ((temp & 0xf0) >> 4);
					count++;
					pdst++;
					if (count == bmp_width)
					{
						break;
					}

					pdst[0] = temp & 0x0f;
					count++;
					pdst++;
					if (count == bmp_width)
					{
						break;
					}
				}
				pdst -= bmp_width;
				p += bytesPerLine;
			}
		break;
		case 8:
			pdst += (bmp_width*bmp_height);
			for(i=0;i<bmp_height;i++)
			{
				p = psrc+(i+1)*bytesPerLine;
				p -= skip;
				for(j=0;j<bmp_width;j++)
				{
					pdst -= 1;
					p -= 1;
					pdst[0] = p[0];
				}
			}
		break;
		case 16:
			pdst += (bmp_width*bmp_height*2);
			for(i=0;i<bmp_height;i++)
			{
				p = psrc+(i+1)*bytesPerLine;
				p -= skip;
				for(j=0;j<bmp_width;j++)
				{
					pdst -= 2;
					p -= 2;
					pdst[0] = p[1];
					pdst[1] = p[0];
				}
			}
		break;
		case 24:
			pdst += (bmp_width*bmp_height*3);
			for(i=0;i<bmp_height;i++)
			{
				p = psrc+(i+1)*bytesPerLine;
				p -= skip;
				for(j=0;j<bmp_width;j++)
				{
					pdst -= 3;
					p -= 3;
					pdst[0] = p[2];
					pdst[1] = p[1];
					pdst[2] = p[0];
				}
			}
		break;
		case 32:
			pdst += (bmp_width*bmp_height*4);
			for(i=0;i<bmp_height;i++)
			{
				p = psrc+(i+1)*bmp_width*4;
				for(j=0;j<bmp_width;j++)
				{
					pdst -= 4;
					p -= 4;
					pdst[0] = p[2];
					pdst[1] = p[1];
					pdst[2] = p[0];
					pdst[3] = p[3];
				}
			}
		break;
		
		default:
		break;
	}	
}

static void DrawMatrix(UWORD Xpos, UWORD Ypos,UWORD Width, UWORD High,const UBYTE* Matrix)
{
	UWORD i,j,x,y;
	UBYTE R,G,B;
	UBYTE temp1,temp2;
	double Gray;
	
	for (y=0,j=Ypos;y<High;y++,j++)
	{
 		for (x=0,i=Xpos;x<Width;x++,i++)
		{
			switch(bmp_BitCount)
			{
				case 1:
				case 4:
				case 8:
					R = palette[Matrix[(y*Width+x)]].rgbRed;
					G = palette[Matrix[(y*Width+x)]].rgbGreen;
					B = palette[Matrix[(y*Width+x)]].rgbBlue;
				break;
				
				case 16:
					temp1 = Matrix[(y*Width+x)*2];
					temp2 = Matrix[(y*Width+x)*2+1];
					R = (temp1 & 0x7c)<<1;
					G = (((temp1 & 0x03) << 3 ) | ((temp2&0xe0) >> 5))<<3;
					B = (temp2 & 0x1f)<<3;
				break;
				
				case 24:
					R = Matrix[(y*Width+x)*3];
					G = Matrix[(y*Width+x)*3+1];
					B = Matrix[(y*Width+x)*3+2];
				break;
				
				case 32:
					R = Matrix[(y*Width+x)*4];
					G = Matrix[(y*Width+x)*4+1];
					B = Matrix[(y*Width+x)*4+2];
				break;
				
				default:
				break;
			}
		
			Gray = (R*299 + G*587 + B*114 + 500) / 1000;
            if(isColor && i%3==2)
				Paint_SetPixel(i, j, Gray/2);
			else
				Paint_SetPixel(i, j, Gray);
		}
	}
}

/**
 * @brief Read and render a BMP file to the e-Paper display buffer.
 *
 * Opens the specified BMP file, parses its headers, and draws the image at the given
 * (x, y) position on the display buffer. Supports images of any size; if the image
 * exceeds the display area, it will be clipped.
 *
 * @param path Path to the BMP file.
 * @param x X coordinate on the display to start drawing.
 * @param y Y coordinate on the display to start drawing.
 * @return 0 on success, negative value on error.
 */
int GUI_ReadBmp(const char *path, UWORD x, UWORD y)
{
	//bmp file pointer
	FILE *fp;
	BMPFILEHEADER FileHead;
	BMPINFOHEADER InfoHead;
	UDOUBLE total_length;
	UBYTE *buf = NULL;
	UDOUBLE ret = -1;
	
	fp = fopen(path,"rb");
	if (fp == NULL)
	{
		return(-1);
	}
 
	ret = fread(&FileHead, sizeof(BMPFILEHEADER),1, fp);
	if (ret != 1)
	{
		Debug("Read header error!\n");
		fclose(fp);
		return(-2);
	}

	//Detect if it is a bmp image, since BMP file type is "BM"(0x4D42)
	if (FileHead.bType != 0x4D42)
	{
		Debug("It's not a BMP file\n");
		fclose(fp);
		return(-3);
	}
	
	// Comment out BMP info Debug prints, keep error Debug prints
	// Debug("*****************************************\n");
	// Debug("BMP_bSize:%d \n", FileHead.bSize);
 	// Debug("BMP_bOffset:%d \n", FileHead.bOffset);
	
	ret = fread((char *)&InfoHead, sizeof(BMPINFOHEADER),1, fp);
	if (ret != 1)
	{
		Debug("Read infoheader error!\n");
		fclose(fp);
		return(-4);
	}
	
	// Debug("BMP_biInfoSize:%d \n", InfoHead.biInfoSize);
 	// Debug("BMP_biWidth:%d \n", InfoHead.biWidth);
	// Debug("BMP_biHeight:%d \n", InfoHead.biHeight);
	// Debug("BMP_biPlanes:%d \n", InfoHead.biPlanes);
	// Debug("BMP_biBitCount:%d \n", InfoHead.biBitCount);
	// Debug("BMP_biCompression:%d \n", InfoHead.biCompression);
	// Debug("BMP_bimpImageSize:%d \n", InfoHead.bimpImageSize);
	// Debug("BMP_biXPelsPerMeter:%d \n", InfoHead.biXPelsPerMeter);
	// Debug("BMP_biYPelsPerMeter:%d \n", InfoHead.biYPelsPerMeter);
	// Debug("BMP_biClrUsed:%d \n", InfoHead.biClrUsed);
	// Debug("BMP_biClrImportant:%d \n", InfoHead.biClrImportant);
	
	total_length = FileHead.bSize-FileHead.bOffset;
	bytesPerLine=((InfoHead.biWidth*InfoHead.biBitCount+31)>>5)<<2;
	imageSize=bytesPerLine*InfoHead.biHeight;
	skip=(4-((InfoHead.biWidth*InfoHead.biBitCount)>>3))&3;
	
	// Debug("bimpImageSize:%d\n", InfoHead.bimpImageSize);
	// Debug("total_length:%d\n", total_length);
	// Debug("bytesPerLine = %d\n", bytesPerLine);
	// Debug("imageSize = %d\n", imageSize);
	// Debug("skip = %d\n", skip);
	// Debug("*****************************************\n");
	
    bmp_width = InfoHead.biWidth;
    bmp_height = InfoHead.biHeight;
	bmp_BitCount = InfoHead.biBitCount;
	
	//This is old code, but allocate imageSize byte memory is more reasonable
    bmp_src_buf = (UBYTE*)calloc(1,total_length);
	//bmp_src_buf = (UBYTE*)calloc(1,imageSize);
    if(bmp_src_buf == NULL){
        Debug("Load > malloc bmp out of memory!\n");
        return -1;
    }
	//This is old code, but allocate imageSize byte memory is more reasonable
	bmp_dst_buf = (UBYTE*)calloc(1,total_length);
	//bmp_dst_buf = (UBYTE*)calloc(1,imageSize);
    if(bmp_dst_buf == NULL){
        Debug("Load > malloc bmp out of memory!\n");
        return -2;
    }

	 //Jump to data area
    fseek(fp, FileHead.bOffset, SEEK_SET);
	
	//Bytes per line
    buf = bmp_src_buf;
    log_debug("[BMP] total_length: %llu", total_length);
    // Print file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, FileHead.bOffset, SEEK_SET); // Reset to data offset after checking file size
    log_debug("[BMP] File size: %ld bytes", file_size);
    log_debug("[BMP] FileHead.bOffset: %u", FileHead.bOffset);
    fseek(fp, FileHead.bOffset, SEEK_SET);
    log_debug("[BMP] ftell after seek: %ld", ftell(fp));
    size_t bytes_read = fread(bmp_src_buf, 1, total_length, fp);
    log_debug("[BMP] Bytes actually read for pixel data: %zu", bytes_read);
    if (bytes_read < total_length) {
        log_warn("[BMP] Error: Only read %zu of %llu bytes of pixel data!", bytes_read, total_length);
        fclose(fp);
        free(bmp_src_buf);
        free(bmp_dst_buf);
        return -6;
    }

	//Jump to color pattern board
	switch(bmp_BitCount)
	{	
		case 1:
			fseek(fp, 54, SEEK_SET);
			ret = fread(palette,1,4*2,fp);
			if (ret != 8) 
			{
				Debug("Error: fread != 8\n");
				return -5;
			}

			//this is old code, will likely result in memory leak if use 1bp source bmp image
			 
			bmp_dst_buf = (UBYTE*)calloc(1,InfoHead.biWidth * InfoHead.biHeight);
			if(bmp_dst_buf == NULL)
			{
				Debug("Load > malloc bmp out of memory!\n");
				return -5;
			}
			
		break;
		
		case 4:
			fseek(fp, 54, SEEK_SET);
			ret = fread(palette,1,4*16,fp);
			if (ret != 64) 
			{
				Debug("Error: fread != 64\n");
				return -5;
			}
			//this is old code, will likely result in memory leak if use 4bp source bmp image
			
			bmp_dst_buf = (UBYTE*)calloc(1,InfoHead.biWidth * InfoHead.biHeight);
			if(bmp_dst_buf == NULL)
			{
				Debug("Load > malloc bmp out of memory!\n");
				return -5;
			}
			
		break;
		
		case 8:
			fseek(fp, 54, SEEK_SET);

			ret = fread(palette,1,4*256,fp);

			if (ret != 1024) 
			{
				Debug("Error: fread != 1024\n");
				return -5;
			}
		break;
		
		default:
		break;
	}

	log_debug("[BMP] BMP biBitCount: %d", InfoHead.biBitCount);
	log_debug("[BMP] BMP biWidth: %d", InfoHead.biWidth);
	log_debug("[BMP] BMP biHeight: %d", InfoHead.biHeight);
	if (InfoHead.biBitCount == 1) {
        log_trace("[BMP] Palette size: 2");
        for (int i = 0; i < 2; ++i) {
            log_trace("[BMP] Palette[%d]: R=%d G=%d B=%d", i, palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue);
        }
    } else if (InfoHead.biBitCount == 4) {
        log_trace("[BMP] Palette size: 16");
        for (int i = 0; i < 16; ++i) {
            log_trace("[BMP] Palette[%d]: R=%d G=%d B=%d", i, palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue);
        }
    } else if (InfoHead.biBitCount == 8) {
        log_trace("[BMP] Palette size: 256");
        for (int i = 0; i < 16; ++i) {
            log_trace("[BMP] Palette[%d]: R=%d G=%d B=%d", i, palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue);
        }
    }
    log_trace("[BMP] First 64 bytes of BMP pixel data:");
    char pixel_row[256] = {0};
    for (int i = 0; i < 64 && i < total_length; ++i) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%02X ", bmp_src_buf[i]);
        strcat(pixel_row, buf);
        if ((i+1) % 16 == 0) {
            log_trace("[BMP] %s", pixel_row);
            pixel_row[0] = '\0';
        }
    }
    if (64 % 16 != 0) {
        log_trace("[BMP] %s", pixel_row);
    }

	Bitmap_format_Matrix(bmp_dst_buf,bmp_src_buf);
	DrawMatrix(x, y,InfoHead.biWidth, InfoHead.biHeight, bmp_dst_buf);

    free(bmp_src_buf);
    free(bmp_dst_buf);

	bmp_src_buf = NULL;
	bmp_dst_buf = NULL;

	fclose(fp);
	return(0);
}