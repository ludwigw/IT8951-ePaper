#include <stdio.h>
#include <assert.h>
#include "../include/EPD_IT8951.h"

void test_config_mode(UWORD mode, UBYTE expected_bpp, UWORD expected_rotate, UBYTE expected_mirror, UBYTE expected_is_color) {
    EPD_Config cfg = EPD_IT8951_ComputeConfig(mode);
    assert(cfg.bits_per_pixel == expected_bpp);
    assert(cfg.rotate == expected_rotate);
    assert(cfg.mirror == expected_mirror);
    assert(cfg.is_color == expected_is_color);
    printf("Mode %u: PASS\n", mode);
}

int main() {
    printf("Testing EPD_IT8951_ComputeConfig...\n");
    // mode, bpp, rotate, mirror, is_color
    test_config_mode(0, 1, 0, 0, 0); // INIT
    test_config_mode(1, 4, 0, 1, 0); // 10.3"
    test_config_mode(2, 4, 0, 1, 0); // 5.2"
    test_config_mode(3, 8, 0, 0, 1); // color
    test_config_mode(4, 1, 0, 0, 0); // A2
    test_config_mode(6, 1, 0, 1, 0); // A2 mirrored
    printf("All config tests passed!\n");
    return 0;
} 