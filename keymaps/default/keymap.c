#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _SCROLL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Right button is dual-purpose:
    //   - a quick tap  -> normal right click (MS_BTN2)
    //   - held down    -> activates the _SCROLL layer while held
    // Left click stays a plain button so click-and-drag text selection
    // keeps working normally -- that's why this isn't on the left button.
    // This is QMK's built-in layer-tap (LT), so timing (how long counts
    // as a "hold") is governed by TAPPING_TERM, default 200ms.
    [_BASE] = LAYOUT(
        MS_BTN1,             // Left click switch  -> GP6
        LT(_SCROLL, MS_BTN2) // Right click switch -> GP7 (tap = click, hold = scroll mode)
    ),
    // No keys change on this layer -- it's used purely as a flag that
    // pointing_device_task_user() checks below. KC_TRNS keeps both
    // positions "transparent" (falls through to base layer behavior)
    // in case the layer is ever active during a keypress.
    [_SCROLL] = LAYOUT(
        KC_TRNS,
        KC_TRNS
    )
};

// --- Scroll-via-button-hold ---
//
// While the _SCROLL layer is active (right button held down), redirect
// normal cursor movement (x/y) into scroll (h/v) instead. This is
// intent-based rather than position-based, so it works no matter where
// on the pad your finger happens to land -- unlike the driver's built-in
// circular-scroll gesture, which only checks touch position at the exact
// moment of first contact and doesn't fit this pad's fixed entry point.
//
// If scroll direction feels backwards once tested, flip the relevant
// sign below (e.g. change `mouse_report.y` to `-mouse_report.y`).

// --- TEMPORARY: debug console, remove once scroll is confirmed working ---
// POINTING_DEVICE_DEBUG (config.h) only compiles debug prints IN -- this
// turns on the separate runtime flag that actually makes them fire.
void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_mouse  = true;

    // Lower overall cursor sensitivity. This pad's default works out to
    // roughly 740 CPI given its 35mm diameter -- drop it further here if
    // it still feels too fast. Higher number = faster cursor.
    pointing_device_set_cpi(400);
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static int16_t accum_h = 0;
    static int16_t accum_v = 0;

    // How many units of raw finger movement it takes to produce one
    // scroll "tick". Higher = less sensitive / slower scrolling.
    const int8_t SCROLL_DIVISOR = 6;

    if (layer_state_is(_SCROLL)) {
        int16_t dx  = mouse_report.x;
        int16_t dy  = mouse_report.y;
        int16_t adx = dx < 0 ? -dx : dx;
        int16_t ady = dy < 0 ? -dy : dy;

        // Lock to whichever axis dominates THIS sample, so a mostly-
        // vertical swipe doesn't also nudge horizontal scroll (and vice
        // versa). Only the winning axis accumulates; the other is left
        // untouched rather than reset, so genuinely diagonal drags still
        // scroll smoothly rather than stuttering axis to axis.
        if (adx > ady) {
            accum_h += dx;
        } else if (ady > adx) {
            accum_v += dy;
        }

        mouse_report.h = 0;
        mouse_report.v = 0;
        if (accum_h >= SCROLL_DIVISOR || accum_h <= -SCROLL_DIVISOR) {
            mouse_report.h = accum_h / SCROLL_DIVISOR;
            accum_h %= SCROLL_DIVISOR;
        }
        if (accum_v >= SCROLL_DIVISOR || accum_v <= -SCROLL_DIVISOR) {
            mouse_report.v = accum_v / SCROLL_DIVISOR;
            accum_v %= SCROLL_DIVISOR;
        }
        mouse_report.x = 0;
        mouse_report.y = 0;

        if (mouse_report.h != 0 || mouse_report.v != 0) {
            dprintf("SCROLL h=%d v=%d\n", mouse_report.h, mouse_report.v);
        }
    } else {
        // Reset accumulators whenever not actively scrolling, so stray
        // leftover sub-threshold movement doesn't cause a surprise tick
        // the next time scroll mode is entered.
        accum_h = 0;
        accum_v = 0;
    }
    return mouse_report;
}
