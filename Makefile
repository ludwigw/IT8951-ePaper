# Enhanced Makefile for IT8951-ePaper library and examples (robust object mapping, fixed foreach)

# Platform selection: one of BCM, LGPIO, GPIOD (default: BCM)
PLATFORM ?= BCM

# Directories
SRC_DIRS = src/GUI src/e-Paper src/Fonts src/Config
PLATFORM_DIR = src/platform
EXAMPLES_DIR = examples
BIN_DIR = bin
INCLUDE_DIR = include

# Compiler and flags
CC = gcc
AR = ar
CFLAGS = -I$(INCLUDE_DIR) $(foreach d,$(SRC_DIRS),-I$d) -I$(PLATFORM_DIR) -Wall -Wextra -g
LDFLAGS =

# Platform-specific sources
PLATFORM_SRC_BCM = src/platform/DEV_Config_BCM.c
PLATFORM_SRC_LGPIO = src/platform/DEV_Config_LGPIO.c
PLATFORM_SRC_GPIOD = src/platform/DEV_Config_GPIOD.c

ifeq ($(PLATFORM),BCM)
PLATFORM_SRC = $(PLATFORM_SRC_BCM)
PLATFORM_DEFS = -DBCM=1 -DLGPIO=0 -DGPIOD=0
PLATFORM_LIBS = -lbcm2835
else ifeq ($(PLATFORM),LGPIO)
PLATFORM_SRC = $(PLATFORM_SRC_LGPIO)
PLATFORM_DEFS = -DBCM=0 -DLGPIO=1 -DGPIOD=0
PLATFORM_LIBS = -llgpio
else ifeq ($(PLATFORM),GPIOD)
PLATFORM_SRC = $(PLATFORM_SRC_GPIOD)
PLATFORM_DEFS = -DBCM=0 -DLGPIO=0 -DGPIOD=1
PLATFORM_LIBS = -lgpiod
else
$(error Unknown PLATFORM: $(PLATFORM))
endif

# Helper to flatten paths for unique object names
# This ensures that each object file name is unique, even if source files have the same name in different directories.
# It is critical that only one platform-specific DEV_Config source is included in SRC at a time, or duplicate object file rules will result.
define FLATTEN
$(subst /,_,$(subst src/,,$(subst examples/,,$(1:.c=))))
endef

# Source and object files
SRC = $(foreach d,$(SRC_DIRS),$(wildcard $d/*.c)) $(PLATFORM_SRC)
# Exclude the generic DEV_Config.c file, we only want platform-specific ones
SRC := $(filter-out src/Config/DEV_Config.c,$(SRC))
# Exclude platform-specific files from src/Config and src/platform based on platform
ifeq ($(PLATFORM),BCM)
SRC := $(filter-out src/Config/RPI_gpiod.c src/Config/dev_hardware_SPI.c,$(SRC))
SRC := $(filter-out src/platform/DEV_Config_LGPIO.c src/platform/DEV_Config_GPIOD.c,$(SRC))
else ifeq ($(PLATFORM),LGPIO)
SRC := $(filter-out src/Config/RPI_gpiod.c src/Config/dev_hardware_SPI.c,$(SRC))
SRC := $(filter-out src/platform/DEV_Config_BCM.c src/platform/DEV_Config_GPIOD.c,$(SRC))
else ifeq ($(PLATFORM),GPIOD)
SRC := $(filter-out src/platform/DEV_Config_BCM.c src/platform/DEV_Config_LGPIO.c,$(SRC))
endif
OBJ = $(foreach f,$(SRC),$(BIN_DIR)/$(call FLATTEN,$(f)).o)
OBJ_SRC = $(foreach f,$(SRC),$(BIN_DIR)/$(call FLATTEN,$(f)).o:$(f))
DEPS = $(OBJ:.o=.d)

# Example sources and binaries
EXAMPLE_SRC = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS = $(foreach f,$(EXAMPLE_SRC),$(call FLATTEN,$(f)))
EXAMPLE_OBJ = $(foreach f,$(EXAMPLE_SRC),$(BIN_DIR)/$(call FLATTEN,$(f)).o)
EXAMPLE_OBJ_SRC = $(foreach f,$(EXAMPLE_SRC),$(BIN_DIR)/$(call FLATTEN,$(f)).o:$(f))

LIB_NAME = libit8951epd.a

.PHONY: all clean apidocs docs-clean install test $(EXAMPLE_BINS)

all: $(BIN_DIR) $(LIB_NAME) $(EXAMPLE_BINS)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Build static library
$(LIB_NAME): $(OBJ)
	$(AR) rcs $@ $^

# Build each example as a separate binary
$(EXAMPLE_BINS): %: $(BIN_DIR)/%.o $(LIB_NAME)
	$(CC) $(CFLAGS) $(PLATFORM_DEFS) -o $@ $< -L. -lit8951epd $(PLATFORM_LIBS) -lm

# Define a template for object build rules
define OBJ_template
$1: $2 | $(BIN_DIR)
	$(CC) $(CFLAGS) $(PLATFORM_DEFS) -MMD -MP -c $2 -o $1
endef

# Generate explicit object build rules for all sources
$(foreach rule,$(OBJ_SRC),$(eval $(call OBJ_template,$(word 1,$(subst :, ,$(rule))),$(word 2,$(subst :, ,$(rule))))))
$(foreach rule,$(EXAMPLE_OBJ_SRC),$(eval $(call OBJ_template,$(word 1,$(subst :, ,$(rule))),$(word 2,$(subst :, ,$(rule))))))

-include $(DEPS)

clean:
	rm -rf $(BIN_DIR) *.a $(EXAMPLE_BINS)

# Install headers, static library, and CLI tool
install: $(LIB_NAME) bin/epdraw
	install -d /usr/local/include/it8951epd
	install -m 644 $(INCLUDE_DIR)/*.h /usr/local/include/it8951epd/
	install -m 644 $(LIB_NAME) /usr/local/lib/
	install -d /usr/local/bin
	install -m 755 bin/epdraw /usr/local/bin/

# Documentation targets (retained from previous Makefile)
apidocs:
	doxygen Doxyfile

docs-clean:
	rm -rf docs/api 

# CLI tool for end users
bin/epdraw: src/epdraw.c $(LIB_NAME)
	$(CC) $(CFLAGS) $(PLATFORM_DEFS) -o $@ $< -L. -lit8951epd $(PLATFORM_LIBS) -lm

# Run tests
test:
	$(MAKE) -C tests 

# Check for duplicate object file names (warn if any duplicates are found)
# This is a Makefile hack: it prints a warning if any object file names are duplicated in OBJ
check-duplicates:
	@dups=`echo $(OBJ) | tr ' ' '\n' | sort | uniq -d`; \
	if [ "$$dups" ]; then \
	  echo "WARNING: Duplicate object file names detected:"; \
	  echo "$$dups"; \
	fi 