# Testing Plan

> **Note:** In CI (GitHub Actions), the test matrix builds and runs all platform-agnostic tests for each supported platform (BCM, LGPIO, GPIOD). Platform-specific tests (which require real hardware) are skipped or print a message in CI. To fully test hardware abstraction, run platform-specific tests manually on a real device.

This document outlines the testing strategy for the IT8951-ePaper library.

## Current Test Coverage

### Unit Tests
The library includes comprehensive unit tests in the `tests/` directory:

- **GUI Module Tests:**
  - `test_GUI_Paint.c` - Basic drawing primitives
  - `test_GUI_Paint_draw.c` - Drawing functions (lines, rectangles, circles)
  - `test_GUI_Paint_alignment.c` - Memory alignment tests
  - `test_GUI_Paint_edgecases.c` - Edge case handling
  - `test_GUI_BMPfile.c` - BMP file loading
  - `test_GUI_BMPfile_errors.c` - Error handling for invalid BMPs
  - `test_GUI_BMPfile_valid.c` - Valid BMP file processing
  - `test_GUI_Fonts.c` - Font rendering and text display

- **e-Paper Driver Tests:**
  - `test_EPD_IT8951_buffer.c` - Buffer management
  - `test_EPD_IT8951_structs.c` - Data structure handling
  - `test_EPD_IT8951_modes.c` - Display mode configuration
  - `test_EPD_IT8951_error.c` - Error handling
  - `test_EPD_IT8951_DisplayBMP.c` - High-level API testing

- **Platform Tests:**
  - `test_DEV_Config_platform_bcm.c` - BCM platform abstraction
  - `test_DEV_Config_platform_lgpio.c` - LGPIO platform abstraction
  - `test_DEV_Config_platform_gpiod.c` - GPIOD platform abstraction

- **CLI Tests:**
  - `test_cli.c` - Command-line interface testing

### Mock Infrastructure
- `mock_DEV_Config.c` - Hardware abstraction layer mocks
- `bcm2835.h` - Mock headers for testing

## What is and isn't tested in CI

- **What is tested:**
  - All platform-agnostic logic (GUI, BMP parsing, buffer management, CLI, high-level API)
  - Build and test for all supported platforms (BCM, LGPIO, GPIOD) using the correct dev libraries
  - Ensures the codebase compiles and links for each platform

- **What is NOT tested:**
  - Actual hardware access (GPIO, SPI) for any platform
  - Platform-specific tests that require real hardware (these are skipped or print a message)
  - Full integration with a real e-Paper display (manual testing required)

## Running Tests

### Local Testing
```sh
# Run all tests
make test

# Run specific test
make -C tests test_cli

# Run tests with verbose output
make -C tests run
```

### Test Dependencies
Tests use mock implementations to avoid requiring actual hardware:
- Hardware abstraction layer is mocked
- No actual GPIO/SPI operations are performed
- Tests focus on logic and data handling

## CI/CD Testing Strategy

### Recommended CI/CD Setup

#### 1. GitHub Actions
The workflow uses a matrix to build and test for all supported platforms (BCM, LGPIO, GPIOD). Only platform-agnostic tests are run in CI.

#### 2. Docker Testing
You can also use Docker locally to test the build and run the platform-agnostic tests.

### Multi-Platform Testing

#### Platform-Specific Tests
- **BCM Platform:** Test with bcm2835 library
- **LGPIO Platform:** Test with lgpio library (Pi 5)
- **GPIOD Platform:** Test with gpiod library (Jetson, etc.)

#### Build Matrix
```yaml
strategy:
  matrix:
    platform: [BCM, LGPIO, GPIOD]
    include:
      - platform: BCM
        deps: libbcm2835-dev
      - platform: LGPIO
        deps: liblgpio-dev
      - platform: GPIOD
        deps: libgpiod-dev
```

## Integration Testing

### Hardware Testing (Manual)
For complete validation, manual testing on actual hardware is recommended:

1. **Raspberry Pi Setup:**
   - Pi Zero W2, Pi 3, Pi 4, Pi 5
   - Different OS versions (Raspberry Pi OS, Ubuntu)
   - Various e-Paper display models

2. **Test Scenarios:**
   - Basic image display
   - Different display modes
   - Error handling (invalid files, hardware issues)
   - Performance testing with large images

### Automated Hardware Testing
Consider using:
- **QEMU:** Emulate Raspberry Pi for basic testing
- **Hardware CI:** Dedicated Pi hardware for automated testing
- **Docker with GPIO simulation:** Mock hardware interfaces

## Test Data

### Test Images
- Small test BMP files for unit tests
- Various image formats and sizes
- Invalid/corrupted files for error testing

### Test Assets
Located in `tests/assets/`:
- Valid BMP test images
- Invalid file formats
- Edge case images (very small, very large, etc.)

## Coverage Goals

### Code Coverage Targets
- **Unit Tests:** >90% line coverage
- **Integration Tests:** All major use cases
- **Error Handling:** All error paths tested
- **Platform Support:** All platforms tested (build and logic only in CI)

### Performance Testing
- Memory usage profiling
- Image processing speed
- Display refresh timing

## Future Testing Enhancements

### Planned Improvements
1. **Property-based testing** for image processing
2. **Fuzzing** for BMP file parsing
3. **Memory leak detection** with Valgrind
4. **Static analysis** with cppcheck/clang-tidy
5. **Performance benchmarking** suite

### Continuous Improvement
- Regular test coverage analysis
- Automated test result reporting
- Test failure investigation and fixes
- Documentation updates based on test findings

## Troubleshooting Tests

### Common Issues
1. **Missing dependencies:** Install required libraries
2. **Platform-specific failures:** Check platform configuration
3. **Mock failures:** Verify mock implementations
4. **Build errors:** Check compiler flags and includes

### Debug Commands
```sh
# Verbose test output
make -C tests run V=1

# Debug specific test
gdb tests/test_cli

# Check test dependencies
ldd tests/test_cli
```

---

For more information, see the test source files in the `tests/` directory and the test README. 