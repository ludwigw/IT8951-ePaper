#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/EPD_IT8951.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        printf("Usage: epdraw <bmp_path> [vcom] [mode]\n");
        printf("  <bmp_path>: Path to BMP image file\n");
        printf("  [vcom]: VCOM voltage (default: 0, use panel default)\n");
        printf("  [mode]: Display mode (default: 2, GC16)\n");
        printf("\nDisplay modes:\n");
        printf("  0: No rotate, no mirroring (default)\n");
        printf("  1: No rotate, X mirroring (10.3\" e-Paper HAT)\n");
        printf("  2: No rotate, X mirroring (5.2\" e-Paper) - Recommended\n");
        printf("  3: No rotate, no mirroring, color (6\" color)\n");
        return 1;
    }
    const char *bmp_path = argv[1];
    int vcom = (argc > 2) ? atoi(argv[2]) : 0;
    int mode = (argc > 3) ? atoi(argv[3]) : 2; // GC16_Mode

    // Check if the BMP file exists before proceeding
    FILE *fp = fopen(bmp_path, "rb");
    if (!fp) {
        fprintf(stderr, "Error: BMP file '%s' not found or not readable.\n", bmp_path);
        return 2;
    }
    fclose(fp);

    int result = EPD_IT8951_DisplayBMP(bmp_path, vcom, mode);
    if (result == 0) {
        printf("Image displayed successfully!\n");
    } else {
        printf("Failed to display image (error %d)\n", result);
    }
    return result;
} 