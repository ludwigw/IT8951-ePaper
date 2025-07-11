#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/GUI_BMPfile.h"

// Helper to create a file with given content
void write_file(const char* filename, const void* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    assert(f);
    fwrite(data, 1, size, f);
    fclose(f);
}

void test_invalid_bmp_header() {
    // Not a BMP: just some text
    const char* fname = "invalid_header.bmp";
    const char* text = "NOTABMP";
    write_file(fname, text, strlen(text));
    int result = GUI_ReadBmp(fname, 0, 0);
    assert(result < 0);
    remove(fname);
}

void test_truncated_bmp_file() {
    // Valid BMP header but truncated file
    const char* fname = "truncated.bmp";
    unsigned char bmp_header[14] = {0x42, 0x4D}; // 'BM' signature, rest zero
    write_file(fname, bmp_header, sizeof(bmp_header));
    int result = GUI_ReadBmp(fname, 0, 0);
    assert(result < 0);
    remove(fname);
}

void test_null_path() {
    int result = GUI_ReadBmp(NULL, 0, 0);
    assert(result < 0);
}

int main() {
    test_invalid_bmp_header();
    test_truncated_bmp_file();
    test_null_path();
    printf("All GUI_BMPfile error handling tests passed!\n");
    return 0;
} 