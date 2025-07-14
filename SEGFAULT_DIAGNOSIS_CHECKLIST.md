# Segmentation Fault Diagnosis Checklist

This checklist is designed to systematically diagnose and prevent segmentation faults in the e-Paper display codebase. Check off each item as you review and verify the code.

---

## 1. Memory Allocation ✅ COMPLETED
- [x] All `malloc`/`calloc`/`realloc` calls are checked for `NULL` before use.
- [x] All allocated memory is freed exactly once (no leaks, no double-free).
- [x] No use of memory after it has been freed.
- [x] Buffer sizes are always calculated correctly for the intended use.

**Notes:** All memory allocations are properly checked. Minor potential leak in `GUI_ReadBmp` for 1bpp/4bpp cases (old buffer not freed before new allocation), but this is not the cause of the current segfault.

---

## 2. File Operations
- [ ] Every `fopen`/`fread`/`fseek`/`fclose` is checked for errors.
- [ ] File pointers are not used if the open/read failed.
- [ ] All files are closed after use, even on error paths.

---

## 3. Buffer Indexing and Access
- [ ] All buffer accesses (arrays, pointers) are within allocated bounds.
- [ ] No off-by-one errors in loops (especially with width/height and bit depth).
- [ ] All image and display buffer sizes match the expected dimensions and bit depth.
- [ ] No buffer overflows or underflows in image processing functions.

---

## 4. Function Return Values
- [ ] All functions that can fail (return negative/error codes) are checked by the caller.
- [ ] Error codes are propagated and logged with meaningful messages.
- [ ] No function is assumed to succeed without checking its return value.

---

## 5. Global State and Concurrency
- [ ] Global variables (e.g., `Paint`, `bmp_src_buf`, `bmp_dst_buf`) are not used after being freed.
- [ ] No concurrent access to global state that could cause race conditions.
- [ ] State is reset or re-initialized as needed between runs.

---

## 6. Static Analysis and Tooling
- [ ] Compile with `-Wall -Wextra -Werror` and resolve all warnings.
- [ ] Run `cppcheck` or `clang-tidy` and address reported issues.
- [ ] Use `valgrind` to check for invalid memory accesses and leaks during runtime.
- [ ] Document any tool findings and resolutions in this checklist.

---

## Progress Notes
- **ROOT CAUSE FOUND**: The segfault was caused by missing hardware initialization. `DEV_Module_Init()` was not being called before `EPD_IT8951_DisplayBMP()`, causing the program to try to access uninitialized GPIO/SPI hardware.
- **FIX APPLIED**: Added `DEV_Module_Init()` call before display operations and `DEV_Module_Exit()` call for cleanup.
- **LOGGING ADDED**: Comprehensive logging throughout the initialization path to track hardware setup, SPI configuration, GPIO pin setup, and display initialization.
- **FORMAT WARNING FIXED**: Changed `%llu` to `%u` for `image_size` variable to match `uint32_t` type. 