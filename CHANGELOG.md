# Changelog

All notable changes to this watchface will be documented here.

## [Unreleased]

No unreleased changes.

---

## [1.1] - 2026-04-11

### Added

- Low Vision Mode with backing panel and improved readability

### Changed

- Split AM/PM into separate layer for better layout control
- Reworked layout system using apply_layout()

### Fixed

- 12-hour time truncation issues
- 24-hour time truncation (ellipses) in worst-case widths
- Date descender clipping (e.g., "p" in Apr)
- Further widened the 12-hour time area to target remaining midnight/near-midnight ellipsis cases
- Rebalanced the 12-hour time and AM/PM line so it sits more naturally after the midnight fit fixes
- Restored a consistent full battery-track display in Low Vision Mode while keeping its thicker bar
- Restored the Bluetooth disconnect icon to a fully visible right-center position in both normal and low vision modes
- Matched the Bluetooth icon layer to the full 30x30 asset size to prevent clipping

---

## [1.0] - Initial Functional Build

### Added

- Seasonal backgrounds with hourly images
- Particle animations (tap/shake triggered)
- Bluetooth disconnect indicator
- Battery bar
- Clay settings system

### Notes

- Initial working version
