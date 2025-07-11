#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>
#include "../include/EPD_IT8951.h"

#define MAX_PATH 1024
#define MAX_CMD 2048

/**
 * @brief Check if a file is a BMP by examining its header
 * @param filename Path to the file to check
 * @return 1 if BMP, 0 if not
 */
int is_bmp_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;
    
    unsigned char header[2];
    size_t read = fread(header, 1, 2, fp);
    fclose(fp);
    
    return (read == 2 && header[0] == 'B' && header[1] == 'M');
}

/**
 * @brief Check if ImageMagick is available (try both 'magick' and 'convert' commands)
 * @return 1 if available, 0 if not
 */
int check_imagemagick() {
    // Try modern 'magick' command first
    if (system("magick --version > /dev/null 2>&1") == 0) {
        return 1;
    }
    // Try legacy 'convert' command
    if (system("convert --version > /dev/null 2>&1") == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Get the appropriate ImageMagick command
 * @return "magick" or "convert" depending on what's available
 */
const char* get_imagemagick_cmd() {
    if (system("magick --version > /dev/null 2>&1") == 0) {
        return "magick";
    }
    return "convert";
}

/**
 * @brief Convert any image to BMP format suitable for e-Paper display
 * @param input_path Path to input image (any format supported by ImageMagick)
 * @param output_path Path for output BMP file
 * @param rotation Rotation in degrees (-90, 0, 90, 180)
 * @param colors Number of colors (16 for grayscale, 256 for color)
 * @return 0 on success, -1 on failure
 */
int convert_image_to_bmp(const char *input_path, const char *output_path, int rotation, int colors) {
    char cmd[MAX_CMD];
    const char* magick_cmd = get_imagemagick_cmd();
    
    // Build ImageMagick command with appropriate settings
    if (colors == 16) {
        // Grayscale conversion with dithering (like original script)
        snprintf(cmd, sizeof(cmd), 
            "%s \"%s\" -colors %d -dither FloydSteinberg -colorspace Gray -rotate %d \"%s\"",
            magick_cmd, input_path, colors, rotation, output_path);
    } else {
        // Color conversion (for color e-Paper)
        snprintf(cmd, sizeof(cmd), 
            "%s \"%s\" -colors %d -rotate %d \"%s\"",
            magick_cmd, input_path, colors, rotation, output_path);
    }
    
    printf("Converting image: %s\n", cmd);
    
    int result = system(cmd);
    if (result != 0) {
        fprintf(stderr, "Error: Image conversion failed (exit code %d)\n", result);
        return -1;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        printf("Usage: epdraw <image_path> [vcom] [mode]\n");
        printf("  <image_path>: Path to image file (any format: PNG, JPG, BMP, etc. - will be auto-converted)\n");
        printf("  [vcom]: VCOM voltage (default: 0, use panel default)\n");
        printf("  [mode]: Display mode (default: 2, GC16)\n");
        printf("\nDisplay modes:\n");
        printf("  0: INIT mode - Clear display (1bpp, no mirroring)\n");
        printf("  1: GC16 mode - High quality (4bpp, horizontal mirroring for 10.3\" e-Paper HAT)\n");
        printf("  2: GC16 mode - High quality (4bpp, horizontal mirroring for 5.2\" e-Paper) - Recommended\n");
        printf("  3: GC16 mode - Color (8bpp, no mirroring for 6\" color e-Paper)\n");
        printf("  4: A2 mode - Fast black/white (1bpp, no mirroring)\n");
        printf("  6: A2 mode - Fast black/white (1bpp, horizontal mirroring)\n");
        printf("\nThe mode automatically determines:\n");
        printf("  - Bit depth (1bpp, 4bpp, or 8bpp)\n");
        printf("  - Rotation and mirroring\n");
        printf("  - Color vs grayscale mode\n");
        printf("\nExamples:\n");
        printf("  epdraw photo.jpg                    # Any image format, auto-converted and displayed\n");
        printf("  epdraw photo.png 2510 2             # Custom VCOM and mode\n");
        printf("  epdraw image.bmp                    # Direct BMP display (no conversion needed)\n");
        return 1;
    }
    
    const char *input_path = argv[1];
    int vcom = (argc > 2) ? atoi(argv[2]) : 0;
    int mode = (argc > 3) ? atoi(argv[3]) : 2; // GC16_Mode
    
    // Validate mode
    if (mode < 0 || mode > 6) {
        fprintf(stderr, "Error: Invalid mode %d. Use 0-6.\n", mode);
        return 2;
    }
    
    char bmp_path[MAX_PATH];
    int need_conversion = 0;
    
    // Check if input is already a BMP file
    if (is_bmp_file(input_path)) {
        strncpy(bmp_path, input_path, sizeof(bmp_path) - 1);
        bmp_path[sizeof(bmp_path) - 1] = '\0';
        printf("Using existing BMP file: %s\n", bmp_path);
    } else {
        // Need to convert the image
        need_conversion = 1;
        
        // Check if ImageMagick is available
        if (!check_imagemagick()) {
            fprintf(stderr, "Error: ImageMagick not found. Please install ImageMagick:\n");
            fprintf(stderr, "  Ubuntu/Debian: sudo apt-get install imagemagick\n");
            fprintf(stderr, "  macOS: brew install imagemagick\n");
            fprintf(stderr, "  Or provide a BMP file directly.\n");
            return 2;
        }
        
        // Create output BMP path
        char *input_copy = strdup(input_path);
        char *dir = dirname(input_copy);
        char *base_copy = strdup(input_path);
        char *base = basename(base_copy);
        
        // Remove extension from base name
        char *dot = strrchr(base, '.');
        if (dot) *dot = '\0';
        
        snprintf(bmp_path, sizeof(bmp_path), "%s/%s.bmp", dir, base);
        free(input_copy);
        free(base_copy);
        
        // Determine rotation and colors based on mode
        int rotation = -90; // Default e-Paper rotation
        int colors = 16;    // Default grayscale
        
        if (mode == 3) {
            colors = 256; // Color mode
        }
        // For modes 1, 2, 6 (horizontal mirroring), we need to flip the image
        // since the display will mirror it horizontally
        if (mode == 1 || mode == 2 || mode == 6) {
            rotation = 90; // Flip horizontally by rotating 90 degrees
        }
        
        printf("Converting %s to %s (rotation: %d°, colors: %d)...\n", input_path, bmp_path, rotation, colors);
        
        // Convert the image
        if (convert_image_to_bmp(input_path, bmp_path, rotation, colors) != 0) {
            fprintf(stderr, "Error: Failed to convert image\n");
            return 2;
        }
    }
    
    // Check if the BMP file exists before proceeding
    FILE *fp = fopen(bmp_path, "rb");
    if (!fp) {
        fprintf(stderr, "Error: BMP file '%s' not found or not readable.\n", bmp_path);
        return 2;
    }
    fclose(fp);
    
    printf("Displaying image on e-Paper...\n");
    int result = EPD_IT8951_DisplayBMP(bmp_path, vcom, mode);
    if (result == 0) {
        printf("Image displayed successfully!\n");
        
        // Clean up temporary BMP file if we created it
        if (need_conversion) {
            printf("Cleaning up temporary file: %s\n", bmp_path);
            unlink(bmp_path);
        }
    } else {
        printf("Failed to display image (error %d)\n", result);
    }
    
    return result;
} 