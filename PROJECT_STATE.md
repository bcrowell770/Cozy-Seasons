# Project State

## Current Status

v1.1 has been published. The watchface is functional and building successfully in CloudPebble.

## Completed

- Seasonal background system implemented
- 24 hourly images per season wired
- Seasonal particle effects implemented
- Tap/shake effect triggering implemented
- Bluetooth disconnect icon implemented
- Battery bar implemented
- Clay settings implemented for:
  - text color
  - show date
  - animate seasonal effects
  - season mode
  - manual season
  - Bluetooth icon
  - Bluetooth vibration
- Low Vision Mode setting added
- Separate AM/PM layer implemented
- Layout logic centralized into `apply_layout()`
- Low vision backing panel implemented
- Time/date vertical hierarchy polished for Basalt
- Low vision date pill implemented with shared contrast logic
- Current low-vision time font applied in both normal and low vision modes
- Unused normal-mode custom time font path removed
- Date layer overflow mode aligned with the single-line Basalt layout
- 24-hour time now centers visually when AM/PM is hidden
- Date text rects enlarged and shifted to protect descenders in both modes
- 24-hour time frames widened for worst-case Basalt digit combinations
- 12-hour and 24-hour time frames widened again for midnight/worst-case strings
- 12-hour time/AM-PM spacing tightened again to target midnight without changing 24-hour behavior
- 12-hour time frames widened again with a stronger left shift to target `12:08 AM`-style worst cases
- 12-hour main time frame widened again to make the time portion the full left-hand block up to the AM/PM boundary
- 12-hour time/AM-PM unit rebalanced slightly to the right for better visual centering
- Battery bar track rendering is now consistent between normal and low vision modes
- Bluetooth icon frame enlarged and restored to the right-center area in both modes
- Bluetooth icon layer now matches the 30x30 asset size instead of using a smaller clipping frame
- CloudPebble message key issue identified and resolved

## Current Behavior Notes

- 12-hour mode now fits cleanly in Low Vision Mode
- Leading zero is removed in 12-hour mode
- Time now sits higher in both modes with clearer separation from the date
- Date now sits lower, just above the battery bar, in both modes
- Low vision mode uses a tighter main time panel plus a separate date pill
- Low vision panel contrast behavior is working well
- Date is now treated as a single-line label rather than a wrapping block
- 24-hour mode centers the time within the available area instead of keeping it right-biased
- 24-hour mode now uses wider dedicated time frames than before in both modes
- Time frames now explicitly target worst-case strings including `12:00`, `12:00 AM`, `00:00`, `23:59`, and `08:08`
- 12-hour layout now gives the main time a bit more width and packs AM/PM closer to the time
- The 12-hour path now uses a more aggressive left-shifted time frame while leaving the 24-hour path unchanged
- The 12-hour time layer now starts at the left edge of its layout block in both modes
- The 12-hour line is now nudged slightly right so the combined time + AM/PM unit feels better centered
- Both battery modes now show a full track plus charged fill; low vision differs by thickness only
- Bluetooth icon now uses the right-center placement in both modes instead of moving to the top-right in low vision mode
- Bluetooth icon frame sizing now matches the underlying asset dimensions

## Known Issues / Polish Needed

- None

## Build / Tooling Notes

CloudPebble requires Clay keys to be present in two places:

1. `config.js`
2. CloudPebble Settings → PebbleKit JS Message Keys

If only `config.js` is updated, build may fail with undeclared `MESSAGE_KEY_*`.

## Next Milestone

No active milestone. Future work should start from a new task.
