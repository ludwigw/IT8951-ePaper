#include <assert.h>
#include <stdio.h>
#include "../include/EPD_IT8951.h"

void test_null_buffer() {
    // Should not crash or dereference null
    EPD_IT8951_4bp_Refresh(NULL, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_2bp_Refresh(NULL, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_1bp_Refresh(NULL, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_8bp_Refresh(NULL, 0, 0, 0, 0, 0, 0);
}

void test_zero_size() {
    unsigned char buf[1] = {0};
    // Should not crash or write out of bounds
    EPD_IT8951_4bp_Refresh(buf, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_2bp_Refresh(buf, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_1bp_Refresh(buf, 0, 0, 0, 0, 0, 0, 0);
    EPD_IT8951_8bp_Refresh(buf, 0, 0, 0, 0, 0, 0);
}

int main() {
    test_null_buffer();
    test_zero_size();
    printf("EPD_IT8951 error handling tests passed!\n");
    return 0;
} 