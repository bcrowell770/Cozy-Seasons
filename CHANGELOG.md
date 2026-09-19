# Changelog

All notable changes to this watchface will be documented here.

## [1.4.0]

- Spring petals now have rounded tops and tapered bases for a more recognizable petal shape.
- Autumn leaves now have pointed, asymmetric shapes and short stems.
- Preserved the mixed particle sizes, seasonal colors, and smooth movement introduced in v1.3.0.
- All 96 hourly seasonal backgrounds remain unchanged.

## [1.3.0]

- More natural seasonal animations with smoother movement and independently varied drift.
- Fireflies now wander gently in all directions, with slower, varied blinking.
- Larger particles across all seasons, while preserving their mix of sizes.
- Spring petals now feature pink, lavender-pink, pale yellow, and white.
- Improved summer pollen with delicate seed fluff and a slower, wind-driven motion.
- Repeated shakes no longer interrupt and restart an active animation.
- All 96 hourly seasonal backgrounds remain unchanged.

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

### Validation

- Built locally with Pebble SDK 4.17 and successfully sideloaded and tested on Basalt hardware

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
