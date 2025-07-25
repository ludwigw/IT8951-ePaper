#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>
#include "../include/EPD_IT8951.h"
#include "../include/Debug.h"
#include "../include/DEV_Config.h"

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
 * @param mirror 1 for horizontal mirroring, 0 for no mirroring
 * @return 0 on success, -1 on failure
 */
int convert_image_to_bmp(const char *input_path, const char *output_path, int rotation, int colors, int mirror) {
    char cmd[MAX_CMD];
    const char* magick_cmd = get_imagemagick_cmd();
    
    // Build ImageMagick command with appropriate settings
    if (colors == 16) {
        // Grayscale conversion with dithering, force 4bpp BMP output
        if (mirror) {
            snprintf(cmd, sizeof(cmd), 
                "%s \"%s\" -colors %d -dither FloydSteinberg -colorspace Gray -type Palette -define bmp:format=bmp3 -depth 4 -rotate %d -flop \"%s\"",
                magick_cmd, input_path, colors, rotation, output_path);
        } else {
            snprintf(cmd, sizeof(cmd), 
                "%s \"%s\" -colors %d -dither FloydSteinberg -colorspace Gray -type Palette -define bmp:format=bmp3 -depth 4 -rotate %d \"%s\"",
                magick_cmd, input_path, colors, rotation, output_path);
        }
    } else {
        // Color conversion (for color e-Paper)
        if (mirror) {
            snprintf(cmd, sizeof(cmd), 
                "%s \"%s\" -colors %d -rotate %d -flop \"%s\"",
                magick_cmd, input_path, colors, rotation, output_path);
        } else {
            snprintf(cmd, sizeof(cmd), 
                "%s \"%s\" -colors %d -rotate %d \"%s\"",
                magick_cmd, input_path, colors, rotation, output_path);
        }
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
    // Initialize logging system
    const char* log_level_str = getenv("LOG_LEVEL");
    if (log_level_str) {
        log_level_t log_level;
        if (strcmp(log_level_str, "ERROR") == 0) log_level = LOG_LEVEL_ERROR;
        else if (strcmp(log_level_str, "WARN") == 0) log_level = LOG_LEVEL_WARN;
        else if (strcmp(log_level_str, "INFO") == 0) log_level = LOG_LEVEL_INFO;
        else if (strcmp(log_level_str, "DEBUG") == 0) log_level = LOG_LEVEL_DEBUG;
        else if (strcmp(log_level_str, "TRACE") == 0) log_level = LOG_LEVEL_TRACE;
        else log_level = LOG_LEVEL_WARN;
        log_init(log_level);
    }

    // --- Parse --stay-awake flag ---
    int stay_awake = 0;
    int arg_offset = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--stay-awake") == 0) {
            stay_awake = 1;
            // Remove the flag from argv for positional parsing
            for (int j = i; j < argc - 1; ++j) {
                argv[j] = argv[j + 1];
            }
            argc--;
            break;
        }
    }

    if (argc < 2 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        printf("Usage: epdraw [--stay-awake] <image_path> [vcom] [mode]\n");
        printf("  [--stay-awake]: Do not put the display to sleep after update (default: sleep after update)\n");
        printf("  <image_path>: Path to image file (any format: PNG, JPG, BMP, etc. - will be auto-converted)\n");
        printf("  [vcom]: VCOM voltage (default: 0, use panel default)\n");
        printf("          Can be integer (2510) or float (-1.18V)\n");
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
        printf("  epdraw --stay-awake photo.png -1.18 2 # Custom VCOM (-1.18V), mode, and stay awake\n");
        printf("  epdraw image.bmp                    # Direct BMP display (no conversion needed)\n");
        return 1;
    }
    
    const char *input_path = argv[1];
    
    // Parse VCOM value - can be integer (like 2510) or float (like -1.18)
    int vcom = 0;
    if (argc > 2) {
        char *endptr;
        double vcom_float = strtod(argv[2], &endptr);
        if (*endptr == '\0') {
            // Valid number - convert to VCOM format
            // VCOM values are in millivolts × 1000, e.g., 2510 = -2.51V
            if (vcom_float < 0) {
                // Negative voltage: convert to positive integer format
                vcom = (int)(-vcom_float * 1000);
            } else {
                // Positive voltage: convert to integer format
                vcom = (int)(vcom_float * 1000);
            }
        } else {
            // Try as integer
            vcom = atoi(argv[2]);
        }
    } else {
        // Default to 0 to use panel default (don't change VCOM)
        vcom = 0;
    }
    
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
        int mirror = 0;
        if (mode == 3) {
            colors = 256; // Color mode
        }
        // For mode 2, enable horizontal mirroring
        if (mode == 2) {
            mirror = 1;
            rotation = 90; // Keep previous logic if needed
        } else if (mode == 1 || mode == 6) {
            // Optionally keep mirroring for these modes if desired
            mirror = 1;
            rotation = 90;
        }
        printf("Converting %s to %s (rotation: %d°, colors: %d, mirror: %d)...\n", input_path, bmp_path, rotation, colors, mirror);
        
        // Convert the image
        if (convert_image_to_bmp(input_path, bmp_path, rotation, colors, mirror) != 0) {
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
    
    printf("epdraw: Initializing hardware (GPIO, SPI)...\n");
    if (DEV_Module_Init() != 0) {
        fprintf(stderr, "epdraw: ERROR: Failed to initialize hardware\n");
        return 2;
    }
    // Print BUSY pin state after hardware init
    printf("BUSY pin state after init: %d\n", DEV_Digital_Read(EPD_BUSY_PIN));
    printf("epdraw: Hardware initialization completed\n");
    
    printf("epdraw: Displaying BMP: %s, VCOM: %d, mode: %d\n", bmp_path, vcom, mode);
    int result = EPD_IT8951_DisplayBMP(bmp_path, vcom, mode);
    if (result == 0) {
        printf("Image displayed successfully!\n");
        // E-paper displays need time to physically update.
        // If the program exits or powers down the panel too quickly after sending the image, the update may not complete.
        // The delay ensures the panel has time to finish the refresh before any shutdown or further commands.
        DEV_Delay_ms(5000);
        if (!stay_awake) {
            printf("Putting display to sleep...\n");
            EPD_IT8951_Sleep();
        } else {
            printf("Leaving display awake (per --stay-awake flag)\n");
        }
         // Clean up temporary BMP file if we created it
        if (need_conversion) {
            printf("Cleaning up temporary file: %s\n", bmp_path);
            unlink(bmp_path);
        }
    } else {
        fprintf(stderr, "epdraw: ERROR: Failed to display image (error code %d)\n", result);
        if (result == -10) fprintf(stderr, "epdraw: ERROR: Failed to initialize display or get panel info\n");
        else if (result == -11) fprintf(stderr, "epdraw: ERROR: Out of memory allocating display buffer\n");
        else if (result == -12) fprintf(stderr, "epdraw: ERROR: Invalid bit depth\n");
        else if (result == -1) fprintf(stderr, "epdraw: ERROR: BMP file not found or could not be opened\n");
        else if (result == -2) fprintf(stderr, "epdraw: ERROR: BMP file header read error\n");
        else if (result == -3) fprintf(stderr, "epdraw: ERROR: Not a BMP file\n");
        else if (result == -4) fprintf(stderr, "epdraw: ERROR: BMP info header read error\n");
        else if (result == -5) fprintf(stderr, "epdraw: ERROR: BMP palette read error or out of memory\n");
        // Add more as needed
    }
    
    printf("epdraw: Cleaning up hardware resources...\n");
    DEV_Module_Exit();
    printf("epdraw: Hardware cleanup completed\n");
    
    return result;
} 