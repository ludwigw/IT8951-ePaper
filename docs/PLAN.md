# Plan: Tidying Up and Modernizing the IT8951-ePaper Library

> **Note:** At every step, keep hardware abstraction clean and well-documented to make future ports (e.g., ESP32, STM32) easy. See the end of this file for a summary table of best practices.

## Goals
- Make the library easier to use for new and experienced users
- Improve code readability and maintainability
- Simplify the process of displaying images on e-Paper devices
- Provide clear, modern documentation and examples
- **Future-proof for new platforms (ESP32, STM32, etc.)**

---

## 1. Code Organization
- [ ] Restructure the directory layout for clarity (e.g., `src/`, `examples/`, `include/`, `docs/`)
  - **Portability Tip:** Keep all hardware-specific code in a dedicated subdirectory (e.g., `src/platform/`).
- [ ] Separate core driver code from example/demo code
  - **Portability Tip:** Ensure no hardware-specific code leaks into display logic, GUI, or buffer management.
- [ ] Use consistent naming conventions for files and functions
  - **Portability Tip:** Use generic names for abstraction functions (e.g., `DEV_Digital_Write`).

## 2. Documentation
- [x] Create a `/docs` directory with overview, quickstart, architecture, and troubleshooting guides
- [x] Add API reference documentation (doxygen or markdown)
  - **Doxygen HTML API docs are generated in docs/api/html and linked from the README.**
  - **All major modules and functions are documented with Doxygen comments.**
- [ ] Add more usage examples (e.g., drawing primitives, partial updates, text rendering) -- See what was removed during commit 
8368cb9b0e271235a1a6973eba4bbe97ff60a462 for ideas.
- [ ] Provide a clear "Getting Started" guide in the root README
  - **Portability Tip:** Add a section in the docs (e.g., in `architecture.md` or a new `porting.md`) explaining how to add support for new platforms by implementing the hardware abstraction layer.

## 3. Build System
- [ ] Simplify the Makefile; provide clear targets for building library, examples, and cleaning
  - **Portability Tip:** Structure the build system so platform selection is easy (e.g., `make PLATFORM=ESP32`). Avoid hardcoding platform macros.
- [ ] Add dependency checks and helpful error messages
- [ ] Consider providing a CMake configuration for cross-platform builds
  - **Portability Tip:** CMake makes it easier to support multiple platforms and toolchains.

## 4. API Simplification
- [ ] Abstract away low-level details for common use cases (e.g., `display_image(path, vcom, mode)`)
  - **Portability Tip:** Keep the public API platform-agnostic; all platform-specific details should be hidden in the hardware abstraction layer.
- [ ] Provide a simple, high-level API for image display
- [ ] Document all public functions with clear usage and parameter descriptions

## 5. User Experience Improvements
- [ ] Add command-line help and argument validation to example programs
- [ ] Provide pre-built binaries for common platforms (optional)
  - **Portability Tip:** For MCUs, provide example firmware projects instead of binaries.
- [ ] Add scripts/utilities for image conversion and testing
- [ ] Add error handling and user-friendly messages throughout

## 6. Testing & CI
- [x] Add basic unit tests for core logic (where possible)
  - **Fonts module and text rendering are now fully covered by automated tests.**
  - **Platform abstraction, buffer logic, and e-Paper driver are tested.**
  - **Portability Tip:** Structure tests so platform-agnostic logic can be tested on any platform; hardware-specific tests should be isolated.
- [ ] Set up continuous integration (CI) for build/test on supported platforms
  - **Portability Tip:** Consider using GitHub Actions or similar to test builds for all platforms.

## 7. Community & Contribution
- [ ] Add a `CONTRIBUTING.md` guide
  - **Portability Tip:** Include a section: “How to add support for a new platform (e.g., ESP32)” with a checklist and pointers to the abstraction layer.
- [ ] Add issue templates and PR guidelines

---

## Priorities
1. Code organization and documentation
2. API simplification and user experience
3. Build system improvements
4. Testing and community support

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