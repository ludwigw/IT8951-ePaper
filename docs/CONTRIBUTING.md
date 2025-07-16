# Contributing to IT8951-ePaper

Thank you for considering contributing to this project!

## How to Contribute
- Open issues for bugs, questions, or feature requests.
- Submit pull requests for improvements, bugfixes, or new features.
- Follow code style and documentation guidelines.

## How to Add Support for a New Platform (e.g., ESP32, STM32)

1. **Create a new hardware abstraction file** (e.g., `DEV_Config_esp32.c/h`).
2. **Implement all required functions** as declared in `DEV_Config.h` using your platform’s SDK (GPIO, SPI, delays, etc.).
3. **Keep all hardware-specific code in the abstraction layer.**
4. **Update the build system** to allow selecting your platform (e.g., via Makefile or CMake option).
5. **Test thoroughly** on your hardware.
6. **Document any platform-specific requirements or limitations.**
7. **Submit a pull request** with your changes and documentation updates.

**Checklist:**
- [ ] New abstraction files created and named clearly
- [ ] All required functions implemented
- [ ] No platform-specific code in display/GUI logic
- [ ] Build system updated
- [ ] Documentation updated (architecture, quickstart, etc.)
- [ ] Tests (if possible) added or updated

For more details, see [docs/architecture.md](architecture.md) and [docs/PLAN.md](PLAN.md). 