#include <assert.h>
#include <stdio.h>
#include "../include/GUI_BMPfile.h"

void test_valid_bmp() {
    int result = GUI_ReadBmp("../Raspberry/pic/render.bmp", 0, 0);
    assert(result == 0);
}

int main() {
    test_valid_bmp();
    printf("Valid BMP parsing test passed!\n");
    return 0;
} 