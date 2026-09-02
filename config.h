#pragma once

// =====================================================================
// SPI bus wiring for the Cirque Pinnacle (GlidePoint) trackpad module.
// No SPI_DRIVER override needed here -- with no BOARD set in rules.mk,
// QMK's default RP2040 profile already assigns SPI_DRIVER = SPID0 and
// enables that peripheral. We only need to override the actual pins to
// match how the board is physically wired.
// =====================================================================
#define SPI_SCK_PIN GP2
#define SPI_MOSI_PIN GP3
#define SPI_MISO_PIN GP4
#define POINTING_DEVICE_CS_PIN GP5

// =====================================================================
// Cirque Pinnacle / GlidePoint 40mm circle trackpad settings
// (updated from 35mm -- see calibration note below)
// =====================================================================
#define CIRQUE_PINNACLE_DIAMETER_MM 40

// The X/Y bounds below were calibrated for the ORIGINAL 35mm module and
// this specific case fit -- they do NOT necessarily carry over to a new
// 40mm module, even though it's electrically the same sensor family.
// Manufacturing tolerance between individual units, plus how the new
// module physically sits in the case, can shift the practical reachable
// range. Re-run the calibration sweep with the 40mm module wired up:
//   1. Uncomment CIRQUE_PINNACLE_REACHABLE_CALIBRATION below, reflash
//   2. `qmk console`, then firmly sweep to each edge (left/right/top/
//      bottom/diagonals) a few times
//   3. Watch xLo/xHi/yLo/yHi until they stop climbing
//   4. Update the 4 defines below with the new numbers, then comment
//      CIRQUE_PINNACLE_REACHABLE_CALIBRATION back out
// Leaving the old 35mm numbers in for now as a rough starting point --
// tap-to-click and cursor movement will still work with these, just
// not as precisely tuned to the new pad's real edges until recalibrated.
#define CIRQUE_PINNACLE_X_LOWER 300
#define CIRQUE_PINNACLE_X_UPPER 1750
#define CIRQUE_PINNACLE_Y_LOWER 140
#define CIRQUE_PINNACLE_Y_UPPER 1365
// #define CIRQUE_PINNACLE_REACHABLE_CALIBRATION

// Left undefined -> defaults to CIRQUE_PINNACLE_ABSOLUTE_MODE, which is
// what the known-working firmware this is based on uses. Cursor movement
// works normally in this mode either way.
// #define CIRQUE_PINNACLE_POSITION_MODE

#define CIRQUE_PINNACLE_TAP_ENABLE

// Circular/perimeter scroll (POINTING_DEVICE_GESTURES_SCROLL_ENABLE) is
// intentionally NOT defined here. That gesture only checks touch
// position at the exact moment of first contact, which doesn't fit a
// pad where your finger always enters from roughly the same spot --
// no threshold tuning can fix that mismatch. Scroll is instead handled
// in keymap.c via a hold-to-scroll button modifier (see
// pointing_device_task_user), which works regardless of where on the
// pad you touch down.

// Optional: tap the upper-right corner (partially off the sensor) to
// right-click via gesture, on top of tap-to-click above. Uncomment if
// you want tap-based right click in addition to the GP7 hardware button.
// #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE

// Orientation adjustments -- tune these to match how you physically
// mounted the pad. ROTATION_180 was confirmed correct for this build
// (moving finger right/up correctly moves the cursor right/up).
#define POINTING_DEVICE_ROTATION_180
// #define POINTING_DEVICE_INVERT_X
// #define POINTING_DEVICE_INVERT_Y

// --- TEMPORARY: verbose driver debug output, remove once scroll works ---
#define POINTING_DEVICE_DEBUG
