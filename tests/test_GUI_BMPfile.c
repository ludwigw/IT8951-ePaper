#include <assert.h>
#include <stdio.h>
#include "../include/GUI_BMPfile.h"

void test_GUI_ReadBmp_missing_file() {
    int result = GUI_ReadBmp("nonexistent.bmp", 0, 0);
    assert(result < 0); // Should return error for missing file
}

int main() {
    test_GUI_ReadBmp_missing_file();
    printf("All GUI_BMPfile tests passed!\n");
    return 0;
} 