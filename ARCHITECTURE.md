# Architecture

## Project
Seasonal Pebble watchface for Basalt (144x168), written in C with Clay configuration.

## Current Release
- Published version: v1.1
- v1.1 focus: layout polish, Low Vision Mode, AM/PM split, Bluetooth icon sizing, and battery/date readability fixes

## Platform
- Pebble Basalt only
- Resolution: 144x168
- Performance-sensitive
- No continuous animation

## Core Features
- Seasonal background images
  - Spring
  - Summer
  - Autumn
  - Winter
- 24 hourly images per season
- Seasonal particle effects triggered by tap/shake
  - Spring: petals
  - Summer: seeds by day, fireflies by night
  - Autumn: leaves
  - Winter: snow
- Time and date display
- Bluetooth disconnect icon
- Battery bar
- Clay settings for appearance and behavior

## Layout System
The watchface uses layered rendering:

1. Background image layer
2. Seasonal effect layer
3. Optional low-vision backing panel layer
4. Time layer
5. AM/PM layer
6. Date layer
7. Battery layer
8. Bluetooth icon layer

## Time Rendering
Time is split into:
- main time layer
- separate AM/PM layer

This avoids layout issues on Basalt in 12-hour mode and allows tighter control over spacing.

## Modes
### Normal Mode
- Standard layout
- Lightweight presentation
- No backing panel

### Low Vision Mode
- Larger / clearer layout
- Backing panel behind time
- Separate date pill
- Higher contrast
- Larger battery bar
- Intended to improve readability without significantly changing performance

## Settings
Current settings include:
- TextColor
- ShowDate
- AnimateLeaves
- ShowBtIcon
- BtVibrate
- SeasonMode
- ManualSeason
- LowVisionMode

## Persistence
Settings are stored via Pebble persistent storage using a single settings struct.

## Background Logic
Season can be:
- automatic by month
- manual
- daily random

Background image changes based on:
- effective season
- current hour

## Animation Logic
Particle effects are:
- generated in code
- triggered by tap/shake
- time-limited
- not continuous

## Constraints
- Must remain clean on Basalt
- Avoid clutter
- Preserve seasonal aesthetic
- Keep rendering cheap
- Keep Bluetooth icon and battery from colliding with text
