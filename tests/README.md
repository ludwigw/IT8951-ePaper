# Tests Directory

This directory contains unit and integration tests for the IT8951-ePaper library. The goal is to ensure that refactoring and new features do not break existing functionality, especially for core image processing and display logic.

## How to Run the Tests

1. Make sure the library is built (or at least the headers are available).
2. Compile the test files manually, e.g.:
   ```sh
   gcc -I../Raspberry/lib/GUI -o test_GUI_Paint test_GUI_Paint.c
   ./test_GUI_Paint
   gcc -I../Raspberry/lib/GUI -o test_GUI_BMPfile test_GUI_BMPfile.c
   ./test_GUI_BMPfile
   ```
   (You may need to adjust include paths depending on your setup.)

3. All tests should print a success message if they pass.

## What is Covered
- **test_GUI_Paint.c**: Tests image buffer creation, pixel setting, and rotation logic.
- **test_GUI_BMPfile.c**: Tests error handling for missing BMP files in the loader.

## What to Add Next
- More tests for image drawing primitives (lines, rectangles, circles, text)
- Tests for buffer alignment and bit depth conversions
- Integration tests for the full image-to-framebuffer pipeline (mocked if needed)
- Hardware-in-the-loop tests for end-to-end validation (optional)

---

**If you add or refactor code, please add or update tests to ensure continued reliability!** 