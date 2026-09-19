# Project State

## Current Version: v1.4.0

- Basalt watchface with all 96 hourly backgrounds retained.
- v1.3.0 introduced smoother fractional particle motion, independent drift, larger mixed sizes, pastel spring colors, and improved summer seed fluff. The user confirms v1.3.0 was published.
- v1.4.0 replaces rounded-block petals and leaves with tapered petals and pointed, asymmetric leaves with short stems. Both sizes have explicit silhouettes and mirrored orientations.
- The user approved the animations and final silhouettes in CloudPebble.
- The SDK 4.17 build passed; the final PBW's internal version label was verified as 1.4.0.
- Release artifact: `Cozy-Seasons-v1.4.0.pbw`.

## Remaining Work

- Investigate slow transfers on Time 1 and Time 2 with timed installs and logs.
- The SDK reports 709,940 resource bytes against its 256 KB publishing limit. This existing warning remains unresolved.
- A local emulator crashed during earlier testing; the user subsequently confirmed successful CloudPebble loading and animation review. Hardware stability is not established by the build checks.
- Reduced-palette experiments were not applied because of visible quality changes. Preserve hourly backgrounds while evaluating better compression; consider 12 images per season only as a fallback for review.

## Source Layout

- `src/c/main.c` is the local SDK's compiled watchface source.
- `main.c` and `src/main.c` mirror the watchface logic for existing import workflows.
- `src/pkjs/` contains Clay settings and JavaScript setup.
- `package.json` contains version, platform, message keys, and resources.
