# Changelog

All notable changes to this watchface will be documented here.

## [Unreleased]

No unreleased changes.

---

## [1.2] - 2026-07-18

### Added

- Complete local Pebble SDK project files and resource manifest
- Reproducible local Basalt build support through the Pebble SDK
- Background resource allocation failure logging

### Changed

- Reduced the 24 winter background images from 1,535,595 bytes to 227,863 bytes by converting them to the native Basalt color palette
- Load the initial background before smaller UI allocations to reduce heap fragmentation
- Reduced the unused AppMessage outbox allocation from 256 bytes to 64 bytes

### Fixed

- Retry a failed background allocation on the next minute tick instead of leaving the watchface blank until the following hour
- Detach the bitmap layer before destroying its background bitmap, preventing a stale pointer during image changes

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
