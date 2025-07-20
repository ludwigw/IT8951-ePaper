# SPI and Hardware Initialization Diff Plan: IT8951-ePaper

This document tracks the systematic comparison between the official Waveshare demo and the current codebase, focusing on hardware initialization and SPI configuration. Use the checklist to mark off each difference as it is tested or resolved.

---

## Checklist: Differences to Test

- [ ] **SPI Bit Order**
  - Official demo: LSB first
  - Current code: MSB first
  - **Action:** Change to LSB first in your codebase and test.

- [ ] **SPI Speed**
  - Official demo: 20 MHz (or 12.5 MHz)
  - Current code: 7.8 MHz (divider 32), previously tried 3.9 MHz (divider 64)
  - **Action:** Match the official demo’s speed if needed.

- [ ] **Data Interval**
  - Official demo: Sets 5 us interval between bytes
  - Current code: No explicit interval set (bcm2835 may not expose this)
  - **Action:** Investigate if this is needed/possible with bcm2835.

- [ ] **BUSY Pin Pull-up**
  - Official demo: No explicit pull-up
  - Current code: Pull-up enabled
  - **Action:** Test with and without pull-up if issues persist.

- [ ] **Reset Sequence Delays**
  - Official demo: 200ms/10ms/200ms
  - Current code: 500ms/50ms/500ms (recently changed)
  - **Action:** Try reverting to official timing if needed.

- [ ] **Command Order/Initialization**
  - Ensure the order of hardware init, reset, and first commands matches exactly.

- [ ] **BMP Format**
  - Confirm the BMP file written by your CLI matches the format expected by the demo (bit depth, palette, etc.).

---

## Notes
- Mark each item as complete (`[x]`) after testing.
- Add observations and results for each test below the checklist. 