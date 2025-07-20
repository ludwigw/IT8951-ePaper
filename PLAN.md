# Modernization and Portability Plan for IT8951-ePaper

## Status (December 2024)

- **Directory structure:** ✅ Modernized; all code in `src/`, headers in `include/`, examples in `examples/`, tests in `tests/`.
- **Documentation:** ✅ Doxygen and user docs complete and up to date.
- **Testing:** ✅ Automated tests for all modules, including hardware abstraction, fonts, GUI, and e-Paper driver. Mocks for hardware present.
- **Build system:** ✅ Fully modern Makefile at project root.
  - ✅ Builds static library (`libit8951epd.a`) from all sources in `src/`.
  - ✅ Builds all example binaries from `examples/`.
  - ✅ Supports platform selection (`PLATFORM=BCM|LGPIO|GPIOD`).
  - ✅ All object and dependency files are uniquely named and mapped, regardless of source directory.
  - ✅ `make install` copies headers and library to `/usr/local`.
  - ✅ Documentation and clean targets included.
  - ✅ **CI/CD:** GitHub Actions workflow with matrix builds for all platforms.
  - ✅ **Tested:** All build logic works as intended. CI passes for all platforms.

## System Dependencies

- **BCM platform:** Requires `bcm2835` development headers (`libbcm2835-dev` on Raspberry Pi OS).
- **LGPIO platform:** Requires `lgpio` development headers.
- **GPIOD platform:** Requires `gpiod` development headers.

## Recent Progress (December 2024)

- ✅ **CI/CD Pipeline:** GitHub Actions workflow with matrix builds for BCM, LGPIO, and GPIOD platforms.
- ✅ **Platform Compatibility:** All three platforms now build and test successfully in CI.
- ✅ **Build System Robustness:** Fixed duplicate object file warnings and improved platform separation.
- ✅ **Code Quality:** Resolved all compiler warnings and platform-specific header conflicts.
- ✅ **High-Level API:** Added `EPD_IT8951_DisplayBMP` function for one-call BMP image display.
- ✅ **CLI Tool:** Enhanced `epdraw` command-line interface as the main entry point.
- ✅ **Docker Support:** Added Dockerfile for Linux-based builds and testing.
- ✅ **Documentation:** Updated Quick Start guide to recommend CLI tool usage.

All legacy files and the old `Raspberry/` tree have been removed from the repository.
All new/renamed files, tests, and documentation are tracked and committed.
The build system is robust, with unique object mapping and explicit rules.
The test suite and all examples are included and tracked.
The repository is clean and ready for release or further development.

---

# Roadmap: Tidying Up and Modernizing the IT8951-ePaper Library

> **Note:** At every step, keep hardware abstraction clean and well-documented to make future ports (e.g., ESP32, STM32) easy. See the end of this file for a summary table of best practices.

## Goals
- Make the library easier to use for new and experienced users
- Improve code readability and maintainability
- Simplify the process of displaying images on e-Paper devices
- Provide clear, modern documentation and examples
- **Future-proof for new platforms (ESP32, STM32, etc.)**

---

## 1. Code Organization
- [x] Restructure the directory layout for clarity (e.g., `src/`, `examples/`, `include/`, `docs/`)
  - **Portability Tip:** Keep all hardware-specific code in a dedicated subdirectory (e.g., `src/platform/`).
- [x] Separate core driver code from example/demo code
  - **Portability Tip:** Ensure no hardware-specific code leaks into display logic, GUI, or buffer management.
- [x] Use consistent naming conventions for files and functions
  - **Portability Tip:** Use generic names for abstraction functions (e.g., `DEV_Digital_Write`).

## 2. Documentation
- [x] Create a `/docs` directory with overview, quickstart, architecture, and troubleshooting guides
- [x] Add API reference documentation (doxygen or markdown)
  - **Doxygen HTML API docs are generated in docs/api/html and linked from the README.**
  - **All major modules and functions are documented with Doxygen comments.**
- [ ] Add more usage examples (e.g., drawing primitives, partial updates, text rendering) -- See what was removed during commit 8368cb9b0e271235a1a6973eba4bbe97ff60a462 for ideas.
- [x] Provide a clear "Getting Started" guide in the root README
  - **Portability Tip:** Add a section in the docs (e.g., in `architecture.md` or a new `porting.md`) explaining how to add support for new platforms by implementing the hardware abstraction layer.

## 3. Build System
- [x] Simplify the Makefile; provide clear targets for building library, examples, and cleaning
  - **Portability Tip:** Structure the build system so platform selection is easy (e.g., `make PLATFORM=ESP32`). Avoid hardcoding platform macros.
- [x] Add dependency checks and helpful error messages
- [ ] Consider providing a CMake configuration for cross-platform builds
  - **Portability Tip:** CMake makes it easier to support multiple platforms and toolchains.

## 4. API Simplification
- [x] Abstract away low-level details for common use cases (e.g., `display_image(path, vcom, mode)`)
  - **Portability Tip:** Keep the public API platform-agnostic; all platform-specific details should be hidden in the hardware abstraction layer.
- [x] Provide a simple, high-level API for image display
- [x] Document all public functions with clear usage and parameter descriptions

## 5. User Experience Improvements
- [x] Add command-line help and argument validation to example programs
- [ ] Provide pre-built binaries for common platforms (optional)
  - **Portability Tip:** For MCUs, provide example firmware projects instead of binaries.
- [x] Add scripts/utilities for image conversion and testing
- [x] Add error handling and user-friendly messages throughout

## 6. Testing & CI
- [x] Add basic unit tests for core logic (where possible)
  - **Fonts module and text rendering are now fully covered by automated tests.**
  - **Platform abstraction, buffer logic, and e-Paper driver are tested.**
  - **Portability Tip:** Structure tests so platform-agnostic logic can be tested on any platform; hardware-specific tests should be isolated.
- [x] Set up continuous integration (CI) for build/test on supported platforms
  - **Portability Tip:** Consider using GitHub Actions or similar to test builds for all platforms.

## 7. Community & Contribution
- [x] Add a `CONTRIBUTING.md` guide
  - **Portability Tip:** Include a section: "How to add support for a new platform (e.g., ESP32)" with a checklist and pointers to the abstraction layer.
- [ ] Add issue templates and PR guidelines

---

## Remaining Priorities
1. **Add more usage examples** - Drawing primitives, partial updates, text rendering
2. **Consider CMake configuration** - For cross-platform builds
3. **Add issue templates and PR guidelines** - For community contributions
4. **Pre-built binaries** - For common platforms (optional)

**Testing coverage and API documentation for all major modules (including Fonts) is complete. The codebase is now fully documented, tested, and ready for further development or release. This is a good milestone for a commit.**

---

## Portability Best Practices: What to Do/What to Avoid

| Do                                                      | Avoid                                  |
|---------------------------------------------------------|----------------------------------------|
| Keep hardware access in a single abstraction layer      | Mixing platform code into display logic|
| Use generic function names and signatures               | Hardcoding Pi/BCM specifics everywhere |
| Document the abstraction interface                      | Spreading GPIO/SPI code across modules |
| Structure build system for easy platform selection      | Relying on manual Makefile edits only  |
| Add porting notes to docs and CONTRIBUTING.md           | Assuming only one platform forever     |

---

**This plan is a living document. Update as improvements are made or new needs are discovered.** 