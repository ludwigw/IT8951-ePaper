# Simple Makefile for running unit tests in the IT8951-ePaper library

CC = gcc
CFLAGS = -I../src/GUI -I../src/e-Paper -I../src/Fonts -I../src/Config -I../include -Wall -Wextra -g

# Core tests that work with any platform
CORE_TESTS = test_GUI_Paint test_GUI_BMPfile test_GUI_Paint_draw test_GUI_BMPfile_errors test_GUI_BMPfile_valid test_GUI_Paint_alignment test_GUI_Paint_edgecases test_EPD_IT8951_buffer test_EPD_IT8951_structs test_EPD_IT8951_modes test_EPD_IT8951_error test_GUI_Fonts test_EPD_IT8951_DisplayBMP test_cli

# Platform-specific tests (only build if dependencies are available)
PLATFORM_TESTS = test_DEV_Config_platform_bcm

# Default tests (core tests only)
TESTS = $(CORE_TESTS)

# All tests including platform tests (if dependencies are available)
ALL_TESTS = $(CORE_TESTS) $(PLATFORM_TESTS)

all: $(TESTS)

# Build each test

test_GUI_Paint: test_GUI_Paint.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_BMPfile: test_GUI_BMPfile.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_Paint_draw: test_GUI_Paint_draw.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_BMPfile_errors: test_GUI_BMPfile_errors.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_BMPfile_valid: test_GUI_BMPfile_valid.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_Paint_alignment: test_GUI_Paint_alignment.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_GUI_Paint_edgecases: test_GUI_Paint_edgecases.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_EPD_IT8951_buffer: test_EPD_IT8951_buffer.c ../src/e-Paper/EPD_IT8951.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_EPD_IT8951_structs: test_EPD_IT8951_structs.c ../src/e-Paper/EPD_IT8951.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_EPD_IT8951_modes: test_EPD_IT8951_modes.c ../src/e-Paper/EPD_IT8951.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_EPD_IT8951_error: test_EPD_IT8951_error.c ../src/e-Paper/EPD_IT8951.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_DEV_Config_platform: test_DEV_Config_platform.c
	$(CC) -I. $(CFLAGS) -o $@ $< -lm

test_DEV_Config_platform_bcm: test_DEV_Config_platform_bcm.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

# Platform-specific tests (only build if dependencies are available)
test_DEV_Config_platform_lgpio: test_DEV_Config_platform_lgpio.c
	@echo "Skipping LGPIO test (lgpio.h not available)"
	@echo "To enable: install liblgpio-dev and run 'make test_DEV_Config_platform_lgpio'"

test_DEV_Config_platform_gpiod: test_DEV_Config_platform_gpiod.c
	@echo "Skipping GPIOD test (gpiod.h not available)"
	@echo "To enable: install libgpiod-dev and run 'make test_DEV_Config_platform_gpiod'"

test_GUI_Fonts: test_GUI_Fonts.c ../src/GUI/GUI_Paint.c ../src/Fonts/font8.c ../src/Fonts/font12.c ../src/Fonts/font16.c ../src/Fonts/font20.c ../src/Fonts/font24.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_EPD_IT8951_DisplayBMP: test_EPD_IT8951_DisplayBMP.c ../src/e-Paper/EPD_IT8951.c ../src/GUI/GUI_BMPfile.c ../src/GUI/GUI_Paint.c mock_DEV_Config.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_cli: test_cli.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

test_config_logic: test_config_logic.c ../src/e-Paper/EPD_IT8951.c
	$(CC) -I. $(CFLAGS) $^ -o $@ -lm

run: all
	@for t in $(TESTS); do \
		echo "Running $$t..."; \
		./$$t; \
	done

clean:
	rm -f $(TESTS) 